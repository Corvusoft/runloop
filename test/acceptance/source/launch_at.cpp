//System Includes
#include <chrono>
#include <memory>
#include <string>
#include <ciso646>
#include <stdexcept>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::string;
using std::error_code;
using std::make_shared;
using std::make_error_code;
using std::invalid_argument;
using std::chrono::seconds;
using std::chrono::time_point;
using std::chrono::system_clock;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Launching tasks at a specific datestamp" )
{
    bool task_called = false;
    const auto datestamp = system_clock::now( ) + seconds( 1 );
    
    auto runloop = make_shared< RunLoop >( );
    runloop->launch_at( datestamp, [ &task_called, datestamp, runloop ]( void )
    {
        runloop->stop( );
        task_called = true;
        REQUIRE( system_clock::now( ) >= datestamp );
        return error_code( );
    } );
    
    error_code status = runloop->start( );
    
    REQUIRE( status == error_code( ) );
    REQUIRE( task_called == true );
}

TEST_CASE( "Returning errors from tasks" )
{
    bool error_handler_called = false;
    const auto datestamp = system_clock::now( );
    
    auto runloop = make_shared< RunLoop >( );
    runloop->set_error_handler( [ runloop, &error_handler_called ]( const string & key, const error_code & status, const string & message )
    {
        runloop->stop( );
        error_handler_called = true;
        REQUIRE( key.empty( ) );
        REQUIRE( not message.empty( ) );
        REQUIRE( status == std::errc::already_connected );
        return error_code( );
    } );
    
    runloop->launch_at( datestamp, [ ]( void )
    {
        return make_error_code( std::errc::already_connected );
    } );
    error_code status = runloop->start( );
    
    REQUIRE( status == error_code( ) );
    REQUIRE( error_handler_called == true );
}

TEST_CASE( "Throwing exceptions from tasks" )
{
    bool error_handler_called = false;
    const auto datestamp = system_clock::now( );
    
    auto runloop = make_shared< RunLoop >( );
    runloop->set_error_handler( [ runloop, &error_handler_called ]( const string & key, const error_code & status, const string & message )
    {
        runloop->stop( );
        error_handler_called = true;
        REQUIRE( key.empty( ) );
        REQUIRE( not message.empty( ) );
        REQUIRE( status == std::errc::operation_canceled );
        return error_code( );
    } );
    
    runloop->launch_at( datestamp, [ ]( void )
    {
        throw invalid_argument( "bad" );
        return error_code( );
    } );
    error_code status = runloop->start( );
    
    REQUIRE( status == error_code( ) );
    REQUIRE( error_handler_called == true );
}
