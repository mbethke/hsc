#!/usr/bin/perl -n
# Converts strings from STDIN to correctly quoted HSC expressions on STDOUT
chomp;
%t=('"'=>'\'','\''=>'"');
s/(['"])/'+$t{$1}$1$t{$1}+'/g;
s/\+['"]{2}\+/+/g;
print "('$_')";

