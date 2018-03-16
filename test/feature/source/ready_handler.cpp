//System Includes
#include <memory>
#include <string>
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

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Ready handler." )
{
    bool ready_handler_called = false;
    auto runloop = make_shared< RunLoop >( );
    runloop->set_ready_handler( [ runloop, &ready_handler_called ]( void )
    {
        ready_handler_called = true;
        REQUIRE( runloop->is_stopped( ) == false );
        REQUIRE( runloop->is_suspended( ) == false );
        return error_code( );
    } );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( ready_handler_called == true );
}

TEST_CASE( "Returning errors from the ready handler." )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->set_ready_handler( [ ]( void )
    {
        return make_error_code( std::errc::connection_aborted );
    } );
    
    bool error_handler_called = false;
    runloop->set_error_handler( [ &error_handler_called ]( const string & key, const error_code & status, const string & message )
    {
        error_handler_called = true;
        REQUIRE( message == "task execution failed." );
        REQUIRE( status == std::errc::connection_aborted );
        REQUIRE( key == "corvusoft::core::runloop::ready_handler" );
        return error_code( );
    } );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( error_handler_called == true );
}

TEST_CASE( "Throwing exceptions from the ready handler." )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->set_ready_handler( [ ]( void )
    {
        throw "error";
        return error_code( );
    } );
    
    bool error_handler_called = false;
    runloop->set_error_handler( [ &error_handler_called ]( const string & key, const error_code & status, const string & message )
    {
        error_handler_called = true;
        REQUIRE( status == error_code( ) );
        REQUIRE( key == "corvusoft::core::runloop::ready_handler" );
        REQUIRE( message == "non-std::exception raised when calling task execution." );
        return error_code( );
    } );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( error_handler_called == true );
}
