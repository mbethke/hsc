/*
 * hScMsg - invoke hsc and send messages to ScMsg
 *
 * $VER: hScMsg 1.1 (14.9.98)
 *
 * Written 1996-1998 Thomas Aglassinger <agi@giga.or.at>. Public domain.
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
 *     "Note", "Warning" and "Error" are used for display
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
Address 'SC_SCMSG DelComp "' || FileName || '"'

/* store return-code of hsc */
hScMsgRC = RC

/* process message file, if exists */
IF Open('File', TmpFile, 'R') THEN DO

    message_counter = 0

    DO WHILE ~ Eof('File')
        Line = ReadLn('File')
        IF (Line ~= '') THEN DO
            message_counter = message_counter + 1

            /* extract information from current message-line */
            Parse VAR Line File ':::' Line ':::' MsgClass ':::' MsgNum ':::' Msg
            IF ((MsgClass='Error') | (MsgClass='Fatal Error')) THEN DO
                MsgClass='Error'
            END
            ELSE IF (MsgClass='Note') THEN DO
                /* Leave message class as 'Note' */
            END
            ELSE DO
                MsgClass='Warning'
            END

            /* Send message to ScMsg */
            Address  'SC_SCMSG' NewMsg '"'||FileName||'"' '"'||FileName||'"' Line '0 "" 0' MsgClass MsgNum Msg
        END
    END

    IF (message_counter > 0) THEN DO
        SAY 'message_counter =' message_counter
        Address 'SC_SCMSG' Show Activate
    END

    /* cleanup */
    Call Close('File')
    Address Command 'Delete quiet' TmpFile
END

/* exit with returncode of hsc */
Exit hScMsgRC
