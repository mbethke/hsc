<$include file="macros.hsc">

<$define PROJECT_SYMLINK_DB:string="symlinks.db">

<$macro SYMLINK /CLOSE TGT:string/R>
<$define link:string>
<PERL ARGS=(TGT + ' ' + PROJECT_SYMLINK_DB) TOVAR="link">
my ($file,$tgt,%db,$sym,$path,$subpath,$line);
$tgt = shift; $file = shift;
open(DB,$file) or die "Cannot open symlink database $file";
$line = 1;
while(<DB>) {
  chomp;
  ($sym,$path) = split /\s*=\s*/;
  while(($subpath) = $path =~ m/.*%(.+)%/) {
    die "Unknown symbolic path '$subpath' used on line $line"
      unless(defined $db{$subpath}); 
    $path =~ s/%$subpath%/$db{$subpath}/;
  }
  $db{$sym} = $path;
  last if($sym eq $tgt);
  $line++;
}
die "Symbolic path '$tgt' not defined in $file" unless(defined $db{$tgt});
print $db{$tgt};
</PERL>
<A HREF=(link)><$content></A>
</$macro>

<HTML>
<HEAD><TITLE>Symbolic Links</TITLE></HEAD>
<BODY>
<SYMLINK TGT="foo">link</SYMLINK>
<SYMLINK TGT="bar">link</SYMLINK>
<SYMLINK TGT="baz">link</SYMLINK>
</BODY>
</HTML>

<* vi: set ts=2 et: *>
