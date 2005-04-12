/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1997  Sergio Monesi and Nick Craig-Wood
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
 * riscos/msgbrowser.c
 *
 * throwback support for RiscOS
 */

#ifndef RISCOS
/* Avoid that anything is attempted to be compiled on none-RiscOS systems.
 * A dummy symbol is declared to keep the linker from whining. */
char riscos_message_browser_dummy = 0;
#else

#include "hsc/global.h"

#include "riscos/unixname.h"
#include <kernel.h>

#undef  DDEUtils_ThrowbackStart
#define DDEUtils_ThrowbackStart         0x42587
#undef  XDDEUtils_ThrowbackStart
#define XDDEUtils_ThrowbackStart        0x62587
#undef  DDEUtils_ThrowbackSend
#define DDEUtils_ThrowbackSend          0x42588
#undef  XDDEUtils_ThrowbackSend
#define XDDEUtils_ThrowbackSend         0x62588
#undef  DDEUtils_ThrowbackEnd
#define DDEUtils_ThrowbackEnd           0x42589
#undef  XDDEUtils_ThrowbackEnd
#define XDDEUtils_ThrowbackEnd          0x62589

#define ddeutils_SEVERITY_WARNING       0
#define ddeutils_SEVERITY_ERROR         1
#define ddeutils_SEVERITY_SERIOUS_ERROR     2
#define ddeutils_THROWBACK_PROCESSING       0
#define ddeutils_THROWBACK_ERROR_DETAILS    1
#define ddeutils_THROWBACK_INFO_DETAILS     2

BOOL init_msg_browser(HSCPRC * hp, STRPTR filename)
{
    BOOL success = TRUE;

    if (!upstrcmp(msg_browser, "THROWBACK"))
    {
        _kernel_swi_regs r;

        _kernel_swi(XDDEUtils_ThrowbackStart, &r, &r);
    }

    return success;
}

void del_msg_browser(HSCPRC * hp)
{
    if (!upstrcmp(msg_browser, "THROWBACK"))
    {
        _kernel_swi_regs r;

        _kernel_swi(XDDEUtils_ThrowbackEnd, &r, &r);
    }
}

void send_msg_browser(HSCPRC * hp,
                     HSCMSG_CLASS msg_class, HSCMSG_ID msg_id,
                     STRPTR fname, ULONG x, ULONG y,
                     STRPTR msg_text)
{
    if (!upstrcmp(msg_browser, "THROWBACK"))
    {
        if (fname != NULL)
        {
            _kernel_swi_regs r;
            char *riscosname = unixname_to_riscos(fname);

            r.r[0] = ddeutils_THROWBACK_ERROR_DETAILS;
            r.r[2] = (int) riscosname;
            r.r[3] = (int) y;
            switch (msg_class)
            {
            case MSG_NOTE:
            case MSG_STYLE:
            case MSG_PORT:
            case MSG_WARN:
                r.r[4] = ddeutils_SEVERITY_WARNING;
                break;
            case MSG_ERROR:
                r.r[4] = ddeutils_SEVERITY_ERROR;
                break;
            case MSG_FATAL:
                r.r[4] = ddeutils_SEVERITY_SERIOUS_ERROR;
                break;
            default:
                r.r[4] = ddeutils_SEVERITY_WARNING;
                break;
            }
            r.r[5] = (int) msg_text;

            _kernel_swi(XDDEUtils_ThrowbackSend, &r, &r);

            free(riscosname);
        }
    }
}

#endif /* RISCOS */

