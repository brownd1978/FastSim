
add_library(ProbTools SHARED 

AbsLikelihoodFraction.cc
AsymGaussConsistency.cc
ChisqConsistency.cc
CombinedConsistency.cc
Consistency.cc
ConsistencySet.cc
GaussConsistency.cc
NumRecipes.cc
PoissonConsistency.cc
probab.cc
)
target_link_libraries(ProbTools ROOT::MathCore ErrLogger BaBar )

# set top-level directory as include root
target_include_directories(ProbTools PRIVATE ${PROJECT_SOURCE_DIR})

# set shared library version equal to project version
set_target_properties(ProbTools PROPERTIES VERSION ${PROJECT_VERSION})
