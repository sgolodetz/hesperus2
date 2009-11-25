#################
# LinkASX.cmake #
#################

INCLUDE(${hesperus2_SOURCE_DIR}/LinkAngelScript.cmake)
TARGET_LINK_LIBRARIES(${targetname} debug asx_d optimized asx)
