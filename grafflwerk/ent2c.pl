#!/usr/bin/perl
# Converts SGML/XML entity lists as found on
# http://www.w3.org/TR/xhtml1/DTD/xhtml-special.ent
# into HSC's C structures

while(<>) {
	chomp;
	$t .= $_;
}
$t =~ s/<!-- .*? -->//go;
$t =~ s/<!ENTITY\s+(\w+)\s+"&#(\d+);"\s*>/\t{"$1",\t$2, FALSE},\n/gs;
print $t;
