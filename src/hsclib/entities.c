/* Entities to be defined during startup -- replaces $defent tags in hsc.prefs */

static struct HSCENTITY {
   STRPTR name;
   unsigned short numeric;
   unsigned char replace;
} HSCInternalEntities[] = {
   /* basic entities w/o replacement chars */
   {"amp",     0, '\0'},  /* & */
   {"lt",      0, '\0'},  /* < */
   {"gt",      0, '\0'},  /* > */
   {"quot",    0, '\0'},  /* " */
   /* ISO-8859-1 entities stay in the prefs file! */

   /* UNICODE entities that don't have a corresponding character in Latin-1 */
   {"ensp",		8194, 0}, 	/* en space (1/2-em) */
   {"emsp",		8195, 0}, 	/* em space */
   {"emsp13",	8196, 0}, 	/* 1/3-em space */
   {"emsp14",	8197, 0}, 	/* 1/4-em space */
   {"numsp",	8199, 0}, 	/* digit space (width of a number) */
   {"puncsp",	8200, 0}, 	/* punctuation space (width of comma) */
   {"thinsp",	8201, 0}, 	/* thin space (1/6-em) */
   {"hairsp",	8202, 0}, 	/* hair space */
   {"dash",		8208, 0}, 	/* hyphen (true graphic) */
   {"ndash",	8211, 0}, 	/* en dash */
   {"mdash",	8212, 0}, 	/* em dash */
   {"rsquor",	8217, 0}, 	/* rising single quote, right (high) */
   {"lsquor",	8218, 0}, 	/* rising single quote, left (low) */
   {"rdquor",	8221, 0}, 	/* rising dbl quote, right (high) */
   {"ldquor",	8222, 0}, 	/* rising dbl quote, left (low) */
   {"dagger",	8224, 0}, 	/* dagger */
   {"Dagger",	8225, 0}, 	/* double dagger */
   {"bull",		8226, 0}, 	/* round bullet, filled */
   {"nldr",		8229, 0}, 	/* double baseline dot (en leader) */
   {"hellip",	8230, 0}, 	/* ellipsis (horizontal) */
   {"caret",	8257, 0}, 	/* caret (insertion mark) */
   {"hybull",	8259, 0}, 	/* rectangle, filled (hyphen bullet) */
   {"incare",	8453, 0}, 	/* in-care-of symbol */
   {"copysr",	8471, 0}, 	/* sound recording copyright sign */
   {"rx",		8478, 0}, 	/* pharmaceutical prescription (Rx) */
   {"frac13",	8531, 0}, 	/* fraction one-third */
   {"frac23",	8532, 0}, 	/* fraction two-thirds */
   {"frac15",	8533, 0}, 	/* fraction one-fifth */
   {"frac25",	8534, 0}, 	/* fraction two-fifths */
   {"frac35",	8535, 0}, 	/* fraction three-fifths */
   {"frac45",	8536, 0}, 	/* fraction four-fifths */
   {"frac16",	8537, 0}, 	/* fraction one-sixth */
   {"frac56",	8538, 0}, 	/* fraction five-sixths */
   {"vellip",	8942, 0}, 	/* vertical ellipsis */
   {"drcrop",	8972, 0}, 	/* downward right crop mark  */
   {"dlcrop",	8973, 0}, 	/* downward left crop mark  */
   {"urcrop",	8974, 0}, 	/* upward right crop mark  */
   {"ulcrop",	8975, 0}, 	/* upward left crop mark  */
   {"telrec",	8981, 0}, 	/* telephone recorder symbol */
   {"target",	8982, 0}, 	/* register mark or target */
   {"blank",	9251, 0}, 	/* significant blank symbol */
   {"uhblk",	9600, 0}, 	/* upper half block */
   {"lhblk",	9604, 0}, 	/* lower half block */
   {"block",	9608, 0}, 	/* full block */
   {"blk14",	9617, 0}, 	/* 25haded block */
   {"blk12",	9618, 0}, 	/* 50haded block */
   {"blk34",	9619, 0}, 	/* 75haded block */
   {"squ",		9633, 0}, 	/* square, open */
   {"squf",		9642, 0}, 	/* sq bullet, filled */
   {"rect",		9645, 0}, 	/* rectangle, open */
   {"marker",	9646, 0}, 	/* histogram marker */
   {"utrif",	9652, 0}, 	/* up tri, filled */
   {"utri",		9653, 0}, 	/* up triangle, open */
   {"rtrif",	9656, 0}, 	/* r tri, filled */
   {"rtri",		9657, 0}, 	/* /triangleright B: r triangle, open */
   {"dtrif",	9662, 0}, 	/* dn tri, filled */
   {"dtri",		9663, 0}, 	/* down triangle, open */
   {"ltrif",	9666, 0}, 	/* l tri, filled */
   {"ltri",		9667, 0}, 	/* /triangleleft B: l triangle, open */
   {"cir",		9675, 0}, 	/* circle, open */
   {"starf",	9733, 0}, 	/* /bigstar - star, filled */
   {"star",		9734, 0}, 	/* star, open */
   {"female",	9792, 0}, 	/* female symbol */
   {"male",		9794, 0}, 	/* male symbol */
   {"spades",	9824, 0}, 	/* spades suit symbol */
   {"hearts",	9825, 0}, 	/* heart suit symbol */
   {"diams",	9826, 0}, 	/* diamond suit symbol */
   {"clubs",	9827, 0}, 	/* club suit symbol */
   {"flat",		9837, 0}, 	/* musical flat */
   {"natur",	9838, 0}, 	/* /natural - music natural */
   {"sharp",	9839, 0}, 	/* musical sharp */
   {"phone",	9952, 0}, 	/* telephone symbol */
   {"check",	10003,0},	/* tick, check mark */
   {"cross",	10007,0},	/* ballot cross */
   {"malt",		10016,0},	/* maltese cross */
   {"lozf",		10022,0},	/* /blacklozenge - lozenge, filled */
   {"loz",		10023,0},	/* /lozenge - lozenge or total mark */
   {"sext",		10038,0},	/* sextile (6-pointed star) */
   {"fflig",	64256,0},	/* small ff ligature */
   {"filig",	64257,0},	/* small fi ligature */
   {"fllig",	64258,0},	/* small fl ligature */
   {"ffilig",	64259,0},	/* small ffi ligature */
   {"ffllig",	64260,0},	/* small ffl ligature */
   {"fjlig",	65533,0} 	/* small fj ligature */
};
