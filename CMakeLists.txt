include(dciIntegrationUnit)
dciIntegrationUnit(utils
        INCLUDE_DIRS include
        CMM_DIRS cmakeModules
        DEPENDS test)

file(GLOB_RECURSE INC include/*)
file(GLOB_RECURSE SRC src/*)
add_library(${UNAME} SHARED ${INC} ${SRC} cmakeModules/dciUtilsPch.cmake)
dciIntegrationSetupTarget(${UNAME})

file(GLOB_RECURSE TST test/*)
include(dciTest)
dciTest(${UNAME} noenv
    SRC ${TST}
    LINK ${UNAME}
)
