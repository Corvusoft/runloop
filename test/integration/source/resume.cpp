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

TEST_CASE( "Calling resume twice" )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->launch( [ runloop ]( void )
    {
        REQUIRE( runloop->is_stopped( ) == false );
        REQUIRE( runloop->is_suspended( ) == false );
        
        runloop->resume( );
        REQUIRE( runloop->is_stopped( ) == false );
        REQUIRE( runloop->is_suspended( ) == false );
        
        runloop->resume( );
        REQUIRE( runloop->is_stopped( ) == false );
        REQUIRE( runloop->is_suspended( ) == false );
        
        runloop->stop( );
        return error_code( );
    } );
    
    REQUIRE( runloop->start( ) == error_code( ) );
}
