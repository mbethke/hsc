/*
 * hScMsg - invoke hsc and send messages to ScMsg
 *
 * $VER: hScMsg 1.0 (30.7.96)
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
 *   rx hScMsg <filename> <hsc-options>
 *
 *   This script passes all options to hsc, and sets MSGFORMAT and
 *   MSGFILE according to it's needs. Therefore, avoid these two
 *   options when invoking hScMsg.
 *
 *   Note that you MUST pass the filename as first arg and that it
 *   has to show up again in <hsc-options>
 *
 * EXAMPLE
 *   rx hScMsg hugo.hsc FROM hugo.hsc TO hugo.html COMPACT RPLCENT
 *             ^^^^^^^^      ^^^^^^^^
 * BUGS
 *   o filenames containig blanks won't work
 *   o ScMsg can't handle all message-classes of hsc, therefor only
 *     "Warning" and "Error" are used for display
 */

ScMsgPath = "sc:c/ScMsg"
hscPath   = "hsc"
TmpFile = 'T:hScMsg' || PRAGMA('ID') || '.tmp'

Options Failat 21

Parse ARG Filename hscOpts

/* display help and exit */
IF ((Filename="") | (Filename='?')) THEN DO
    SAY 'Usage: hScMsg <filename> <hsc-options>'
    SAY '       (see source-code for details)'
    Exit
END

/* invoke hsc */
CmdLine = hscPath 'MSGFILE=' || TmpFile 'MSGFORMAT="%f:::%y:::%c:::%i:::%m"' hscOpts
Address Command CmdLine

/* invoke ScMsg, if it's not already there */
IF ~Show('ports', 'SC_SCMSG') THEN DO
    Address Command 'Run <>nil:' SCMSGPATH 'HIDDEN'
    Address Command 'WaitForPort SC_SCMSG'
END

/* check, if ScMsg showed up */
IF ~Show('ports', 'SC_SCMSG') THEN DO
    Say "Couldn't start ScMsg, giving up."
    Exit 10
END

/* remove old messages in ScMsg-window */
Address 'SC_SCMSG' DelFile FileName

/* store return-code of hsc */
hScMsgRC = RC

/* process message file, if exists */
IF Open('File', TmpFile, 'R') THEN DO

    DO UNTIL Eof('File')
        Line = ReadLn('File')

        /* extract information from current message-line */
        Parse VAR Line File ':::' Line ':::' MsgClass ':::' MsgNum ':::' Msg
        IF ((MsgClass='Error') | (MsgClass='Fatal Error')) THEN
            MsgClass='Error'
        ELSE
            MsgClass='Warning'

        /* Send message to ScMsg */
        Address  'SC_SCMSG' NewMsg '"'||FileName||'"' '"'||FileName||'"' Line '0 "" 0' MsgClass MsgNum Msg
    END
    Address 'SC_SCMSG' Show Activate

    /* cleanup */
    Call Close('File')
    Address Command 'Delete quiet' TmpFile
END

/* exit with returncode of hsc */
Exit hScMsgRC
