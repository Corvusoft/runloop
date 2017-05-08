cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )

project( "unit test suite" LANGUAGES CXX )

#
# Configuration
#
set( SOURCE_DIR "source" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCATCH_CONFIG_MAIN" )

include_directories( SYSTEM ${catch_INCLUDE} )

#
# Build
#
add_executable( unit_test_suite ${SOURCE_DIR}/suite.cpp )
target_link_libraries( unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/unit_test_suite )
