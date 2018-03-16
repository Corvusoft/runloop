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
using std::chrono::seconds;
using std::chrono::time_point;
using std::chrono::system_clock;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Launching tasks at a specific datestamp." )
{
    bool task_called = false;
    const auto datestamp = system_clock::now( ) + seconds( 1 );
    
    auto runloop = make_shared< RunLoop >( );
    runloop->launch_at( datestamp, [ &task_called, datestamp ]( void )
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
