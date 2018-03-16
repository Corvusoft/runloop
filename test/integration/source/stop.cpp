//System Includes
#include <memory>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::make_shared;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Calling stop twice." )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->start( );
    REQUIRE( runloop->is_stopped( ) == false );
    REQUIRE( runloop->is_suspended( ) == false );
    
    runloop->stop( );
    REQUIRE( runloop->is_stopped( ) == true );
    REQUIRE( runloop->is_suspended( ) == false );
    
    runloop->stop( );
    REQUIRE( runloop->is_stopped( ) == true );
    REQUIRE( runloop->is_suspended( ) == false );
}

TEST_CASE( "Calling stop on a suspended loop." )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->start( );
    REQUIRE( runloop->is_stopped( ) == false );
    REQUIRE( runloop->is_suspended( ) == false );
    
    runloop->suspend( );
    REQUIRE( runloop->is_stopped( ) == false );
    REQUIRE( runloop->is_suspended( ) == true );
    
    runloop->stop( );
    REQUIRE( runloop->is_stopped( ) == true );
    REQUIRE( runloop->is_suspended( ) == false );
}
