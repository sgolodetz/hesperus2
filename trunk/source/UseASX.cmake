################
# UseASX.cmake #
################

INCLUDE(${hesperus2_SOURCE_DIR}/UseAngelScript.cmake)
INCLUDE(${hesperus2_SOURCE_DIR}/UseBoost.cmake)

INCLUDE_DIRECTORIES(${hesperus2_SOURCE_DIR}/../libraries/asx-2.17.2/include)
LINK_DIRECTORIES(${hesperus2_SOURCE_DIR}/../libraries/asx-2.17.2/lib)
