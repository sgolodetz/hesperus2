#######################
# SetToolTarget.cmake #
#######################

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${hesperus2_BINARY_DIR}/bin/engine/tools)
ADD_EXECUTABLE(${targetname} ${sources})
INCLUDE(${hesperus2_SOURCE_DIR}/VCTargetHack.cmake)
