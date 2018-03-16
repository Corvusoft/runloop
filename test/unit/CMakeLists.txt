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
add_executable( cancel_unit_test_suite ${SOURCE_DIR}/cancel.cpp )
target_link_libraries( cancel_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( cancel_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/cancel_unit_test_suite )

add_executable( constructor_unit_test_suite ${SOURCE_DIR}/constructor.cpp )
target_link_libraries( constructor_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( constructor_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/constructor_unit_test_suite )

add_executable( destructor_unit_test_suite ${SOURCE_DIR}/destructor.cpp )
target_link_libraries( destructor_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( destructor_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/destructor_unit_test_suite )

add_executable( launch_at_unit_test_suite ${SOURCE_DIR}/launch_at.cpp )
target_link_libraries( launch_at_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( launch_at_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/launch_at_unit_test_suite )

add_executable( launch_if_unit_test_suite ${SOURCE_DIR}/launch_if.cpp )
target_link_libraries( launch_if_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( launch_if_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/launch_if_unit_test_suite )

add_executable( launch_in_unit_test_suite ${SOURCE_DIR}/launch_in.cpp )
target_link_libraries( launch_in_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( launch_in_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/launch_in_unit_test_suite )

add_executable( launch_unit_test_suite ${SOURCE_DIR}/launch.cpp )
target_link_libraries( launch_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( launch_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/launch_unit_test_suite )

add_executable( resume_unit_test_suite ${SOURCE_DIR}/resume.cpp )
target_link_libraries( resume_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( resume_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/resume_unit_test_suite )

add_executable( set_error_handler_unit_test_suite ${SOURCE_DIR}/set_error_handler.cpp )
target_link_libraries( set_error_handler_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( set_error_handler_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/set_error_handler_unit_test_suite )

add_executable( set_log_handler_unit_test_suite ${SOURCE_DIR}/set_log_handler.cpp )
target_link_libraries( set_log_handler_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( set_log_handler_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/set_log_handler_unit_test_suite )

add_executable( set_ready_handler_unit_test_suite ${SOURCE_DIR}/set_ready_handler.cpp )
target_link_libraries( set_ready_handler_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( set_ready_handler_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/set_ready_handler_unit_test_suite )

add_executable( set_worker_limit_unit_test_suite ${SOURCE_DIR}/set_worker_limit.cpp )
target_link_libraries( set_worker_limit_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( set_worker_limit_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/set_worker_limit_unit_test_suite )

add_executable( stop_unit_test_suite ${SOURCE_DIR}/stop.cpp )
target_link_libraries( stop_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( stop_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/stop_unit_test_suite )

add_executable( suspend_unit_test_suite ${SOURCE_DIR}/suspend.cpp )
target_link_libraries( suspend_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( suspend_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/suspend_unit_test_suite )

add_executable( wait_unit_test_suite ${SOURCE_DIR}/wait.cpp )
target_link_libraries( wait_unit_test_suite ${STATIC_LIBRARY_NAME} )
add_test( wait_unit_test_suite ${EXECUTABLE_OUTPUT_PATH}/wait_unit_test_suite )
