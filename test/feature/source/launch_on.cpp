//System Includes
#include <memory>
#include <string>
#include <ciso646>
#include <csignal>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::raise;
using std::string;
using std::error_code;
using std::make_shared;
using std::make_error_code;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Launching tasks on operating system signals" )
{
    bool sigalrm_handler_called = false;
    auto runloop = make_shared< RunLoop >( );
    runloop->launch_on( SIGUSR1, [ &sigalrm_handler_called ]( void )
    {
        sigalrm_handler_called = true;
        return error_code( );
    } );
    
    runloop->launch( [ ]( void )
    {
        raise( SIGUSR1 );
        return error_code( );
    } );
    
    error_code status = runloop->start( );
    REQUIRE( status == error_code( ) );
    
    status = runloop->wait( );
    REQUIRE( status == error_code( ) );
    REQUIRE( sigalrm_handler_called == true );
    
    status = runloop->stop( );
    REQUIRE( status == error_code( ) );
}

TEST_CASE( "Returning errors from signal handlers" )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->launch_on( SIGUSR1, [ ]( void )
    {
        return make_error_code( std::errc::not_a_socket );
    }, "SIGALRM HANDLER" );
    
    bool error_handler_called = false;
    runloop->set_error_handler( [ &error_handler_called ]( const string & key, const error_code & code, const string & message )
    {
        error_handler_called = true;
        REQUIRE( key == "SIGALRM HANDLER" );
        REQUIRE( code == std::errc::not_a_socket );
        REQUIRE( not message.empty( ) );
        return error_code( );
    } );
    
    runloop->set_ready_handler( [ ]( void )
    {
        raise( SIGUSR1 );
        return error_code( );
    } );
    
    error_code status = runloop->start( );
    REQUIRE( status == error_code( ) );
    
    status = runloop->wait( );
    REQUIRE( status == error_code( ) );
    
    status = runloop->stop( );
    REQUIRE( status == error_code( ) );
    REQUIRE( error_handler_called == true );
}

TEST_CASE( "Throwing exceptions from signal handlers" )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->launch_on( SIGUSR1, [ ]( void )
    {
        throw "error";
        return error_code( );
    }, "SIGALRM HANDLER" );
    
    bool error_handler_called = false;
    runloop->set_error_handler( [ &error_handler_called ]( const string & key, const error_code & code, const string & message )
    {
        error_handler_called = true;
        REQUIRE( key == "SIGALRM HANDLER" );
        REQUIRE( code == std::errc::operation_canceled );
        REQUIRE( not message.empty( ) );
        return error_code( );
    } );
    
    runloop->set_ready_handler( [ ]( void )
    {
        raise( SIGUSR1 );
        return error_code( );
    } );
    
    error_code status = runloop->start( );
    REQUIRE( status == error_code( ) );
    
    status = runloop->wait( );
    REQUIRE( status == error_code( ) );
    
    status = runloop->stop( );
    REQUIRE( status == error_code( ) );
    REQUIRE( error_handler_called == true );
}
