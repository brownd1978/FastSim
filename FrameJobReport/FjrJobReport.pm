#------------------------------------------------------------==OOO==
# BaBar bookkeeping software.
#
# $Id: FjrJobReport.pm,v 1.1 2004/01/28 18:01:17 douglas Exp $
#
#     FjrJobReport.pm: Read names and values from a FrameWork job
#                     report file, and provide accessors to their 
#                     values for perl scripting.
#
# REVISION HISTORY
# Created.                           2004/01/26  douglas
#-------------------------------------------------------------------
#
package FrameJobReport::FjrJobReport;

#  Standard pragmas
use FindBin;
use lib $FindBin::Bin;

use strict;
use vars '$AUTOLOAD';

#-----[Begin Class Implementation]------------------

#---------------------------------------------------
# SIG: new( ) - RET: ref. to object
# DES: The class constructor. 
sub new {
    my( $classname ) = @_;
	#setup the default parameters for the object, accept some values
	#from the constructor call, and setup some default values
	bless { 
		filename => ($_[1]),
		debug => ($_[2] || 1),
		report => readFile($classname,$_[1])
	}, $classname;
}

#-=-=-=-=-=-=-=-=[BEGIN METHODS]=-=-=-=-=-=-=-=-#

#---------------------------------------------------
# SIG: readFile( ) - RET: sucess or failure boolean
# DES: Reads the FrameWork job report file and loads the
# values hash
sub readFile {
	my( $self, $fileName ) = @_;
	
	my $reportValues = {};
	
	unless( -e "$fileName" ) {
		warn "FjrJobReport - job report file $fileName does not exist.\n";
		return 0;
	}
	
	unless( open REPORTFILE, "<$fileName" ) {
		warn "FjrJobReport - can not open job report file $fileName.\n";
		return 0;
	}
	while( my $line = <REPORTFILE> ) {
		if( $line =~ /^(\S+)::(\S+)\.(\S+) (.*)/ ) {
			$reportValues->{$1}->{$2}->{$3} = $4;
		}
	}
	close REPORTFILE;
	return $reportValues;
}

1;

__END__

=pod

=head1 FjrJobReport.pm

FjrJobReport.pm -- standard module for loading values from a FrameWork job
report, and holding these values in a perl object.

=head1 SYNOPSIS

	use FrameJobReport::FjrJobReport;
	my $reportValues = FrameJobReport::FjrJobReport->new("file1");
	my $value1 = $reportValue->{report}->{Bbr}->{hierarchy}->{name};
	
=head1 Description

A perl module for loading the values from a FramWork job report file, a 
.trd file.  The module requires the path to the file at creation, and
this file will get read at the time of the opject creation.

The values get loaded into a hash called "report", and it is a multi-
refference hash, where the levels are:

	1. Bbr or Usr
	2. Report value heirarchy
	3. Report value name
	
To find the value of a name in the report file then needs to ref. 
through these levels.  

	i.e. $opject->{report}->{level1}->{level2}->{level3}
	
And this will return the value.

=head1 To Do

Need to provide methods to list the keys to the different levels.

Probably need to deal with the format of the heirarchy better.
Need a simpler interface to value just from name.

=head1 AUTHOR

Douglas Smith <douglas@slac.stanford.edu>

=cut
