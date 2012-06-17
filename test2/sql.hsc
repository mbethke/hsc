<$include file=macros.hsc>

<$macro ADDRESS_QUERY COLUMNS:string COND:string FORMATTER:string/R SPCATTR:bool>
<SQL_QUERY DBMS=mysql COND?=COND FORMATTER?=FORMATTER SPCATTR?=SPCATTR
           COLUMNS?=COLUMNS TABLE=addresses DATABASE=test>
</$macro>

<$macro SQL_QUERY COLUMNS:string='*' TABLE:string/R DATABASE:string/R
                  COND:string='' FORMATTER:string/R NULL:string='&nbsp;'
                  DBMS:string/R USER:string='' PASSWORD:string=''
                  HOST:string='localhost' SPCATTR:bool>
<$define QUERY:string/C=("SELECT " + COLUMNS + " FROM " + TABLE + " " +
                         COND + ";")>
<PERL ARGS=("'" + DBMS + "' '" + HOST + "' '" + USER + "' '" + PASSWORD +
            "' '" + QUERY + "' '" + DATABASE + "' '" + FORMATTER + "' '" +
            NULL + "' " + SPCATTR)>
use DBI;
my ($dbms,$host,$user,$pw,$query,$db,$fmt,$null,$spcattr) = @ARGV;
my $dbh = DBI->connect("DBI:$dbms:database=$db;host=$host",$user,$pw) || 
  die "Error connecting to $dbms database \"$db\" on $host";
my $sth = $dbh->prepare($query) || die "Error preparing query: $dbh->err";
$sth->execute;
defined($dbh->err) && die "Error reading from database $db: $dbh->err";
$null = quotestring($null);
my $i=0;
while(my $res = $sth->fetchrow_hashref) {
  ++$i;
  print "<$fmt";
  print " _ROW_='$i'" if($spcattr);
  foreach(keys %$res) {
    print " $_=" . (defined($res->{$_}) ? quotestring($res->{$_}) : $null);
  }
  print ">";
}
$dbh->disconnect;

sub quotestring {
  my $s = shift;
  my $sq = (-1 != substr($s,'\''));
  if(-1 != substr($s,'"') and $sq) {
    my %t=('"'=>'\'','\''=>'"');
    $s =~ s/("|')/'+$t{$1}$1$t{$1}+'/g;
    return "('$s')";
  }
  $sq && return "\"$s\"";
  return "'$s'";
}
</PERL>
</$macro>

<$macro _ADDRESS-TABLE-FORMATTER FIRST:string LAST:string STREET:string CITY:string
        ZIP:string COUNTRY:string PHONE:string _ROW_:string>
<TR>
<TH STYLE="background-color:#aaaa00"><(_ROW_)></TH>
<TD><(first)></TD>
<TD><(last)></TD>
<TD><(street)></TD>
<TD><(zip)></TD>
<TD><(city)></TD>
<TD><(country)></TD>
<TD><(phone)></TD>
</TR>
</$macro>

<$macro _ADDRESS-LIST-FORMATTER FIRST:string LAST:string STREET:string CITY:string
        COUNTRY:string>
<LI><UL>                           
<LI>Name: <(last)>, <(first)></LI>
<LI>Street: <(street)></LI>
<LI>City: <(city)></LI>
<LI>Country: <(country)></LI>
</UL></LI>
</$macro>

<* Webpage proper starts here *>
<HTML>
<HEAD><TITLE>HSC and SQL Databases</TITLE></HEAD>
<BODY>

<* A tabular layout for the addresses *>
<TABLE BORDER="1">
<ADDRESS_QUERY FORMATTER="_ADDRESS-TABLE-FORMATTER" SPCATTR>
</TABLE>

<* Something similar with nested lists *>
<OL>
<ADDRESS_QUERY COLUMNS="first,last,street,city,country"
  FORMATTER="_ADDRESS-LIST-FORMATTER">
</OL>

</BODY>
</HTML>

<* vi: set et ts=2: *>
