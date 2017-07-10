//System Includes
#include <chrono>
#include <atomic>
#include <memory>
#include <string>
#include <functional>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::atomic;
using std::string;
using std::function;
using std::error_code;
using std::make_shared;
using std::make_error_code;
using std::chrono::milliseconds;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Waiting for all tasks to complete execution" )
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
    runloop->launch( [ runloop, &counter ]( void )
    {
        runloop->wait( );
        runloop->stop( );
        return error_code( );
    } );
    error_code status = runloop->start( );
    
    REQUIRE( count == 3 );
    REQUIRE( status == error_code( ) );
}

TEST_CASE( "Waiting specified amount of time for tasks to complete execution" )
{
    atomic< int > count( 0 );
    const auto counter = [ &count ]( void )
    {
        count++;
        return error_code( );
    };
    const function< error_code ( void ) > will_fire = [ ]( void )
    {
        return error_code( );
    };
    const function< error_code ( void ) > wont_fire = [ ]( void )
    {
        return make_error_code( std::errc::operation_not_permitted );
    };
    
    auto runloop = make_shared< RunLoop >( );
    runloop->launch_if( wont_fire, counter );
    runloop->launch_if( will_fire, counter );
    runloop->launch_if( wont_fire, counter );
    runloop->launch( [ runloop, &counter ]( void )
    {
        runloop->wait( milliseconds( 250 ) );
        runloop->stop( );
        return error_code( );
    } );
    error_code status = runloop->start( );
    
    REQUIRE( count == 1 );
    REQUIRE( status == error_code( ) );
}
