cmake_minimum_required( VERSION 3.1.0 FATAL_ERROR )

project( "runloop" VERSION 1.0.0 LANGUAGES CXX )
message( "Copyright 2013-2018, Corvusoft Ltd, All Rights Reserved." )

#
# Dependencies
#
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake" )

find_package( catch REQUIRED )

#
# Configuration
#
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_INSTALL_LIBDIR library )
set( INCLUDE_DIR "${PROJECT_SOURCE_DIR}/source" )

if ( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
    set( CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/distribution" CACHE PATH "Install path prefix" FORCE )
endif ( )

if( ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_WIN32_WINNT=0x0601 /W4 /wd4068 /wd4702" )
endif ( )

if( NOT WIN32 )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Weffc++ -pedantic -Wno-unknown-pragmas" )
endif ( )

if ( UNIX AND NOT APPLE )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread" )
endif ( )

if ( APPLE )
    set( CMAKE_MACOSX_RPATH ON )
endif ( )

include_directories( ${INCLUDE_DIR} SYSTEM )

#
# Build
#
file( GLOB_RECURSE MANIFEST "source/*.cpp" )

set( STATIC_LIBRARY_NAME "${PROJECT_NAME}-static" )
add_library( ${STATIC_LIBRARY_NAME} STATIC ${MANIFEST} )
set_property( TARGET ${STATIC_LIBRARY_NAME} PROPERTY CXX_STANDARD 14 )
set_property( TARGET ${STATIC_LIBRARY_NAME} PROPERTY CXX_STANDARD_REQUIRED ON )
set_target_properties( ${STATIC_LIBRARY_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME} )
target_link_libraries( ${STATIC_LIBRARY_NAME} )

set( SHARED_LIBRARY_NAME "${PROJECT_NAME}-shared" )
add_library( ${SHARED_LIBRARY_NAME} SHARED ${MANIFEST} )
set_property( TARGET ${SHARED_LIBRARY_NAME} PROPERTY CXX_STANDARD 14 )
set_property( TARGET ${SHARED_LIBRARY_NAME} PROPERTY CXX_STANDARD_REQUIRED ON )
set_target_properties( ${SHARED_LIBRARY_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME} )
set_target_properties( ${SHARED_LIBRARY_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR} VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} )
target_link_libraries( ${SHARED_LIBRARY_NAME} )

enable_testing( )
#add_subdirectory( "${PROJECT_SOURCE_DIR}/test/unit" )
add_subdirectory( "${PROJECT_SOURCE_DIR}/test/feature" )
#add_subdirectory( "${PROJECT_SOURCE_DIR}/test/integration" )

#
# Install
#
file( GLOB ARTIFACTS "source/corvusoft/core/*.hpp" )

install( FILES ${ARTIFACTS} DESTINATION "${CMAKE_INSTALL_PREFIX}/include/corvusoft/core" )
install( TARGETS ${STATIC_LIBRARY_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT library )
install( TARGETS ${SHARED_LIBRARY_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT library )
