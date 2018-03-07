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
    
    runloop->launch_if( true, [ &task_called ]( void )
    {
        task_called++;
        return error_code( );
    } );
    
    error_code status = runloop->start( );
    REQUIRE( status == error_code( ) );
    
    status = runloop->wait( );
    REQUIRE( status == error_code( ) );
    REQUIRE( task_called == 1 );
    
    status = runloop->stop( );
    REQUIRE( status == error_code( ) );
}

TEST_CASE( "Launching tasks when an event occurs" )
{
    int task_called = 0;
    auto runloop = make_shared< RunLoop >( );
    
    function< error_code ( void ) > successful_event = [ ]( void )
    {
        return error_code( );
    };
    runloop->launch_if( successful_event, [ &task_called ]( void )
    {
        task_called++;
        return error_code( );
    } );
    
    
    function< error_code ( void ) > not_permitted_event = [ ]( void )
    {
        return make_error_code( std::errc::operation_not_permitted );
    };
    runloop->launch_if( not_permitted_event, [ &task_called ]( void )
    {
        task_called++;
        return error_code( );
    } );
    
    
    function< error_code ( void ) > operation_canceled_event = [ ]( void )
    {
        return make_error_code( std::errc::operation_canceled );
    };
    runloop->launch_if( operation_canceled_event, [ &task_called ]( void )
    {
        task_called++;
        return error_code( );
    } );
    
    
    error_code status = runloop->start( );
    REQUIRE( status == error_code( ) );
    
    status = runloop->wait( milliseconds( 100 ) );
    REQUIRE( status == error_code( ) );
    REQUIRE( task_called == 1 );
    
    status = runloop->stop( );
    REQUIRE( status == error_code( ) );
}

TEST_CASE( "Returning errors from task events" )
{
    bool error_handler_called = false;
    auto runloop = make_shared< RunLoop >( );
    runloop->set_error_handler( [ &error_handler_called ]( const string & key, const error_code & status, const string & message )
    {
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
    
    status = runloop->wait( );
    REQUIRE( status == error_code( ) );
    REQUIRE( error_handler_called == true );
    
    status = runloop->stop( );
    REQUIRE( status == error_code( ) );
}

TEST_CASE( "Throwing exceptions from task events" )
{
    bool error_handler_called = false;
    auto runloop = make_shared< RunLoop >( );
    runloop->set_error_handler( [ &error_handler_called ]( const string & key, const error_code & status, const string & message )
    {
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
    
    status = runloop->wait( );
    REQUIRE( status == error_code( ) );
    REQUIRE( error_handler_called == true );
    
    status = runloop->stop( );
    REQUIRE( status == error_code( ) );
}
