/*
 * ugly/returncode.h
 *
 * defines for program return codes
 *
 */

#ifndef UGLY_RETURNCODE_H       /* avoid include twice */
#define UGLY_RETURNCODE_H

/*
 * define values for return code
 */
#ifdef AMIGA

#define RC_OK     0
#define RC_WARN   5
#define RC_ERROR 10
#define RC_FAIL  20

#elif (defined UNIX) || (defined MSDOS)

#define RC_OK     0
#define RC_WARN   0
#define RC_ERROR  1
#define RC_FAIL   2

#else
#error "system not supported: return codes"

#endif /* return code */

#endif /* UGLY_RETURNCODE_H */

