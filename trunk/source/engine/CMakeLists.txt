#############################
# CMakeLists.txt for engine #
#############################

ADD_SUBDIRECTORY(core)

OPTION(BUILD_TOOLS "Build the engine tools?" ON)
IF(BUILD_TOOLS)
	ADD_SUBDIRECTORY(tools)
ENDIF(BUILD_TOOLS)
