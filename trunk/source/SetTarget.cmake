###################
# SetTarget.cmake #
###################

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${hesperus2_BINARY_DIR}/bin/tests/${targetname}/bin)
ADD_EXECUTABLE(${targetname} ${sources})
INCLUDE(${hesperus2_SOURCE_DIR}/VCTargetHack.cmake)
