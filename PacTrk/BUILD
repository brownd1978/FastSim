To build the PacTrk package within a BaBar test release:

0) make a new test release based on 24.3.0 or later
1) add PackageList to your test release
shell> addpkg PackageList
2) patch link_all_reco.mk
shell> patch PackageList/link_all_reco.mk PacTrk/link_all_reco.mk.dif
4) add the particles database (NB: only needed for standalone tests, not PravdaMC)
shell> cvs co PDT/pdt.table 
5) add packages (for revision 152 and higher, release before 24.3.2b)
addpkg SvtPid  V00-06-05  (Release uses V00-06-04)
addpkg TrkBase  V02-10-16  (Release uses V02-10-14)
6) make in the normal BaBar way
shell> make PacTrk.all 
