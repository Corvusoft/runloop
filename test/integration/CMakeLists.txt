cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )

project( "integration test suite" LANGUAGES CXX )

#
# Configuration
#
set( SOURCE_DIR "source" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCATCH_CONFIG_MAIN" )

include_directories( SYSTEM ${catch_INCLUDE} )

#
# Build
#
add_executable( cancel_integration_test_suite ${SOURCE_DIR}/cancel.cpp )
target_link_libraries( cancel_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( cancel_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/cancel_integration_test_suite )

add_executable( constructor_integration_test_suite ${SOURCE_DIR}/constructor.cpp )
target_link_libraries( constructor_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( constructor_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/constructor_integration_test_suite )

add_executable( resume_integration_test_suite ${SOURCE_DIR}/resume.cpp )
target_link_libraries( resume_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( resume_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/resume_integration_test_suite )

add_executable( start_integration_test_suite ${SOURCE_DIR}/start.cpp )
target_link_libraries( start_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( start_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/start_integration_test_suite )

add_executable( stop_integration_test_suite ${SOURCE_DIR}/stop.cpp )
target_link_libraries( stop_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( stop_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/stop_integration_test_suite )

add_executable( suspend_integration_test_suite ${SOURCE_DIR}/suspend.cpp )
target_link_libraries( suspend_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( suspend_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/suspend_integration_test_suite )

add_executable( wait_integration_test_suite ${SOURCE_DIR}/wait.cpp )
target_link_libraries( wait_integration_test_suite ${STATIC_LIBRARY_NAME} )
add_test( wait_integration_test_suite ${EXECUTABLE_OUTPUT_PATH}/wait_integration_test_suite )
