#!/usr/bin/perl
# David Norvil Brown, University of Louisville, Feb. 19, 2002.
# Script to migrate from RWCString to std::string in BaBar .hh and .cc files.
# This script does NOT modify any METHODS!!!
##
##
## TO USE THIS SCRIPT, cd INTO YOUR PACKAGE DIRECTORY AND ISSUE THE 
## NAME OF THE SCRIPT:
## replace_RWCString.pl
##
##

$numedited = 0;

#
#  Loop through all the files in current directory.  For those ending
#  in .hh or .cc, call subroutine to replace RW items with std items.
#

my $dummy = `ls | cat >  replace_string_tmpTMPtmp.tmp`;
open ( LISTFILE, "<replace_string_tmpTMPtmp.tmp" );

my $line;
my $numvisited = 0;
while ( $line = <LISTFILE> ) {
    chomp($line);
#    print "main sees filename as: $line\n";
    if ( $line =~ /.hh$/ || $line =~ /.cc$/ ) { 
	&replaceString($line);
	$numvisited++;
    }
}

#
# End of loop over files.  Now cleanup and report.
#

close (LISTFILE);
unlink ("replace_string_tmpTMPtmp.tmp");

print "replace_RWCString.pl visited $numvisited files.\n";
print "replace_RWCString.pl edited $numedited files.\n";
print "No methods replaced.  If you used any of the methods of RWCString,\n";
print "you should edit by hand to ensure proper migration.\n";
exit;

##
##
## Subroutine to do the actual checks and replacements.
##
##

sub replaceString
{

my ($filenm) = @_;
#print "subroutine sees filename as:  $filenm\n";

#
# First figure out the type of file (.hh or .cc):
#

my $is_hh = 0;
my $is_cc = 0;

if ( $filenm =~ /.hh$/) { $is_hh = 1;}
if ( $filenm =~ /.cc$/) { $is_cc = 1;}

#
# The following two cases should never happen, but to be safe...
#

if ( $is_hh && $is_cc ) { die "$filenm:  This file seems to be some 
strange amalgamation of .hh and .cc - I QUIT!\n"; }

if ( !$is_hh && !$is_cc ) { die "$filenm:  This file seems to be 
neither .cc or .hh - I QUIT!\n";}

if ( $is_hh ) { print "\n$filenm:  HEADER FILE\n";}
if ( $is_cc ) { print "\n$filenm:  DEFINITIONS FILE\n";}

open ( INFIL, "<$filenm" );
my $outfilnm = "$filenm.temp";
#print "I will write to file $outfilnm\n";

open ( OUTFIL, ">$outfilnm" );

my $line = '';
my $countreplacements = 0;
my $templine = '';
my $buildline = '';

while ( $line = <INFIL> ) {

    if ( $line =~ "rw/cstring.h" && $line =~ "#include" ) {
	print OUTFIL "#include <string>\n";
	if ($is_cc) { print OUTFIL "using std::string;\n"; }
	$countreplacements++;
    } elsif ( $is_hh && $line =~ "RWCString" && $line =~ "class" ) {
	print OUTFIL "#include <string>\n";
	$countreplacements++;
    } elsif ( $line =~ "RWCString" ) {
	$templine = $line;
	$buildline = '';

	while ($templine =~ "RWCString" ) {
	    $templine =~ /(.*)(RWCString)(.*)/s;
	    my $first = $1;
	    my $middle = $2;
	    my $last = $3;

#	    print "first is $first\n";
#	    print "middle is $middle\n";
#	    print "last is $last\n";

	    $buildline = $last.$buildline;

	    if ( $middle =~ "RWCString") {
		if($is_hh) {$buildline = "std::string".$buildline;}
		if($is_cc) {$buildline = "string".$buildline;}
		$countreplacements++;
	    } else {
		$buildline = $middle.$buildline;
	    }

	    if (!($first =~ "RWCString")) {
		$buildline = $first.$buildline;
	    }

	    $templine = $first;
	}

#	print "$line ..produces.. \n$buildline";
	print OUTFIL "$buildline";
    } else {
	print OUTFIL "$line";
    }
}

print "$filenm:  $countreplacements replacements made.\n";
if ( $countreplacements == 1 ) {
    print "\tNOTE:\tThis file had 1 replacement.  This could mean\n";
    print "\t\tthat you have an unneeded include or that you\n";
    print "\t\tare trying to use strings without forward-declaration.\n\n";
} else {
    print "\n";
}

if ( $countreplacements > 0 ) {$numedited++;}

close (INFIL);
close (OUTFIL);
unlink ("$filenm");
$dummyvar = `mv $outfilnm $filenm`;

}

