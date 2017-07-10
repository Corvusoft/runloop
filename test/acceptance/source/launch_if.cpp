//System Includes
#include <chrono>
#include <memory>
#include <string>
#include <ciso646>
#include <functional>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::string;
using std::function;
using std::error_code;
using std::make_shared;
using std::make_error_code;
using std::chrono::milliseconds;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Launching tasks when a condition is met" )
{
    int task_called = 0;
    auto runloop = make_shared< RunLoop >( );
    
    runloop->launch_if( false, [ &task_called ]( void )
    {
        task_called++;
        return error_code( );
    } );
    runloop->launch( [ &runloop ]( void )
    {
        runloop->wait( );
        runloop->stop( );
        return error_code( );
    } );
    
    error_code status = runloop->start( );
    
    REQUIRE( status == error_code( ) );
    REQUIRE( task_called == 0 );
    
    runloop->launch_if( true, [ &task_called, runloop ]( void )
    {
        task_called++;
        runloop->stop( );
        return error_code( );
    } );
    
    status = runloop->start( );
    
    REQUIRE( status == error_code( ) );
    REQUIRE( task_called == 1 );
}

TEST_CASE( "Launching tasks when an event occurs" )
{
    auto runloop = make_shared< RunLoop >( );
    
    int task_called = 0;
    function< error_code ( void ) > event = [ ]( void )
    {
        return error_code( );
    };
    
    runloop->launch_if( event, [ &task_called, runloop ]( void )
    {
        task_called++;
        runloop->stop( );
        return error_code( );
    } );
    error_code status = runloop->start( );
    
    REQUIRE( status == error_code( ) );
    REQUIRE( task_called == 1 );
    
    task_called = 0;
    event = [ ]( void )
    {
        return make_error_code( std::errc::operation_not_permitted );
    };
    
    runloop->launch_if( event, [ &task_called ]( void )
    {
        task_called++;
        return error_code( );
    } );
    runloop->launch( [ &runloop ]( void )
    {
        runloop->wait( milliseconds( 500 ) );
        runloop->stop( );
        return error_code( );
    } );
    status = runloop->start( );
    
    REQUIRE( status == error_code( ) );
    REQUIRE( task_called == 0 );
    
    task_called = 0;
    event = [ ]( void )
    {
        return make_error_code( std::errc::operation_canceled );
    };
    
    runloop->launch_if( event, [ &task_called ]( void )
    {
        task_called++;
        return error_code( );
    } );
    runloop->launch( [ &runloop ]( void )
    {
        runloop->wait( );
        runloop->stop( );
        return error_code( );
    } );
    status = runloop->start( );
    
    REQUIRE( status == error_code( ) );
    REQUIRE( task_called == 0 );
}

TEST_CASE( "Returning errors from task events" )
{
    bool error_handler_called = false;
    auto runloop = make_shared< RunLoop >( );
    runloop->set_error_handler( [ runloop, &error_handler_called ]( const string & key, const error_code & status, const string & message )
    {
        runloop->stop( );
        error_handler_called = true;
        REQUIRE( key.empty( ) );
        REQUIRE( not message.empty( ) );
        REQUIRE( status == std::errc::not_a_socket );
        return error_code( );
    } );
    
    const function< error_code ( void ) > event = [ ]( void )
    {
        return make_error_code( std::errc::not_a_socket );
    };
    runloop->launch_if( event, [ ]( void )
    {
        return error_code( );
    } );
    error_code status = runloop->start( );
    
    REQUIRE( status == error_code( ) );
    REQUIRE( error_handler_called == true );
}

TEST_CASE( "Throwing exceptions from task events" )
{
    bool error_handler_called = false;
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
    
    const function< error_code ( void ) > event = [ ]( void )
    {
        throw "arrrr";
        return error_code( );
    };
    runloop->launch_if( event, [ ]( void )
    {
        return error_code( );
    } );
    error_code status = runloop->start( );
    
    REQUIRE( status == error_code( ) );
    REQUIRE( error_handler_called == true );
}
