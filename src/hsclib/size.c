/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1997  Thomas Aglassinger
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
 * hsclib/size.c
 *
 * evaluate values for WIDTH and HEIGHT from file
 *
 * updated:  8-Nov-1996
 * created:  7-Jan-1996
 */

#include "hsclib/inc_base.h"

#include "hsclib/uri.h"

/* markers for JFIF/JPEG that contain
 * information about image dimension
 */
static UBYTE msof[] =
{
    /* M_SOF0  */ 0xc0,
    /* M_SOF1  */ 0xc1,
    /* M_SOF2  */ 0xc2,
    /* M_SOF3  */ 0xc3,
    /* M_SOF5  */ 0xc5,
    /* M_SOF6  */ 0xc6,
    /* M_SOF7  */ 0xc7,
    /* M_SOF9  */ 0xc9,
    /* M_SOF10 */ 0xca,
    /* M_SOF11 */ 0xcb,
    /* M_SOF13 */ 0xcd,
    /* M_SOF14 */ 0xce,
    /* M_SOF15 */ 0xcf,
    /* end     */ 0x00
};

/*
 * fucking macro to compare fucking unsigned strings without
 * fucking warnings that are only a result of the fucking
 * difference in the declaration of fucking `char' between
 * fucking C (by fucking K&R) and fucking C++ (by fucking
 * Stroustrupp-or-whatever-the-unspellable-name-of-this-guy-is)
 *
 * WHY DID I NEVER HAVE SUCH PROBLEMS IN BASIC,REXX,PASCAL
 * OR OBERON?
 *
 * Because the people who specified these languages had a brain.
 *
 * Where was I? Uh, back to the source code:
 */
#define fuck_strncmp(a,b,n) strncmp((char*)(a),(char*)(b),(n))

/*
 * hsc_msg_img_corrupt
 *
 * display message that image is corrupt
 */
static VOID hsc_msg_img_corrupt(HSCPRC * hp, STRPTR cause)
{
    hsc_message(hp, MSG_IMG_CORRUPT, "image corrupt (%s)");
}

/*
 * try_set_attr
 *
 * if attribute exists and it's value is empty, set
 * new value and update tag-attribute-string; otherwise
 * kust compare the old and new value and warn if they
 * differ
 */
static VOID try_setattr(HSCPRC * hp, HSCVAR * attr, ULONG value)
{
    if (attr)
    {
        STRPTR old_value = get_vartext(attr);
        STRPTR new_value = long2str(value);
        if (!old_value)
        {
            /* set new value */
            set_vartext(attr, new_value);

            /* append attribute name and "=" */
            app_estr(hp->tag_attr_str, " ");
            app_estr(hp->tag_attr_str, attr->name);
            app_estr(hp->tag_attr_str, "=");

            /* append quotes and value */
            if ((hp->quotemode == QMODE_KEEP) || (hp->quotemode == QMODE_DOUBLE))
                app_estrch(hp->tag_attr_str, '\"');
            else if (hp->quotemode == QMODE_SINGLE)
                app_estrch(hp->tag_attr_str, '\'');
            app_estr(hp->tag_attr_str, long2str(value));        /* append value */
            if ((hp->quotemode == QMODE_KEEP) || (hp->quotemode == QMODE_DOUBLE))
                app_estrch(hp->tag_attr_str, '\"');
            else if (hp->quotemode == QMODE_SINGLE)
                app_estrch(hp->tag_attr_str, '\'');

        }
        else
        {
            /* validate old value */
            if (strcmp(old_value, new_value))
            {
                hsc_message(hp, MSG_UNEX_ATTR_VALUE,
                            "unexpected value for %A: expected %q, found %q",
                            attr, new_value, old_value);
            }
        }
    }
}

/*
 * get_attr_size
 *
 * tries to get values for WIDTH and HEIGHT attributes
 * from file; if possible, the corresponding attributes
 * for the tag passed will be set (or validated).
 *
 * result: TRUE, if filetype has been recognised
 */
BOOL get_attr_size(HSCPRC * hp, HSCTAG * tag)
{
#define BUFSIZE  2048
#define WIDTH_PNG  16           /* file indeces for PNG */
#define HEIGHT_PNG 20

    HSCVAR *asrc = tag->uri_size;
    STRPTR srcuri = NULL;

    if (asrc)
    {
        srcuri = get_vartext(asrc);
    }
    else
    {
        panic("no uri_size");
    }

    if (hp->getsize && srcuri && (uri_kind(srcuri) != URI_ext))
    {
        unsigned char *buf = (unsigned char *) umalloc(BUFSIZE);
        EXPSTR *srcpath = init_estr(64);
        EXPSTR *imgpath = init_estr(64);
        ULONG width = 0;
        ULONG height = 0;
        BOOL transparent = FALSE;
        BOOL progressive = FALSE;
        STRPTR filetype = NULL;
        FILE *fref = NULL;      /* file link references to */
        unsigned char id_PNG[] =
        {
            137, 80, 78, 71, 13, 10, 26, 10
        };                      /* PNG image header */

        conv_hscuri2file(hp, srcpath, srcuri);

        DSZ(fprintf(stderr, DHL "   uri : \"%s\"\n" DHL "   path: \"%s\"\n",
                    srcuri, estr2str(srcpath)));

        fref = fopen(estr2str(srcpath), "r");

        if (fref)
        {
            /* fill buffer with zero */
            memset(buf, 0, BUFSIZE);

            /* read buffer  from file */
            fread(buf, BUFSIZE, 1, fref);

            if (buf[0] == 0xff)
            {
                /*
                 * JFIF/JPEG
                 */
                BOOL found = FALSE;
                size_t i = 0;

                /*TODO: recognize and report progressive */
                /*TODO: warning for progressive */
                while (!found && (i < BUFSIZE + 8))
                {
                    if (buf[i] == 0xff)
                    {
                        BOOL is_msof = FALSE;
                        int j = 0;

                        DSZ(fprintf(stderr,
                                    "%04lx: %02x %02x: (%02x%02x %02x%02x) ",
                                    (ULONG) i, buf[i], buf[i + 1],
                                    buf[i + 2], buf[i + 3],
                                    buf[i + 4], buf[i + 5]));

                        /* check if marker is of required type */
                        while (!is_msof && msof[j])
                            if (buf[i + 1] == msof[j])
                                is_msof = TRUE;
                            else
                                j++;

                        if (is_msof)
                        {
                            DSZ(
                                   {
                                   for (j = 0; j < 10; j++)
                                   {

                                   printf("\n  %-2d: $%02x %-3d",
                                          j, buf[i + j], buf[i + j]);
                                   if (buf[i + j] >= 32)
                                   printf(" '%c'", buf[i + j]);

                                   }
                                   }
                            );

                            filetype = "JFIF/JPEG";
                            width = buf[i + 8] + (buf[i + 7] << 8);
                            height = buf[i + 6] + (buf[i + 5] << 8);
                            found = TRUE;

                        }
                        else
                        {
                            DDA(printf("ignore\n"));
                        }
                    }

                    i++;
                }

                /* check if buffer exeeds */
                if (i >= (BUFSIZE + 8))
                {
                    hsc_msg_img_corrupt(hp, "image buffer exeeds");
                }
            }
            else if (!fuck_strncmp("GIF87a", (STRPTR) buf, 6)
                     || !fuck_strncmp("GIF89a", (STRPTR) buf, 6))
            {
                /*
                 * GIF
                 */
                LONG use_global_colormap = (buf[10] & 0x80) >> 7;
                LONG pixeldepth = (buf[10] & 0x07) + 1;
                LONG startimg =
                13 + use_global_colormap * 3 * (1 << pixeldepth);
                BOOL fucked_up = FALSE;

                DSZ(fprintf(stderr, DHL "  buf=%d: gcolmap=%ld, pxldep=%ld\n",
                            buf[10], use_global_colormap, pixeldepth));

                while (!fucked_up && (buf[startimg] != ','))
                {
                    DSZ(fprintf(stderr, DHL "  %04lx: id=%02x\n",
                                startimg, buf[startimg]));

                    if (buf[startimg] == '!')
                    {
                        UBYTE blksize = 0;

                        if (buf[startimg + 1] == 0xF9)
                        {
                            /* graphic control extensions */
                            /* check if transparent */
                            transparent = (buf[startimg + 3] & 0x01);
                            if (transparent)
                            {
                                DDA(fprintf(stderr, DHL "  (transparent)\n"));
                            }
                        }

                        /* skip all blocks */
                        startimg += 2;
                        do
                        {
                            blksize = buf[startimg];
                            DDA(printf("  skip block sized %d\n", blksize));
                            startimg += 1L + blksize;
                        }
                        while (!fucked_up && (blksize));

                        /* check if buffer exeeds */
                        if (startimg > (BUFSIZE + 9))
                        {
                            hsc_msg_img_corrupt(hp, "image buffer exeeds");
                            fucked_up = TRUE;
                        }

                    }
                    else
                    {
                        hsc_msg_img_corrupt(hp, "unknown gif-block");
                        DSZ(fprintf(stderr, "  id='%x', index=%ld/\n",
                                    buf[startimg], startimg));
                        fucked_up = TRUE;
                    }
                }

                if ((buf[startimg] != ',') && !fucked_up)
                {
                    DSZ(fprintf(stderr, DHL "  %04lx: id=%02x\n",
                                startimg, buf[startimg]));
                    hsc_msg_img_corrupt(hp, "image separator expected");
                }
                else
                {
                    /* been sucessful */
                    DSZ(fprintf(stderr, DHL "  %04lx: id=%02x\n",
                                startimg, buf[startimg]));

                    filetype = "GIF";
                    width = buf[startimg + 5] + 256 * buf[startimg + 6];
                    height = buf[startimg + 7] + 256 * buf[startimg + 8];
                    progressive = (0 != (buf[startimg + 9] & (1 << 6)));
                    DDA(fprintf(stderr,
                                DHL "  width : %lu\n"
                                DHL "  height: %lu\n",
                                width, height));
                }
            }
            else if (!fuck_strncmp(id_PNG, buf, 8))
            {
                /*
                 * PNG
                 */

                filetype = "PNG";
                width = 0x00800000 * buf[WIDTH_PNG] +
                    0x00010000 * buf[WIDTH_PNG + 1] +
                    0x00000100 * buf[WIDTH_PNG + 2] +
                    0x00000001 * buf[WIDTH_PNG + 3];
                height = 0x00800000 * buf[HEIGHT_PNG] +
                    0x00010000 * buf[HEIGHT_PNG + 1] +
                    0x00000100 * buf[HEIGHT_PNG + 2] +
                    0x00000001 * buf[HEIGHT_PNG + 3];

                progressive = buf[HEIGHT_PNG + 9];
                /*TODO: transparent */

#if DEBUG_SIZE
                /* disaply whole image buffer */
                if (hp->debug)
                {
                    int i;
                    for (i = 0; i < BUFSIZE; i++)
                    {

                        fprintf(stderr, "%-2d: $%02x %-3d", i, buf[i], buf[i]);
                        if (buf[i] >= 32)
                        {
                            fprintf(stderr, " '%c'\n", buf[i]);
                        }
                        else
                        {
                            fprintf(stderr, "\n");
                        }
                    }
                }
#endif
                DDA(fprintf(stderr, DHL "  width : %lu\nheight: %lu\n",
                            width, height));
            }
            else
            {
                /* unknown file type */
                hsc_message(hp, MSG_UNKN_FILETYPE,
                            "filetype of %q not recognised",
                            estr2str(srcpath));
            }

            DSZ(fprintf(stderr, DHL "  size: \"%s\" (%ldx%ld)\n",
                        filetype, width, height));

            fclose(fref);
        }
        else
        {
            DSZ(fprintf(stderr, DHL "  Can't open image `%s'\n",
                        estr2str(srcpath)));
            hsc_msg_nouri(hp, estr2str(srcpath), srcuri, "image-size");
        }

        /* set values */
        if (height && width)
        {
            HSCVAR *awidth = find_varname(tag->attr, "WIDTH");
            HSCVAR *aheight = find_varname(tag->attr, "HEIGHT");

            /* status message */
            app_estr(srcpath, ": ");
            app_estr(srcpath, filetype);
            app_estr(srcpath, ", ");
            app_estr(srcpath, long2str(width));
            app_estr(srcpath, "x");
            app_estr(srcpath, long2str(height));
            if (progressive)
                app_estr(srcpath, ", progressive");
            if (transparent)
                app_estr(srcpath, ", transparent");
            hsc_status_misc(hp, estr2str(srcpath));

            try_setattr(hp, awidth, width);
            try_setattr(hp, aheight, height);
        }

        /* free local resources */
        ufree(buf);
        del_estr(srcpath);
        del_estr(imgpath);
    }

    return (TRUE);
}
