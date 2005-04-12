/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
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
 * amiga/msgbrowser.c
 *
 * implements throwback functions for SAS/c message browser
 */

#ifndef AMIGA
/* Avoid that anything is attempted to be compiled on none-Amiga systems.
 * A dummy symbol is declared to keep the linker from whining. */
char amiga_message_browser_dummy = 0;
#else

#include "hsc/global.h"
#include "ugly/fname.h"

#include <stdio.h>

#define DEBUG_MSGBROWSER 0

/* Name of SAS/c message browser's ARexx port */
#define SC_SCMSG "SC_SCMSG"

/* Buffer that is hopefully big enough to take the result of sprintf() when
 * creating ARexx commands in it. C sucks. */
static STRARR arexx_command[2048];

/* Command used to invoke VSend if VBrowse is used */
static STRPTR vsend_command = arexx_command;

/* Temporary file to where VBrowse messages should be directed */
static STRPTR vbrowse_filename = NULL;
static FILE *vbrowse_file = NULL;


/* Name of the "compilation unit" for SAS/c; this usually is the name
 * of the main (last) hsc source passed in the command line */
static STRPTR compilation_unit = NULL;

static void send_arexx_command(HSCPRC * hp, STRPTR arexx_command)
{
    /* Hex-escaped Arexx command */
    EXPSTR *escaped_arexx_command = init_estr(256);
    STRPTR command_character = NULL;
    BOOL insert_concatenation;

    set_estr(escaped_arexx_command, "RX >nil: \"cmd='");

    /* Hex-escape nasty characters in command before sending it via RX.
     * Probably more characters then neccessary are escaped, but extending
     * this list is faster then finding out if a character is nasty or not.
     */
    command_character = arexx_command;
    insert_concatenation = TRUE;
    while (command_character[0] != '\0')
    {
        STRARR hex_buffer[10];

        if (insert_concatenation)
        {
            app_estr(escaped_arexx_command, "'||'");
        }
        switch (command_character[0])
        {
        case '\'':
        case '\"':
        case '*':
        case '`':
            sprintf(hex_buffer, "'||'%x'x'", command_character[0]);
            app_estr(escaped_arexx_command, hex_buffer);
            insert_concatenation = TRUE;
            break;

        default:
            app_estrch(escaped_arexx_command, command_character[0]);
            insert_concatenation = FALSE;
            break;
        }

        command_character += 1;
    }
    app_estr(escaped_arexx_command,"';");
#if DEBUG_MSGBROWSER
    app_estr(escaped_arexx_command,"call open(f,'console:'); call writeln(f,cmd);");
#endif
    app_estr(escaped_arexx_command,"address " SC_SCMSG " cmd");
    app_estr(escaped_arexx_command, "\"");

#if DEBUG_MSGBROWSER
    fprintf(stderr, "sending arexx: %s\n",
            estr2str(escaped_arexx_command));
#endif
    system(estr2str(escaped_arexx_command));

    del_estr(escaped_arexx_command);
}

BOOL init_msg_browser(HSCPRC * hp, STRPTR filename)
{
   BOOL success = FALSE;

   if (!upstrcmp(msg_browser, "SCMSG"))
   {
      /* SAS message browser */
      compilation_unit = strclone(filename);

      /* Start browser, if it is not already running */
      if (1)
      {
         int return_code = 0;
         system("RX 'if ~show(p," SC_SCMSG ") then address command \"run <>nil: sc:c/scmsg hidden\"");
         return_code = system("WaitForPort " SC_SCMSG "");
         if (return_code != 0)
         {
            D(fprintf(stderr, "could not start message browser."));
         }
      }

      /* Clear all messages for this document */
      sprintf(arexx_command, "DelComp \"%s\"", compilation_unit);
      send_arexx_command(hp, arexx_command);

      success = TRUE;
   }
   else if (!upstrcmp(msg_browser, "VBROWSE"))
   {
      /* VBrowse */
      vbrowse_filename = tmpnamstr("t:vbrowse-");
      if (vbrowse_filename != NULL)
      {
         vbrowse_file = fopen(vbrowse_filename, "w");
         if (vbrowse_file != NULL)
         {
            size_t fill_counter = strlen(filename);;

            /* Write a title line */
            fprintf(vbrowse_file, "---- %s ", filename);
            while (fill_counter < 77)
            {
               fputc('-', vbrowse_file);
               fill_counter += 1;
            }
            fputc('\n', vbrowse_file);

            /* Remember temporary filename in own buffer as the buffer
             * returned by tmpnamstr() is static and will be overwritten
             * next time it is called. */
            vbrowse_filename = strclone(vbrowse_filename);

            success = TRUE;
         }
      }

      if (!success)
      {
         status_error("unable to open VBrowse redirection file");
      }
   }

   return success;
}

void del_msg_browser(HSCPRC * hp)
{
    if (!upstrcmp(msg_browser, "SCMSG"))
    {
        ufreestr(compilation_unit);
    }
    else if (!upstrcmp(msg_browser, "VBROWSE"))
    {
        /* Evaluate the command for VSend */
        sprintf(arexx_command, "vsend <%s", vbrowse_filename);

        /* Close the temporary message file, pipe it into VSend and
         * remove it.  */
        fclose(vbrowse_file);
        system(vsend_command);
#if 1
        remove(vbrowse_filename);
#endif

        ufreestr(vbrowse_filename);
    }
}

void send_msg_browser(HSCPRC * hp,
                      HSCMSG_CLASS msg_class, HSCMSG_ID msg_id,
                      STRPTR fname, ULONG x, ULONG y,
                      STRPTR msg_text)
{
    if (!upstrcmp(msg_browser, "SCMSG"))
    {
        STRPTR msg_class_str = "*unknown*";

        /* Convert hsc-message class to ScMsg class */
        switch (msg_class)
        {
        case MSG_NOTE:
            msg_class_str = "Note";
            break;
        case MSG_STYLE:
        case MSG_PORT:
        case MSG_WARN:
            msg_class_str = "Warning";
            break;
        default:
            msg_class_str = "Error";
            break;
        }

        /* NewMsg "compunit" "file" line 0 "" 0 <class> <errnum> <text> */
        sprintf(arexx_command, "NewMsg \"%s\" \"%s\" %ld 0 \"\" 0 %s %lu %s",
                compilation_unit, fname, y, msg_class_str, msg_id, msg_text);

        send_arexx_command(hp, arexx_command);
    }
    else if (!upstrcmp(msg_browser, "VBROWSE"))
    {
        STRPTR msg_class_str = "*unknown*";

        /* Convert hsc-message class to VBrowse class. Note that VBrowse
         * requires the class to be lower case. */
        switch (msg_class)
        {
        case MSG_NOTE:
        case MSG_STYLE:
        case MSG_PORT:
        case MSG_WARN:
            msg_class_str = "warning";
            break;
        default:
            msg_class_str = "error";
            break;
        }

        /* Brutally append it to the VBrowse message file without any
         * error checking. */
        if (vbrowse_file != NULL)
        {
            /* As hsc does not display the offending source line, write
             * an empty line instead */
            fprintf(vbrowse_file, ">\n");

            /* Write the line with all the error information */
            fprintf(vbrowse_file, "%s %ld in line %ld of \"%s\": %s\n",
                    msg_class_str, msg_id, y, fname, msg_text);
        }
    }

}

#endif /* AMIGA */
