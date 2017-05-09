cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )

project( "acceptance test suite" LANGUAGES CXX )

#
# Configuration
#
set( SOURCE_DIR "source" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCATCH_CONFIG_MAIN" )

include_directories( SYSTEM ${catch_INCLUDE} )

#
# Build
#
add_executable( cancel_acceptance_test_suite ${SOURCE_DIR}/cancel.cpp )
target_link_libraries( cancel_acceptance_test_suite ${STATIC_LIBRARY_NAME} )
add_test( cancel_acceptance_test_suite ${EXECUTABLE_OUTPUT_PATH}/cancel_acceptance_test_suite )

add_executable( launch_at_acceptance_test_suite ${SOURCE_DIR}/launch_at.cpp )
target_link_libraries( launch_at_acceptance_test_suite ${STATIC_LIBRARY_NAME} )
add_test( launch_at_acceptance_test_suite ${EXECUTABLE_OUTPUT_PATH}/launch_at_acceptance_test_suite )

add_executable( launch_if_acceptance_test_suite ${SOURCE_DIR}/launch_if.cpp )
target_link_libraries( launch_if_acceptance_test_suite ${STATIC_LIBRARY_NAME} )
add_test( launch_if_acceptance_test_suite ${EXECUTABLE_OUTPUT_PATH}/launch_if_acceptance_test_suite )

add_executable( launch_on_acceptance_test_suite ${SOURCE_DIR}/launch_on.cpp )
target_link_libraries( launch_on_acceptance_test_suite ${STATIC_LIBRARY_NAME} )
add_test( launch_on_acceptance_test_suite ${EXECUTABLE_OUTPUT_PATH}/launch_on_acceptance_test_suite )

add_executable( launch_acceptance_test_suite ${SOURCE_DIR}/launch.cpp )
target_link_libraries( launch_acceptance_test_suite ${STATIC_LIBRARY_NAME} )
add_test( launch_acceptance_test_suite ${EXECUTABLE_OUTPUT_PATH}/launch_acceptance_test_suite )

#add_executable( logging_acceptance_test_suite ${SOURCE_DIR}/logging.cpp )
#target_link_libraries( logging_acceptance_test_suite ${STATIC_LIBRARY_NAME} )
#add_test( logging_acceptance_test_suite ${EXECUTABLE_OUTPUT_PATH}/logging_acceptance_test_suite )

add_executable( ready_handler_acceptance_test_suite ${SOURCE_DIR}/ready_handler.cpp )
target_link_libraries( ready_handler_acceptance_test_suite ${STATIC_LIBRARY_NAME} )
add_test( ready_handler_acceptance_test_suite ${EXECUTABLE_OUTPUT_PATH}/ready_handler_acceptance_test_suite )

#add_executable( suspend_acceptance_test_suite ${SOURCE_DIR}/suspend.cpp )
#target_link_libraries( suspend_acceptance_test_suite ${STATIC_LIBRARY_NAME} )
#add_test( suspend_acceptance_test_suite ${EXECUTABLE_OUTPUT_PATH}/suspend_acceptance_test_suite )

add_executable( wait_acceptance_test_suite ${SOURCE_DIR}/wait.cpp )
target_link_libraries( wait_acceptance_test_suite ${STATIC_LIBRARY_NAME} )
add_test( wait_acceptance_test_suite ${EXECUTABLE_OUTPUT_PATH}/wait_acceptance_test_suite )
