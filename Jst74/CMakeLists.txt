add_library(Jst74 SHARED 
klu.F
rlu.F
lu1ent.F
lu2ent.F
lu3ent.F
lu4ent.F
luboei.F
lucell.F
luchge.F
luclus.F
lucomp.F
lucomp_beg.F
ludata.F
ludecy.F
luedit.F
lueevt.F
luerrm.F
luexec.F
lufowo.F
lugive.F
luhepc.F
luindf.F
lujmas.F
lujoin.F
lukfdi.F
lulist.F
lulogo.F
luname.F
luonia.F
luprep.F
luptdi.F
luradk.F
lurobo.F
lushow.F
lusphe.F
lustrf.F
lutabu.F
lutaud.F
lutest.F
luthru.F
luupda.F
lux3jt.F
lux4jt.F
luxdif.F
luxjet.F
luxkfl.F
luxtot.F
luzdis.F
plu.F
rluget.F
rluset.F
ulalem.F
ulalps.F
ulangl.F
ulmass.F
)

# set top-level directory as include root
target_include_directories(Jst74 PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(Jst74 PROPERTIES VERSION ${PROJECT_VERSION})
