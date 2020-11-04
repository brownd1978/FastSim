
add_library(CrnLib SHARED 
abend.F
chisin.F
ddilog.F
dzero.F
#ffcard.F
#fffind.F
#ffget.F
#ffgo.F
#ffinit.F
#ffkey.F
#ffread.F
#ffrset.F
#ffset.F
#ffskip.F
#ffupca.F
#ffuser.F
#ffuset.F
flpsor.F
freq.F
gausin.F
#genbod.F
iucomp.F
kerset.F
lenocc.F
locf.F
lorenb.F
mtlprt.F
mtlset.F
pdk.F
polint.F
ranf.F
rm48.F
rotes2.F
rzero.F
ucopy.F
ucopy2.F
uctoh.F
uhtoc.F
vfill.F
vzero.F
)

# set top-level directory as include root
target_include_directories(CrnLib PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(CrnLib PROPERTIES VERSION ${PROJECT_VERSION})
