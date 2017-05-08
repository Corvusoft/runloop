//System Includes
#include <string>
#include <memory>
#include <functional>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::string;
using std::function;
using std::make_shared;
using std::error_code;
using std::make_error_code;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Call stop with inactive loop", "[runloop::stop]" )
{
    RunLoop runloop;
    REQUIRE( runloop.stop( ) == error_code( ) );
    REQUIRE( runloop.is_stopped( ) == true );
    REQUIRE( runloop.is_suspended( ) == false );
}

TEST_CASE( "Calling stop twice", "[runloop::stop]" )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->launch( [ runloop ]( void )
    {
        REQUIRE( runloop->is_stopped( ) == false );
        REQUIRE( runloop->is_suspended( ) == false );
        
        REQUIRE( runloop->stop( ) == error_code( ) );
        REQUIRE( runloop->is_stopped( ) == true );
        REQUIRE( runloop->is_suspended( ) == false );
        
        REQUIRE( runloop->stop( ) == error_code( ) );
        REQUIRE( runloop->is_stopped( ) == true );
        REQUIRE( runloop->is_suspended( ) == false );
        
        return error_code( );
    } );
    
    REQUIRE( runloop->start( ) == error_code( ) );
}

TEST_CASE( "Calling stop on a suspended loop", "[runloop::stop]" )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->launch( [ runloop ]( void )
    {
        REQUIRE( runloop->is_stopped( ) == false );
        REQUIRE( runloop->is_suspended( ) == false );
        
        runloop->suspend( );
        REQUIRE( runloop->is_suspended( ) == true );
        
        REQUIRE( runloop->stop( ) == error_code( ) );
        REQUIRE( runloop->is_stopped( ) == true );
        REQUIRE( runloop->is_suspended( ) == false );
        
        return error_code( );
    } );
    
    REQUIRE( runloop->start( ) == error_code( ) );
}
