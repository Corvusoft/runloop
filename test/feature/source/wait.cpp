//System Includes
#include <chrono>
#include <atomic>
#include <thread>
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
using std::chrono::milliseconds;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Waiting for all tasks to complete execution." )
{
    atomic< int > count( 0 );
    const auto counter = [ &count ]( error_code )
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
    REQUIRE( count == 3 );
    runloop->stop( );
}

TEST_CASE( "Waiting specified amount of time for tasks to complete execution." )
{
    atomic< int > count( 0 );
    const auto counter = [ &count ]( error_code )
    {
        std::this_thread::sleep_for( milliseconds( 500 ) );
        count++;
        return error_code( );
    };
    
    auto runloop = make_shared< RunLoop >( );
    runloop->launch( counter );
    runloop->launch( counter );
    runloop->launch( counter );
    
    runloop->start( );
    runloop->wait( milliseconds( 250 ) );
    REQUIRE( count == 0 );
    runloop->stop( );
}

TEST_CASE( "Waiting specified task to complete execution." )
{
    atomic< int > count( 0 );
    const auto counter = [ &count ]( error_code )
    {
        std::this_thread::sleep_for( milliseconds( 100 ) );
        count++;
        return error_code( );
    };
    
    auto runloop = make_shared< RunLoop >( );
    runloop->set_worker_limit( 1 );
    runloop->launch( counter, "abc" );
    runloop->launch( counter, "bcd" );
    runloop->launch( counter, "efg" );
    
    runloop->start( );
    runloop->wait( "bcd" );
    REQUIRE( count == 2 );
    
    runloop->stop( );
    REQUIRE( count == 3 );
}
