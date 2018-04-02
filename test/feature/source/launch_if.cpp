//System Includes
#include <memory>
#include <functional>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::function;
using std::error_code;
using std::make_shared;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Launching tasks when a condition is met." )
{
    int task_called = 0;
    
    auto runloop = make_shared< RunLoop >( );
    runloop->launch_if( false, [ &task_called ]( error_code )
    {
        task_called++;
        return error_code( );
    } );
    
    runloop->launch_if( true, [ &task_called ]( error_code )
    {
        task_called++;
        return error_code( );
    } );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( task_called == 1 );
}
