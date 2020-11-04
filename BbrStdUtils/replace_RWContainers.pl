#!/usr/bin/perl
# David Norvil Brown, University of Louisville, Feb. 26, 2002.
# Script to migrate from RW Container to std containers in 
# BaBar .hh and .cc files.
# This script does NOT modify any METHODS!!!
##
##
## TO USE THIS SCRIPT, cd INTO YOUR PACKAGE DIRECTORY AND ISSUE THE 
## NAME OF THE SCRIPT with either header or class name as argument:
## replace_RWContainers.pl  tpslist
##            OR
## replace_RWContainers.pl  RWTPtrSlist
##
##        Or you can do 
## replace_RWContainers.pl help
##        for more info.

# 
# Initializations
#

$numedited = 0;
$OldClass = "undefined";
$OldHeader = "undefined";
$NewClass = "undefined";
$OldIterator = "undefined";
$isPointerClass = "undefined";

if ( $#ARGV != 0 ) { die "Wrong number of arguments.  Please indicate a 
RW header or class name to replace.\n";}

$selection = $ARGV[0];
chomp ($selection);

&findClass($selection);

if ( $selection eq "help" || $selection eq "-h" ) { die;}
if ( $OldClass =~ "undefined" ) { die "Could not understand name: $selection\n"; }

#
#  Loop through all the files in current directory.  For those ending
#  in .hh or .cc, call subroutine to replace RW items with std items.
#

my $dummy = `ls | cat >  replace_list_tmpTMPtmp.tmp`;
open ( LISTFILE, "<replace_list_tmpTMPtmp.tmp" );

my $line;
my $numvisited = 0;
while ( $line = <LISTFILE> ) {
    chomp($line);
#    print "main sees filename as: $line\n";
    if ( $line =~ /.hh$/ || $line =~ /.cc$/ ) { 
	&replaceClass($line);
	$numvisited++;
    }
}

#
# End of loop over files.  Now cleanup and report.
#

close (LISTFILE);
unlink ("replace_list_tmpTMPtmp.tmp");

print "replace_RWContainers.pl visited $numvisited files.\n";
print "replace_RWContainers.pl edited $numedited files.\n";
print "No methods replaced.  If you used any of the methods of RW Containers,\n";
print "you should edit by hand to ensure proper migration.\n";
exit;

##
##
## Subroutine to do the actual checks and replacements.
##
##

sub replaceClass
{

my ($filenm) = @_;
#print "subroutine sees filename as:  $filenm\n";

$checkfirst = `grep $NewClass $filenm`;

$isThere = 0;
if ($checkfirst =~ /#include \<$NewClass\>/s) { $isThere = 1;}

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

while ( $line = <INFIL> ) {

    if ( $line =~ "rw/$OldHeader.h" && $line =~ "#include" ) {
	if ($isThere == 0) {
	    print OUTFIL "#include <$NewClass>\n";
	    if ($is_cc) { print OUTFIL "using std::$NewClass;\n"; }
	}
	$countreplacements++;
    } elsif ( $is_hh && $line =~ "$OldClass" && $line =~ "class" ) {
	if ($isThere == 0) {
	    print OUTFIL "#include <$NewClass>\n";
	}
	$countreplacements++;
    } elsif ( $line =~ "$OldClass" ) {
	$templine = $line;
	$buildline = '';

	if ( $templine =~ "$OldIterator" ) {
	    if ( $isPointerClass eq "yes" ) {
		$templine = starArgumentIn($OldIterator,$templine); 
	    }
	    $templine = replaceIteratorIn($OldIterator,$NewClass,$templine);
	    $countreplacements++;
	    print OUTFIL "$templine";
	    }

	if ( $templine =~ "$OldClass" ) {
	    if ( $isPointerClass eq "yes" ) {
		$templine = starArgumentIn($OldClass,$templine);
	    }
	    $templine = replaceNameIn($OldClass, $NewClass, $templine);
	    print OUTFIL "$templine";
	    $countreplacements++;
	}

    } else {
	print OUTFIL "$line";
    }
}

print "$filenm:  $countreplacements lines replaced.\n";
if ( $countreplacements == 1 ) {
    print "\tNOTE:\tThis file had 1 replacement.  This could mean\n";
    print "\t\tthat you have an unneeded include or that you\n";
    print "\t\tare trying to use lists without forward-declaration.\n\n";
} else {
    print "\n";
}

if ( $countreplacements > 0 ) {$numedited++;}

close (INFIL);
close (OUTFIL);
unlink ("$filenm");
$dummyvar = `mv $outfilnm $filenm`;

}


sub starArgumentIn {
    my ( $aClass, $aLine ) = @_;

    my $someline = '';

    while ( $aLine =~ /(.*)($aClass)(.*)/s ) {
	$myFirst = $1;
	$myMiddle = $2;
	$myEnd = $3;

	# myEnd will contain the argument to star.

	$myEnd =~ /(.*?)(\>)(.*)/s;
	$myEndFirst = $1;
	$myEndMiddle = $2;
	$myEndEnd = $3;

	$someline = $myMiddle.$myEndFirst."*".$myEndMiddle.$myEndEnd.$someline;

	$aLine = $myFirst;

	if ( ! ($myFirst =~ /$aClass/) ) {
	    $someline = $myFirst.$someline;
	}
    }

    return $someline;
}
    
sub replaceIteratorIn {

    my ( $aClass, $aNewClass, $aLine ) = @_;

    # Assume only one per line(?)

    $aLine =~ /(.*)($aClass)(.*)/s;
    $myFirst = $1;
    $myMiddle = $2;
    $myEnd = $3;

    $myEnd =~ /(.*?)(\>)(.*)/s;
    
    $newLine = $myFirst.$aNewClass.$1.$2."::iterator".$3;

    return $newLine;
}
    
sub replaceNameIn {
    
    my ( $aClass, $aNewClass, $aLine ) = @_;

    $someline = '';

    while ( $aLine =~ /(.*)($aClass)(.*)/s ) {
	$myFirst = $1;
	$myMiddle = $2;
	$myEnd = $3;
	
	$someline = $aNewClass.$myEnd.$someline;

	$aLine = $myFirst;
	if (! ( $aLine =~ /$aClass/s) ) {
	    $someline = $myFirst.$someline;
	}

    }
	
    return $someline;
}    

##
## OK, the following is ugly.  But so am I, and lazy, too!  DNB
##

sub findClass {

    my ($clSelect) = @_;

    if ( $clSelect =~ "tpslist" || $clSelect =~ "RWTPtrSlist" ) {
	$OldClass = "RWTPtrSlist";
	$OldHeader = "tpslist";
	$NewClass = "list";
	$OldIterator = "RWTPtrSlistIterator";
	$isPointerClass = "yes";
    }

    if ( $clSelect =~ "tvslist" || $clSelect =~ "RWTValSlist" ) {
	$OldClass = "RWTValSlist";
	$OldHeader = "tvslist";
	$NewClass = "list";
	$OldIterator = "RWTValSlistIterator";
	$isPointerClass = "no";
    }

    if ( $clSelect =~ "tpordvec" || $clSelect =~ "RWTPtrOrderedVector" ) {
	$OldClass = "RWTPtrOrderedVector";
	$OldHeader = "tpordvec";
	$NewClass = "vector";
	$OldIterator = "RWTPtrOrderedVectorIterator";
	$isPointerClass = "yes";
    }

    if ( $clSelect =~ "tvordvec" || $clSelect =~ "RWTValOrderedVector" ) {
	$OldClass = "RWTValOrderedVector";
	$OldHeader = "tvordvec";
	$NewClass = "vector";
	$OldIterator = "RWTValOrderedVectorIterator";
	$isPointerClass = "no";
    }

    if ( $clSelect =~ "rwset" || $clSelect =~ "RWSet" ) {
	$OldClass = "RWSet";
	$OldHeader = "rwset";
	$NewClass = "set";
	$OldIterator = "RWSetIterator";
	$isPointerClass = "no";
    }

    if ( $clSelect =~ "tpsrtvec" || $clSelect =~ "RWTPtrSortedVector" ) {
	$OldClass = "RWTPtrSortedVector";
	$OldHeader = "tpsrtvec";
	$NewClass = "vector";
	$OldIterator = "RWTPtrSortedVectorIterator";
	$isPointerClass = "yes";
    }

    if ( $clSelect =~ "tpvector" || $clSelect =~ "RWTPtrVector" ) {
	$OldClass = "RWTPtrVector";
	$OldHeader = "tpvector";
	$NewClass = "vector";
	$OldIterator = "RWTPtrVectorIterator";
	$isPointerClass = "yes";
    }
    

    if ( $clSelect =~ "tvsrtvec" || $clSelect =~ "RWTValSortedVector" ) {
	$OldClass = "RWTValSortedVector";
	$OldHeader = "tvsrtvec";
	$NewClass = "vector";
	$OldIterator = "RWTValSortedVectorIterator";
	$isPointerClass = "no";
    }

    if ( $clSelect =~ "tvvector" || $clSelect =~ "RWTValVector" ) {
	$OldClass = "RWTValVector";
	$OldHeader = "tvvector";
	$NewClass = "vector";
	$OldIterator = "RWTValVectorIterator";
	$isPointerClass = "no";
    }
    
    if ( $clSelect =~ "sortvec" || $clSelect =~ "RWSortedVector" ) {
	$OldClass = "RWSortedVector";
	$OldHeader = "sortvec";
	$NewClass = "vector";
	$OldIterator = "RWSortedVectorIterator";
	$isPointerClass = "no";
    }
    
    if ( $clSelect =~ "-h" || $clSelect =~ "help" ) {
	print "  replace_RWContainers.pl:  a script to help with RogueWave migration.\n";
	print "  Use:  replace_RWContainers.pl  name \n";
	print "\n where name can be a class name or header name.  For example:\n";
	print "\t\treplace_RWContainers.pl  tpslist\n";
	print "OR\t\treplace_RWContainers.pl  tpslist.h\n";
	print "OR\t\treplace_RWContainers.pl  RWTPtrSlist\n";
	print "\nAvailable names:\n\t\trwset\t\tRWSet\n\t\tsortvec\t\tRWSortedVector\n\t\ttpordvec\t\tRWTPtrOrderedVector\n\t\ttpslist\t\tRWTPtrSlist\n\t\ttpsrtvec\t\tRWTPtrSortedVector\n\t\ttpvector\t\tRWTPtrVector\n\t\ttvordvec\t\tRWTValOrderedVector\n\t\ttvslist\t\tRWTValSlist\n\t\ttvsrtvec\t\tRWTValSortedVector\n\t\ttvvector\t\tRWTValVector\n";


    }
    
  
}

