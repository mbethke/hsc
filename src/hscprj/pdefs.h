/*
 * This source code is part of hsc, a html-preprocessor,
 * Copyright (C) 1995-1998  Thomas Aglassinger
 * Copyright (C) 2001 Matthias Bethke
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
 * hscprj/pdefs.h
 *
 * project-managment-defines for hsc
 *
 */

/*
 * project_file:
 *
 * line 1  : "HSC_PROJECT"                   to identify prokect file
 * line 2  : "VERSION x"                     file format version
 * comments: "# xxx"                         some comment lines
 *
 * fileinfo: "DOCUMENT"   len uri            document
 *                                           start document-info block
 *           "SOURCE"     len file           hsc-source used to create
 *                                           this document
 *           "TITLE"      len title          document title
 *           "ID"         len idname         ID defined in this doc.
 *           "INCLUDE     len file"          file included during
 *                                           processing this document
 */

/*
 * IDs in project file
 */
#define FILEID_HSCPRJ    "HSC_PROJECT"
#define VERSION_HSCPRJ   3

#define LINE_VERSION_STR  "VERSION"
#define LINE_REM_STR      "#"

#define LINE_DOCUMENT_STR  "DOCUMENT"
#define LINE_SOURCE_STR    "SOURCE"
#define LINE_TITLE_STR     "TITLE"
#define LINE_ID_STR        "ID"
#define LINE_INCLUDE_STR   "INCLUDE"
#define LINE_REFERENCE_STR "REFERENCE"

#define ID_CALLER_STR "FROM"

