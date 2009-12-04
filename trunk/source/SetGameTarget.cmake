#######################
# SetGameTarget.cmake #
#######################

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${hesperus2_BINARY_DIR}/bin/games/${targetname})
ADD_EXECUTABLE(${targetname} ${sources} ${headers} ${templates})
INCLUDE(${hesperus2_SOURCE_DIR}/VCTargetHack.cmake)

SET_TARGET_PROPERTIES(${targetname} PROPERTIES DEBUG_OUTPUT_NAME "${targetname}_d")