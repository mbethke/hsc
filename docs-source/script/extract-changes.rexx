/*
 * extract-changes.rexx
 *
 * Extract the descriptions of the differences from the ASCII-files
 * CHANGES and NEWS.
 *
 * Usage: rx extract-changes.rexx <file>
 *
 * Note: If the variable includeVersions contains a value greater
 *       than 1, for every version a heading will be included.
 */
PARSE ARG filename

/* Number of recent versions to include */
includeVersions = 1

/* Constants for state variable */
st_skip    = 0 /* still skipping intro */
st_ruler   = 1 /* after horizontal ruler */
st_heading = 2 /* in "Version ..." heading */
st_text    = 3 /* normal text */

/* State variable */
state = state_skip

/* Counter how many separators (lines starting with "---..")
 * have already occurred. */
sepCounter = 0

IF (Open('file', filename, 'R')) THEN DO
    DO WHILE ((sepCounter <= includeVersions) & (~EOF('file')))
        /* Read next line and check if it is a separator line */
        line = ReadLn('file')
        is_separator = (LEFT(line,10) = '----------')

        IF (is_separator) THEN DO
            /* Translate the clumsy hyphen-sequence into a more
             * elegant horizontal ruler */
            sepCounter = sepCounter + 1
            IF (sepCounter > 1) THEN DO
                SAY '</PRE>'
            END
            state = st_heading
        END
        ELSE DO
            IF (state ~= state_skip) THEN DO
                /* kipped the intro text */
                IF (sepCounter > 0) THEN DO
                    line = ReplaceAllBy(line, '&', '&amp;')
                    line = ReplaceAllBy(line, '<', '&lt;')
                    line = ReplaceAllBy(line, '>', '&gt;')
                    IF (state = st_heading) THEN DO
                        IF (line ~= '') THEN DO
                            /* Insert version heading */
                            IF (includeVersions > 1) THEN DO
                               SAY '<H3>' || line || '</H3>'
                            END

                            /* Start preformatted context */
                            SAY '<PRE>'

                            /* Skip until next empty line */
                            DO WHILE (line ~= '')
                                line = ReadLn('file')
                            END
                            state = st_text
                        END
                    END
                    ELSE DO
                        SAY line
                    END
                END
            END
        END 
    END /* while */
    Call CLOSE('file')
END
ELSE DO /* Open() */
    /* Error message */
    SAY 'error opening "' || filename || '" for input'
END

EXIT 0

ReplaceAllBy: PROCEDURE
    PARSE ARG string, all, by
    result = ''
    DO UNTIL (i=0)
        i = POS(all, string)
        IF (i > 0) THEN DO
            result = result || LEFT(string, i-1) || by
            string = DELSTR(string, 1, i)
        END
        ELSE DO
            result = result || string
        END
    END /* until */

    RETURN result

