//System Includes
#include <atomic>
#include <memory>
#include <string>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::atomic;
using std::string;
using std::error_code;
using std::make_shared;
using std::make_error_code;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Launching tasks on a single thread." )
{
    int count = 0;
    const auto counter = [ &count ]( void )
    {
        count++;
        return error_code( );
    };
    
    auto runloop = make_shared< RunLoop >( );
    runloop->set_worker_limit( 1 );
    
    runloop->launch( counter );
    runloop->launch( counter );
    runloop->launch( counter );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( count == 3 );
}

TEST_CASE( "Launching tasks on multiple threads." )
{
    atomic< int > count( 0 );
    const auto counter = [ &count ]( void )
    {
        count++;
        return error_code( );
    };
    
    auto runloop = make_shared< RunLoop >( );
    runloop->launch( counter );
    runloop->launch( counter );
    runloop->launch( counter );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( count == 3 );
}

TEST_CASE( "Launching tasks within other tasks." )
{
    atomic< int > count( 0 );
    const auto counter = [ &count ]( void )
    {
        count++;
        return error_code( );
    };
    
    auto runloop = make_shared< RunLoop >( );
    runloop->launch( [ runloop, &counter ]( void )
    {
        runloop->launch( counter );
        runloop->launch( counter );
        return error_code( );
    } );
    runloop->launch( counter );
    runloop->launch( counter );
    runloop->launch( counter );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( count == 5 );
}

TEST_CASE( "Rescheduling tasks." )
{
    int count = 0;
    const auto counter = [ &count ]( void )
    {
        count++;
        if ( count == 4 ) return error_code( );
        else return make_error_code( std::errc::resource_unavailable_try_again );
    };
    
    auto runloop = make_shared< RunLoop >( );
    runloop->launch( counter );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( count == 4 );
}

TEST_CASE( "Returning errors from tasks." )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->launch( [ ]( void )
    {
        return make_error_code( std::errc::value_too_large );
    }, "my-key" );
    
    bool error_handler_called = false;
    runloop->set_error_handler( [ &error_handler_called ]( const string & key, const error_code & status, const string & message )
    {
        error_handler_called = true;
        REQUIRE( key == "my-key" );
        REQUIRE( message == "task execution failed." );
        REQUIRE( status == std::errc::value_too_large );
        return error_code( );
    } );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( error_handler_called == true );
}

TEST_CASE( "Throwing exceptions from launched tasks." )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->launch( [ ]( void )
    {
        throw "error";
        return error_code( );
    }, "my-id" );
    
    bool error_handler_called = false;
    runloop->set_error_handler( [ &error_handler_called ]( const string & key, const error_code & status, const string & message )
    {
        error_handler_called = true;
        REQUIRE( key == "my-id" );
        REQUIRE( status == error_code( ) );
        REQUIRE( message == "non-std::exception raised when calling task execution." );
        return error_code( );
    } );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( error_handler_called == true );
}
