/*
 * SendAWeb.rexx - display local document in AWeb 
 *
 * $VER: SendAWeb 1.0 (2.8.96)
 *
 * Copyright 1996 Thomas Aglassinger <agi@giga.or.at>
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
 *--------------------------------------------------------------------------
 *
 * USAGE
 *   rx SendAWeb <document> [<port>]
 *
 *   This script tells AWeb to load or reload a document (usually *.html).
 *   It's intended to be used in your Makefile in the "%.html : %.hsc"-rule
 *   to automatically view the updated document.
 *
 *   To specify the local document, you should use a path relative to the
 *   current directory.
 *
 * EXAMPLE
 *   rx SendAWeb.rexx hugo/sepp.html
 *
 *   rx SendAWeb.rexx hugo/sepp.html AWEB.2
 *
 * EXAMPLE RULE (for your Makefile)
 *
 *   $(DESTDIR)%.html : %.hsc
 *        $(HSC) $(HSCFLAGS) $<
 *        rx SendAWeb.rexx $@
 *
 */

/* get user-args */
Parse ARG document port

/* show help end exit */
IF (document="") THEN DO
    SAY 'Usage: SendAWeb <document> [<port>]'
    Exit
END

/* use default port, if non passed */
IF (port="") THEN
    port = "AWEB.1"
ELSE
    port = STRIP(port)

/* compute document-uri */
uri = 'file://localhost/' || PRAGMA('D') || '/' || document

/* check, if AWeb is running */
IF ~Show('ports', port) THEN DO
    SAY "Couln't find AWeb on port `" || port || "'"
    Exit 10
END

/* tell AWeb to (re)load page*/
address VALUE port
'OPEN "' || uri || '" RELOAD'
