//System Includes
#include <chrono>
#include <memory>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::error_code;
using std::make_shared;
using std::chrono::time_point;
using std::chrono::system_clock;
using std::chrono::milliseconds;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Launching tasks after a specific delay." )
{
    bool task_called = false;
    const auto delay = milliseconds( 500 );
    const auto datestamp = system_clock::now( ) + delay;
    
    auto runloop = make_shared< RunLoop >( );
    runloop->launch_in( delay, [ &task_called, datestamp ]( error_code )
    {
        task_called = true;
        REQUIRE( system_clock::now( ) > datestamp );
        return error_code( );
    } );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( task_called == true );
}
