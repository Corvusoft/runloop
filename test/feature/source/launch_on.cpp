//System Includes
#include <memory>
#include <csignal>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::raise;
using std::error_code;
using std::make_shared;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Launching tasks on operating system signals." )
{
    bool sigalrm_handler_called = false;
    auto runloop = make_shared< RunLoop >( );
    runloop->launch_on( SIGUSR1, [ &sigalrm_handler_called ]( error_code )
    {
        sigalrm_handler_called = true;
        return error_code( );
    } );
    
    runloop->launch( [ ]( error_code )
    {
        raise( SIGUSR1 );
        return error_code( );
    } );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( sigalrm_handler_called == true );
}
