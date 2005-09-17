/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 2003 Matthias Bethke
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
/*
 * hsclib/tcpip.c
 *
 * Functions for TCP/IP-based protocols support
 *
 * created:  23-Dec-2003
 */

#include "sysdep.h"

#include "hsclib/tcpip.h"
#include "hsclib/ldebug.h"
#include "hsclib/lmessage.h"
#include "hsclib/msgid.h"
#include "ugly/umemory.h"
#include <ctype.h>
#include <stdio.h>
#include <errno.h>

#ifdef AMIGA
#include <proto/exec.h>
#include <proto/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
struct Library *SocketBase;
#elif defined UNIX && HAVE_UNISTD_H
#ifdef SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif
#else
/* this system is not yet supported, just create a dummy function */
BOOL check_ext_uri(HSCPRC *hp, char *uri)
{
   hsc_message(hp, MSG_NO_EXTCHECK, "cannot check external URIs -- system not supported");
   return FALSE;
}
#endif

#if defined(UNIX) || defined(AMIGA)

#define HTTP_METHOD ("HEAD ")
#define HTTP_VERSION (" HTTP/1.0")
#define RCVBUF_SIZE (256)
#define REPLYLEN (12)

/* the following protocols are only supported via proxy */
#define check_ftp_uri(h,u,p) (check_proxyonly_uri(h,u,p))
#define check_gopher_uri(h,u,p) (check_proxyonly_uri(h,u,p))

/* get_http_hdr
 * Tries an HTTP/1.0 HEAD request for the given URI and returns a success code
 * Arguments:
 * fulluri: complete URI, only used for error reporting
 * host   : name of server to contact. May be NULL if proxy!=NULL
 * proxy  : name of proxy, if any. NULL otherwise
 * res    : resource to fetch from server. Set res==fulluri unless proxy==NULL!
 * port   : port of server/proxy to contact. Usually 80 for HTTP, 3128 for Squid
 *          proxies
 */
static BOOL get_http_hdr(
      HSCPRC *hp,
      CONSTRPTR fulluri,
      CONSTRPTR host,
      CONSTRPTR proxy,
      CONSTRPTR res,
      unsigned short port)
{
   EXPSTR *message, *request;
   BOOL reading, ret = FALSE;
   int sock, nread, n;
   struct sockaddr_in saddr;
   struct hostent *he;
   char *rcvbuf, *cr=NULL;

   
   D(fprintf(stderr,DHL "get_http_hdr(%s,%s,%s,%s,%hd)\n",fulluri,host,proxy,res,port);)

#ifdef AMIGA
      if(NULL == (SocketBase = OpenLibrary("bsdsocket.library",0))) {
         hsc_message(hp, MSG_NO_EXTCHECK,
               "cannot check external URIs -- no bsdsocket.library");
            return FALSE;
      }
#endif
   /* top directories must be requested as "/", even though they are sometimes
    * not explicitly given in the URI */
   if('\0' == *res) res = "/";

   /* allocate buffer for reply */
   rcvbuf = ucalloc(RCVBUF_SIZE,1);
      
   /* create socket */
   if(-1 != (sock = socket(PF_INET, SOCK_STREAM, 0))) {
      /* construct HTTP request */
      request = init_estr(64);
      set_estr(request,HTTP_METHOD);
      app_estr(request,res);
      app_estr(request,HTTP_VERSION);
      app_estr(request,"\r\n");
      
      /* if no proxy in use, send "Host:" line too (virtual server support) */
      if(NULL == proxy) {
         app_estr(request,"Host: ");
         app_estr(request,host);
         app_estr(request,"\r\n");
      }
      app_estr(request,"\r\n");
      
      /* resolve server/proxy name */
      if(NULL != (he = gethostbyname((proxy != NULL) ? proxy : host))) {
         saddr.sin_family = AF_INET;
         saddr.sin_port = htons(port);
         saddr.sin_addr.s_addr = *((unsigned long*)(he->h_addr_list[0]));
           
         /* connect to server */
         if(-1 != connect(sock,(struct sockaddr*)&saddr,sizeof(saddr))) {
            /* send request */
            D(fprintf(stderr,DHL "Requesting '%s' %s %s\n",
                  estr2str(request),(NULL==proxy)?"from":"via",(NULL==proxy)?host:proxy);)
            write(sock,estr2str(request),strlen(estr2str(request)));
            shutdown(sock,SHUT_WR);
            
            /* read first part of response: "HTTP/x.x xxx " */
            for(nread=0,reading=TRUE; reading && (REPLYLEN-nread);) {
               switch(n = read(sock,rcvbuf,REPLYLEN-nread)) {
                  case -1 :
                     hsc_message(hp, MSG_DUBIOUS_EXTURI,
                           "dubious external URI \"%s\": error reading from server: %s",
                           fulluri,strerror(errno));
                     reading = FALSE;
                     break;
                  case 0 :
                     hsc_message(hp, MSG_DUBIOUS_EXTURI,
                           "dubious external URI \"%s\": short read from server",
                           fulluri);
                     reading = FALSE;
                     break;
                  default :
                     nread += n;
                     break;
               }
            }
            if(REPLYLEN == nread) {
               D(fprintf(stderr,DHL "Response: %s\n",rcvbuf);)
               if(0 == strncmp(rcvbuf,"HTTP/",5)) {
                  switch(rcvbuf[9]) {
                     case '2' :
                        ret = TRUE;    /* success! */
                        break;
                     case '4' :
                     case '5' :
                        hsc_message(hp, MSG_NO_EXTURI,
                              "external URI \"%s\" does not exist (error code %s)",fulluri,rcvbuf+9);
                        break;
                     case '3' :
                        /* count 302 as success */
                        if(('0' == rcvbuf[10]) && ('2' == rcvbuf[11])) {
                           ret = TRUE;
                           break;
                        }
                        /* fall through */
                     default :
                        message = init_estr(32);
                        set_estr(message,rcvbuf+9);
                        /* read rest of response */
                        while((nread = read(sock,rcvbuf,RCVBUF_SIZE-1)) > 0) {
                           if(-1 == nread) break;
                           if(NULL == cr) {
                              cr = strchr(rcvbuf,'\r');
                              if(NULL != cr)
                                 *cr = '\0';
                              app_estr(message,rcvbuf);
                           } /* else do nothing, just read message until end */
                        }
                        hsc_message(hp, MSG_DUBIOUS_EXTURI,
                              "dubious external URI \"%s\": server said \"%s\"",fulluri,estr2str(message));
                        del_estr(message);
                        ret = TRUE; /* we'll count this as OK... */
                        break;
                  }
               } else {
                  /* this should hardly ever happen and counts as OK */
                  D(fprintf(stderr,DHL "Apparently HTTP/0.9; server response: '%s'\n",rcvbuf);)
                     ret = TRUE;
               }
            } /* error handled before */
         } else {
            if(NULL==proxy)
               hsc_message(hp, MSG_NO_EXTURI,
                     "external URI \"%s\" does not exist (no connection to server)",fulluri);
            else
               hsc_message(hp, MSG_DUBIOUS_EXTURI, "dubious external URI \"%s\": proxy \"%s\" is down",
                     fulluri,proxy);
         }
      } else {
         if(NULL==proxy)
            hsc_message(hp, MSG_NO_EXTURI, "external URI \"%s\" does not exist: host address not found",
                  fulluri,proxy);
         else
            hsc_message(hp, MSG_DUBIOUS_EXTURI, "dubious external URI \"%s\": no address for proxy \"%s\"",
                  fulluri,proxy);
      }

      del_estr(request);
      close(sock);
   } else hsc_message(hp, MSG_NO_EXTCHECK, "socket() failed: %s)",strerror(errno));
   ufree(rcvbuf);
#ifdef AMIGA
   CloseLibrary(SocketBase);
#endif
   return ret;
}

/* parse_proxy_addr
 *
 * Splits a Lynx-style proxy URI into host part, stored in the EXPSTR <host>,
 * and a port, stored numerically in the short int pointed to by <port>
 * Returns TRUE on success, FALSE for NULL proxy or missing "http://" prefix.
 */
static BOOL parse_proxy_addr(CONSTRPTR proxy, EXPSTR *host, unsigned short *port) {
   BOOL done;
   CONSTRPTR p;

   if((NULL == proxy) || (0 != strncmp(proxy,"http://",7)))
      return FALSE;
   *port = 3128;   /* Squid default */
   for(p = proxy+7, done=FALSE; !done; ++p) {
      switch(*p) {
         case '\0' :
         case '/' :
            done = TRUE;
            break;
         case ':' :
            *port = atoi(++p);
            while(isdigit(*p)) ++p;
            break;
         default :
            app_estrch(host,*p);
            break;
      }
   }
   return TRUE;
}

static BOOL check_http_uri(HSCPRC *hp, CONSTRPTR uri, CONSTRPTR proxyvar) {
   EXPSTR *host = init_estr(32);
   EXPSTR *proxy = init_estr(32);
   EXPSTR *res = init_estr(32);
   unsigned short port=80;
   BOOL ret,done,use_proxy=TRUE;
   CONSTRPTR p,fulluri=uri;

   if(!parse_proxy_addr(proxyvar,proxy,&port)) {
      use_proxy = FALSE;
      /* no valid proxy specified, so <uri> has to be parsed */
      for(p = uri+7, done=FALSE; !done; ++p) {
         switch(*p) {
            case '\0' :
               done = TRUE;
               break;
            case '/' :
               set_estr(res,p);
               done = TRUE;
               break;
            case ':' :
               port = atoi(++p);
               while(isdigit(*p)) ++p;
               break;
            default :
               app_estrch(host,*p);
               break;
         }
      }
      /* without a proxy, only the server-relative part must be
       * sent in the request! */
      uri = estr2str(res);
   }
   ret = get_http_hdr(hp,fulluri,estr2str(host),
         use_proxy?estr2str(proxy):NULL,uri,port);

   del_estr(host);
   del_estr(proxy);
   del_estr(res);
   return ret;
}

static BOOL check_proxyonly_uri(HSCPRC *hp, CONSTRPTR uri, CONSTRPTR proxy) {
   EXPSTR *host = init_estr(32);
   unsigned short port=80;
   BOOL ret;

   if(!parse_proxy_addr(proxy,host,&port)) {
      /* FTP is only supported via proxy */
      ret = TRUE;
   } else {
      ret = get_http_hdr(hp,uri,uri,estr2str(host),uri,port);
   }

   del_estr(host);
   return ret;
}

BOOL check_ext_uri(HSCPRC *hp, const char *uri) {
   while(isspace(*uri)) ++uri;
   if(0 == strncmp(uri,"http://",7)) {
      return check_http_uri(hp,uri,getenv("http_proxy"));
   } else if(0 == strncmp(uri,"ftp://",6)) {
      return check_ftp_uri(hp,uri,getenv("ftp_proxy"));
   } else if(0 == strncmp(uri,"gopher://",9)) {
      return check_gopher_uri(hp,uri,getenv("gopher_proxy"));
   }
   /* URI scheme is not supported, so we'll just assume it's OK */
   return TRUE;
}
#endif /* defined(UNIX) || defined(AMIGA) */

/* $Id$*/
/* vi: set ts=4: */
