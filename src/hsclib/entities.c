/* Entities to be defined during startup -- replaces $defent tags in hsc.prefs */

static struct HSCENTITY {
   STRPTR name;
   unsigned short numeric;
   BOOL prefnum;
} HSCInternalEntities[] = {
   /* basic entities w/o replacement chars */
   {"amp",     0, FALSE},  /* & */
   {"lt",      0, FALSE},  /* < */
   {"gt",      0, FALSE},  /* > */
   {"quot",    0, FALSE},  /* " */
   /* ISO-8859-1 entities stay in the prefs file to allow users to utilize
    * different 8-bit charsets! */

   /* UNICODE entities that don't have a corresponding character
    * in an 8-bit charset */
   {"ensp",		8194, FALSE}, 	/* en space (1/2-em) */
   {"emsp",		8195, FALSE}, 	/* em space */
   {"emsp13",	8196, FALSE}, 	/* 1/3-em space */
   {"emsp14",	8197, FALSE}, 	/* 1/4-em space */
   {"numsp",	8199, FALSE}, 	/* digit space (width of a number) */
   {"puncsp",	8200, FALSE}, 	/* punctuation space (width of comma) */
   {"thinsp",	8201, FALSE}, 	/* thin space (1/6-em) */
   {"hairsp",	8202, FALSE}, 	/* hair space */
   {"dash",		8208, FALSE}, 	/* hyphen (true graphic) */
   {"ndash",	8211, FALSE}, 	/* en dash */
   {"mdash",	8212, FALSE}, 	/* em dash */
   {"rsquor",	8217, FALSE}, 	/* rising single quote, right (high) */
   {"lsquor",	8218, FALSE}, 	/* rising single quote, left (low) */
   {"rdquor",	8221, FALSE}, 	/* rising dbl quote, right (high) */
   {"ldquor",	8222, FALSE}, 	/* rising dbl quote, left (low) */
   {"dagger",	8224, FALSE}, 	/* dagger */
   {"Dagger",	8225, FALSE}, 	/* double dagger */
   {"bull",		8226, FALSE}, 	/* round bullet, filled */
   {"nldr",		8229, FALSE}, 	/* double baseline dot (en leader) */
   {"hellip",	8230, FALSE}, 	/* ellipsis (horizontal) */
   {"caret",	8257, FALSE}, 	/* caret (insertion mark) */
   {"hybull",	8259, FALSE}, 	/* rectangle, filled (hyphen bullet) */
   {"incare",	8453, FALSE}, 	/* in-care-of symbol */
   {"copysr",	8471, FALSE}, 	/* sound recording copyright sign */
   {"rx",		8478, FALSE}, 	/* pharmaceutical prescription (Rx) */
   {"frac13",	8531, FALSE}, 	/* fraction one-third */
   {"frac23",	8532, FALSE}, 	/* fraction two-thirds */
   {"frac15",	8533, FALSE}, 	/* fraction one-fifth */
   {"frac25",	8534, FALSE}, 	/* fraction two-fifths */
   {"frac35",	8535, FALSE}, 	/* fraction three-fifths */
   {"frac45",	8536, FALSE}, 	/* fraction four-fifths */
   {"frac16",	8537, FALSE}, 	/* fraction one-sixth */
   {"frac56",	8538, FALSE}, 	/* fraction five-sixths */
   {"vellip",	8942, FALSE}, 	/* vertical ellipsis */
   {"drcrop",	8972, FALSE}, 	/* downward right crop mark  */
   {"dlcrop",	8973, FALSE}, 	/* downward left crop mark  */
   {"urcrop",	8974, FALSE}, 	/* upward right crop mark  */
   {"ulcrop",	8975, FALSE}, 	/* upward left crop mark  */
   {"telrec",	8981, FALSE}, 	/* telephone recorder symbol */
   {"target",	8982, FALSE}, 	/* register mark or target */
   {"blank",	9251, FALSE}, 	/* significant blank symbol */
   {"uhblk",	9600, FALSE}, 	/* upper half block */
   {"lhblk",	9604, FALSE}, 	/* lower half block */
   {"block",	9608, FALSE}, 	/* full block */
   {"blk14",	9617, FALSE}, 	/* 25haded block */
   {"blk12",	9618, FALSE}, 	/* 50haded block */
   {"blk34",	9619, FALSE}, 	/* 75haded block */
   {"squ",		9633, FALSE}, 	/* square, open */
   {"squf",		9642, FALSE}, 	/* sq bullet, filled */
   {"rect",		9645, FALSE}, 	/* rectangle, open */
   {"marker",	9646, FALSE}, 	/* histogram marker */
   {"utrif",	9652, FALSE}, 	/* up tri, filled */
   {"utri",		9653, FALSE}, 	/* up triangle, open */
   {"rtrif",	9656, FALSE}, 	/* r tri, filled */
   {"rtri",		9657, FALSE}, 	/* /triangleright B: r triangle, open */
   {"dtrif",	9662, FALSE}, 	/* dn tri, filled */
   {"dtri",		9663, FALSE}, 	/* down triangle, open */
   {"ltrif",	9666, FALSE}, 	/* l tri, filled */
   {"ltri",		9667, FALSE}, 	/* /triangleleft B: l triangle, open */
   {"cir",		9675, FALSE}, 	/* circle, open */
   {"starf",	9733, FALSE}, 	/* /bigstar - star, filled */
   {"star",		9734, FALSE}, 	/* star, open */
   {"female",	9792, FALSE}, 	/* female symbol */
   {"male",		9794, FALSE}, 	/* male symbol */
   {"spades",	9824, FALSE}, 	/* spades suit symbol */
   {"hearts",	9825, FALSE}, 	/* heart suit symbol */
   {"diams",	9826, FALSE}, 	/* diamond suit symbol */
   {"clubs",	9827, FALSE}, 	/* club suit symbol */
   {"flat",		9837, FALSE}, 	/* musical flat */
   {"natur",	9838, FALSE}, 	/* /natural - music natural */
   {"sharp",	9839, FALSE}, 	/* musical sharp */
   {"phone",	9952, FALSE}, 	/* telephone symbol */
   {"check",	10003,FALSE},	/* tick, check mark */
   {"cross",	10007,FALSE},	/* ballot cross */
   {"malt",		10016,FALSE},	/* maltese cross */
   {"lozf",		10022,FALSE},	/* /blacklozenge - lozenge, filled */
   {"loz",		10023,FALSE},	/* /lozenge - lozenge or total mark */
   {"sext",		10038,FALSE},	/* sextile (6-pointed star) */
   {"fflig",	64256,FALSE},	/* small ff ligature */
   {"filig",	64257,FALSE},	/* small fi ligature */
   {"fllig",	64258,FALSE},	/* small fl ligature */
   {"ffilig",	64259,FALSE},	/* small ffi ligature */
   {"ffllig",	64260,FALSE},	/* small ffl ligature */
   {"fjlig",	65533,FALSE} 	/* small fj ligature */
};
