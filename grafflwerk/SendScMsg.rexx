/*
 * SendScMsg.rexx - send hsc-messages to ScMsg
 *
 * $VER: SendScMsg.rexx 1.0 (4.8.1996)
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
 *   rx hScMsg <hsc-source-filename> <hsc-message-filename>
 *
 *   When invoking hsc before calling this script, you need to
 *   set the MSGFILE-option to use the same messagefile as
 *   passed to this script. Additionally, you need to set the
 *   option MSGFORMAT="%f:::%y:::%c:::%i:::%m", which is the 
 *   expected format for this script.
 *
 * EXAMPLE
 *   hsc FROM hugo.hsc TO hugo.html MSGFORMAT="%f:::%y:::%c:::%i:::%m" MSGFILE=t:hscmsg.tmp
 *   rx SendScMsg.rexx hugo.hsc t:hscmsg.tmp
 *
 * BUGS
 *   
 *   o There is no way for this script to check if you've set all
 *     options correctly when invoking hsc.
 *   o concurrecy problems will occure when you invoke multiple
 *     instancies of hsc ouputing to the same message-file
 *   o filenames containig blanks won't work
 *   o ScMsg can't handle all message-classes of hsc, therefor only
 *     "Warning" and "Error" are used for display
 */

ScMsgPath = "sc:c/ScMsg"

Options Failat 21

Parse ARG SourceFilename MessageFilename

/* display help and exit */
IF ((SourceFilename="") | (SourceFilename='?') | (MessageFilename="")) THEN DO
    SAY 'Usage: hScMsg <hsc-source-filename> <hsc-message-filename>'
    SAY '       (see source-code for details)'
    Exit
END

/* strip leading spaces from MessageFilename */
MessageFilename = Strip(MessageFilename, 'L')

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
Address 'SC_SCMSG' DelFile SourceFilename

/* process message file, if exists */
IF Open('File', MessageFilename, 'R') THEN DO

    DO UNTIL Eof('File')
        Line = ReadLn('File')

        /* extract information from current message-line */
        Parse VAR Line File ':::' Line ':::' MsgClass ':::' MsgNum ':::' Msg
        IF ((MsgClass='Error') | (MsgClass='Fatal Error')) THEN
            MsgClass='Error'
        ELSE
            MsgClass='Warning'

        /* Send message to ScMsg */
        Address  'SC_SCMSG' NewMsg '"'||SourceFilename||'"' '"'||SourceFilename||'"' Line '0 "" 0' MsgClass MsgNum Msg
    END
    Address 'SC_SCMSG' Show Activate

    /* cleanup */
    Call Close('File')
END
