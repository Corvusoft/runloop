//System Includes
#include <memory>
#include <string>
#include <ciso646>
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

TEST_CASE( "Ready handler" )
{
    bool ready_handler_called = false;
    
    auto runloop = make_shared< RunLoop >( );
    runloop->set_ready_handler( [ runloop, &ready_handler_called ]( void )
    {
        ready_handler_called = true;
        REQUIRE( runloop->is_stopped( ) == false );
        REQUIRE( runloop->is_suspended( ) == false );
        runloop->stop( );
        return error_code( );
    } );
    
    error_code status = runloop->start( );
    
    REQUIRE( status == error_code( ) );
    REQUIRE( ready_handler_called == true );
}

TEST_CASE( "Returning errors from the ready handler" )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->set_ready_handler( [ ]( void )
    {
        return make_error_code( std::errc::connection_aborted );
    } );
    
    bool error_handler_called = false;
    runloop->set_error_handler( [ &error_handler_called ]( const string & key, const error_code & code, const string & message )
    {
        error_handler_called = true;
        REQUIRE( key.empty( ) );
        REQUIRE( not message.empty( ) );
        REQUIRE( code == std::errc::connection_aborted );
        return error_code( );
    } );
    
    error_code status = runloop->start( );
    
    REQUIRE( error_handler_called == true );
    REQUIRE( status == std::errc::connection_aborted );
    REQUIRE( runloop->is_stopped( ) );
}

TEST_CASE( "Throwing exceptions from the ready handler" )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->set_ready_handler( [ ]( void )
    {
        throw "error";
        return error_code( );
    } );
    
    bool error_handler_called = false;
    runloop->set_error_handler( [ &error_handler_called ]( const string & key, const error_code & code, const string & message )
    {
        error_handler_called = true;
        REQUIRE( key.empty( ) );
        REQUIRE( not message.empty( ) );
        REQUIRE( code == std::errc::operation_canceled );
        return error_code( );
    } );
    
    error_code status = runloop->start( );
    
    REQUIRE( error_handler_called == true );
    REQUIRE( status == std::errc::operation_canceled );
    REQUIRE( runloop->is_stopped( ) );
}
