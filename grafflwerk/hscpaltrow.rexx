/*
 * hscpaltrow.rexx - access project data using hscpitt
 *
 * $VER: hscpaltrow 1.0 (7.4.97)
 *
 * Copyright 1997 Thomas Aglassinger <agi@giga.or.at>
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
 *   rx hscpaltrow <project-file>
 *
 * EXAMPLE
 *   rx hscpaltrow hsc.project
 *
 * For more details, please refer to the documentation of hsc, section
 * `project management'
 */

/*
 * some values you might want to change
 */

/* command to invoke hscpitt */
pittCmd = 'hscpitt'

/* temporary file output is redirected to */
tmpFile = 't:paltrow.' || PRAGMA('ID')

/* command to delete a file */
deleteCmd = 'delete QUIET'

/*
 * debugging support:
 *
 * `debug=1' will enable some SAY commands which tell you
 * what hscpaltrow currently is doing. `dbg' contains a
 * short text which will be used as a prefix by all these
 * SAYs so it is easier to separate from trace output
 */
debug=0
dbg  =':::'

/*
 * main program:
 *
 * when changing anything below, you should know, what are
 * you doing
 */

/* reset return code */
paltrowRC = 0

/* parse user args, display help if necessary */
Parse ARG projectFile

IF (projectFile="") THEN DO
    /* show help end exit */
    SAY 'Usage: hscpaltrow <project-file>'
    Exit
END


/*
 * invoke hscpitt, redirect output to a temporary file
 */
pittCmd = pittCmd 'COMMAND=EXTRACT QUIET PRJFILE=' || projectFile
pittCmd = pittCmd '>' || tmpFile
IF (debug) THEN DO
    SAY dbg 'invoking:' pittCmd
END
Address Command pittCmd
paltrowRC = RC;

/*
 * read the temporary file and parse it line-by-line
 *
 * (only of hscpitt returned sucessfull and the temporary
 * file could be found)
 */
IF (paltrowRC = 0) THEN DO
    IF (Open('File', tmpFile, 'R')) THEN DO

        DO UNTIL Eof('File')

            /* reset variables which will hold the information
             * about a document */
            docName = ""
            docSource = ""
            docIncludes = ""

            /* process all lines belonging to a specific document */
            DO UNTIL (Line="")

                /* read next line */
                Line = ReadLn('File')
                IF (debug) THEN DO
                    SAY dbg 'line:' Line
                END

                IF (Line ~= "") THEN DO

                    /* extract information from current line:
                     * every line corresponds to the template
                     * <id>="<value>", so everything until
                     * the first "=" is treated as ID, and
                     * the remaining data contain the value,
                     * from which the enclosing quotes are
                     * removed. Usually there should be no need
                     * to modify these two lines */
                    Parse VAR Line LineID '="' LineValue
                    LineValue = LEFT(LineValue, LENGTH(LineValue) - 1)

                    /* now act differetly on LineID; you can
                     * add further WHENs here */
                    SELECT
                        WHEN (LineID = 'DOCUMENT') THEN DO
                            docName = LineValue;
                        END
                        WHEN (LineID = 'SOURCE') THEN DO
                            docSource = LineValue;
                        END
                        WHEN (LineID = 'INCLUDE') THEN DO
                            docIncludes = docIncludes LineValue;
                        END
                        OTHERWISE DO
                            IF (debug) THEN
                                SAY dbg 'ignore:' LineID '=' LineValue
                        END
                    END /* select */

                END /* if line */
            END /* until line */

            /* now all information about a specific document
             * has been extracted, and you can do about it
             * whatever you want. This example simply displays
             * it */
            SAY 'Document:' docName
            SAY 'Source  :' docSource
            SAY 'Includes:' || docIncludes
            SAY ''

        END /* until eof */

        /* close input file */
        Call Close('File')

    END /* if open */
    ELSE DO
        SAY 'error opening temporary file "' || tmpFile || '" for input'
        paltrowRC = 10
    END

    /* remove temporary file */
    Address Command deleteCmd tmpFile

END /* if paltrowRC */
ELSE DO
    /* this part of the code is reached if hscpitt failed */
END

Exit paltrowRC

