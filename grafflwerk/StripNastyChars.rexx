/*
 * StripNastyChars.rexx
 *
 * strip all characters except "a".."z", "0".."9" and "A".."Z" from
 * a text passed as argument and output the result to stdout.
 *
 * $VER: StripNastyChars 1.0 (12.10.97)
 */

/* get input from command line */
PARSE ARG text

/* specify nasty chars */
nastyChars = XRANGE(d2c(0), d2c(47)) || XRANGE(d2c(58), d2c(64)) || XRANGE(d2c(91), d2c(96)) || XRANGE(d2c(123), d2c(255))

/* now remove them from input data */
stripped = COMPRESS(text, nastyChars)

/* and display them at standard output
 * (which will be redirected an read by hsc) */
call WriteCH(stdout, stripped)

