cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )

project( "feature test suite" LANGUAGES CXX )

#
# Configuration
#
set( SOURCE_DIR "source" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCATCH_CONFIG_MAIN" )

include_directories( SYSTEM ${catch_INCLUDE} )

#
# Build
#
add_executable( cancel_feature_test_suite ${SOURCE_DIR}/cancel.cpp )
target_link_libraries( cancel_feature_test_suite ${STATIC_LIBRARY_NAME} )
add_test( cancel_feature_test_suite ${EXECUTABLE_OUTPUT_PATH}/cancel_feature_test_suite )

add_executable( launch_in_feature_test_suite ${SOURCE_DIR}/launch_in.cpp )
target_link_libraries( launch_in_feature_test_suite ${STATIC_LIBRARY_NAME} )
add_test( launch_in_feature_test_suite ${EXECUTABLE_OUTPUT_PATH}/launch_in_feature_test_suite )

add_executable( launch_at_feature_test_suite ${SOURCE_DIR}/launch_at.cpp )
target_link_libraries( launch_at_feature_test_suite ${STATIC_LIBRARY_NAME} )
add_test( launch_at_feature_test_suite ${EXECUTABLE_OUTPUT_PATH}/launch_at_feature_test_suite )

add_executable( launch_if_feature_test_suite ${SOURCE_DIR}/launch_if.cpp )
target_link_libraries( launch_if_feature_test_suite ${STATIC_LIBRARY_NAME} )
add_test( launch_if_feature_test_suite ${EXECUTABLE_OUTPUT_PATH}/launch_if_feature_test_suite )

add_executable( launch_on_feature_test_suite ${SOURCE_DIR}/launch_on.cpp )
target_link_libraries( launch_on_feature_test_suite ${STATIC_LIBRARY_NAME} )
add_test( launch_on_feature_test_suite ${EXECUTABLE_OUTPUT_PATH}/launch_on_feature_test_suite )

add_executable( launch_feature_test_suite ${SOURCE_DIR}/launch.cpp )
target_link_libraries( launch_feature_test_suite ${STATIC_LIBRARY_NAME} )
add_test( launch_feature_test_suite ${EXECUTABLE_OUTPUT_PATH}/launch_feature_test_suite )

add_executable( ready_handler_feature_test_suite ${SOURCE_DIR}/ready_handler.cpp )
target_link_libraries( ready_handler_feature_test_suite ${STATIC_LIBRARY_NAME} )
add_test( ready_handler_feature_test_suite ${EXECUTABLE_OUTPUT_PATH}/ready_handler_feature_test_suite )

add_executable( wait_feature_test_suite ${SOURCE_DIR}/wait.cpp )
target_link_libraries( wait_feature_test_suite ${STATIC_LIBRARY_NAME} )
add_test( wait_feature_test_suite ${EXECUTABLE_OUTPUT_PATH}/wait_feature_test_suite )
