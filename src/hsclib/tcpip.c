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

#include "hsclib/tcpip.h"
#include "hsclib/ldebug.h"
#include "hsclib/lmessage.h"
#include "hsclib/msgid.h"
#ifdef UNIX
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#else
#error "Non-UNIX systems not supported yet in TCP/IP module"
#endif

#define HTTP_METHOD ("HEAD ")
#define HTTP_VERSION (" HTTP/1.0")

/* the following protocols are only supported via proxy */
#define check_ftp_uri(h,u,p) (check_proxyonly_uri(h,u,p))
#define check_gopher_uri(h,u,p) (check_proxyonly_uri(h,u,p))

static BOOL get_http_hdr(HSCPRC *hp, CONSTRPTR uri, CONSTRPTR host, CONSTRPTR res, unsigned short port) {
   EXPSTR *message;
   BOOL ret = FALSE;
   char rcvbuf[16]={0};
   int sock;
   struct sockaddr_in saddr;
   struct hostent *he;
   char *reqbuf;

   
   D(fprintf(stderr,DHL "get_http_hdr(%s,%s,%s,%hd)\n",uri,host,res,port);)

   /* top directories must be requested as "/", even though they are sometimes
    * not explicitly given in the URI */
   if('\0' == *res) res = "/";

   /* create socket */
   if(-1 != (sock = socket(PF_INET, SOCK_STREAM, 0))) {
      /* construct HTTP request */
      reqbuf = umalloc(strlen(res) + strlen(HTTP_METHOD) + strlen(HTTP_VERSION) + 5);
      strcpy(reqbuf,HTTP_METHOD);
      strcat(reqbuf,res);
      strcat(reqbuf,HTTP_VERSION);
      strcat(reqbuf,"\r\n\r\n");
      /* resolve server name */
      if(NULL != (he = gethostbyname(host))) {
         saddr.sin_family = AF_INET;
         saddr.sin_port = htons(port);
         saddr.sin_addr.s_addr = *((unsigned long*)(he->h_addr_list[0]));
         D(
               {
               unsigned long a=ntohl(saddr.sin_addr.s_addr);
               fprintf(stderr,DHL "Contacting %s (%lu.%lu.%lu.%lu:%u)\n",
                  host,a>>24,(a>>16)&0xff,(a>>8)&0xff,a&0xff,port);
               }
          )
         /* connect to server */
         if(-1 != connect(sock,(struct sockaddr*)&saddr,sizeof(saddr))) {
            /* send request */
            write(sock,reqbuf,strlen(reqbuf));
            /* read first part of response: "HTTP/x.x xxx " */
            read(sock,rcvbuf,13);
            if(0 == strncmp(rcvbuf,"HTTP/",5)) {
               switch(rcvbuf[9]) {
                  case '2' :
                     ret = TRUE;    /* success! */
                     break;
                  case '4' :
                  case '5' :
                     hsc_message(hp, MSG_NO_EXTURI,
                           "external URI \"%s\" does not exist",uri);
                     break;
                  default :
                     message = init_estr(32);
                     set_estr(message,rcvbuf+9);
                     /* read rest of response */
                     rcvbuf[1] = '\0';
                     /* yes, this sucks */
                     while(1) {
                        read(sock,rcvbuf,1);
                        if('\r' == rcvbuf[0]) break;
                        app_estr(message,rcvbuf);
                     }
                     hsc_message(hp, MSG_NO_EXTURI,
                           "dubious external URI \"%s\": server said \"%s\"",uri,estr2str(message));
                     del_estr(message);
                     ret = TRUE; /* we'll count this as OK... */
                     break;
               }
            } else {
               D(fprintf(stderr,DHL "Apparently HTTP/0.9; server response: '%s'\n",rcvbuf);)
            }
         } else perror("HSC");
      }
      ufree(reqbuf);
      close(sock);
   } else perror("HSC");
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

static BOOL check_http_uri(HSCPRC *hp, CONSTRPTR uri, CONSTRPTR proxy) {
   EXPSTR *host = init_estr(32);
   EXPSTR *res = init_estr(32);
   unsigned short port=80;
   BOOL ret,done;
   const char *p;

   if(!parse_proxy_addr(proxy,host,&port)) {
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
   ret = get_http_hdr(hp,uri,estr2str(host),uri,port);

   del_estr(host);
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
      ret = get_http_hdr(hp,uri,estr2str(host),uri,port);
   }

   del_estr(host);
   return ret;
}

BOOL check_ext_uri(HSCPRC *hp, char *uri) {
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
