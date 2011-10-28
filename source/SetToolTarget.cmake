#######################
# SetToolTarget.cmake #
#######################

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${hesperus2_BINARY_DIR}/bin/engine/tools)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${hesperus2_BINARY_DIR}/bin/engine/tools)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${hesperus2_BINARY_DIR}/bin/engine/tools)
ADD_EXECUTABLE(${targetname} ${sources} ${headers} ${templates})
INCLUDE(${hesperus2_SOURCE_DIR}/VCLibraryHack.cmake)

SET_TARGET_PROPERTIES(${targetname} PROPERTIES DEBUG_OUTPUT_NAME "${targetname}_d")
