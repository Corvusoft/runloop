//System Includes
#include <memory>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::error_code;
using std::make_shared;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Calling suspend twice" )
{
    auto runloop = make_shared< RunLoop >( );
    
    error_code status = runloop->start( );
    REQUIRE( status == error_code( ) );
    REQUIRE( runloop->is_stopped( ) == false );
    REQUIRE( runloop->is_suspended( ) == false );
    
    status = runloop->suspend( );
    REQUIRE( status == error_code( ) );
    REQUIRE( runloop->is_stopped( ) == false );
    REQUIRE( runloop->is_suspended( ) == true );
    
    status = runloop->suspend( );
    REQUIRE( status == error_code( ) );
    REQUIRE( runloop->is_stopped( ) == false );
    REQUIRE( runloop->is_suspended( ) == true );
    
    status = runloop->stop( );
    REQUIRE( status == error_code( ) );
    REQUIRE( runloop->is_stopped( ) == true );
    REQUIRE( runloop->is_suspended( ) == false );
}
