############################
# CMakeLists.txt for tests #
############################

#################
# Library Tests #
#################

ADD_SUBDIRECTORY(test-angelscript_2.17.2)
ADD_SUBDIRECTORY(test-asx-2.17.2)
ADD_SUBDIRECTORY(test-boost_1_41_0)
ADD_SUBDIRECTORY(test-FMOD-4.28.02)
ADD_SUBDIRECTORY(test-propparser)
ADD_SUBDIRECTORY(test-SDL-1.2.14)

###############
# Other Tests #
###############

ADD_SUBDIRECTORY(test-findexe)
ADD_SUBDIRECTORY(test-fsm)
ADD_SUBDIRECTORY(test-hsm)
