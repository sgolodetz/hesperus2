################
# UseSDL.cmake #
################

INCLUDE_DIRECTORIES(
${hesperus2_SOURCE_DIR}/../libraries/SDL-1.2.14/include
)

LINK_DIRECTORIES(
${hesperus2_SOURCE_DIR}/../libraries/SDL-1.2.14/lib
)

FIND_PACKAGE(OpenGL)
