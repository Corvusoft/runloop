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
using std::make_shared;
using std::error_code;
using std::make_error_code;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Calling wait twice", "[runloop::wait]" )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->launch( [ runloop ]( void )
    {
        REQUIRE( runloop->is_stopped( ) == false );
        REQUIRE( runloop->wait( ) == error_code( ) );
        REQUIRE( runloop->wait( ) == error_code( ) );
        
        runloop->stop( );
        return error_code( );
    } );
    
    REQUIRE( runloop->start( ) == error_code( ) );
}

TEST_CASE( "Calling wait on a suspended loop", "[runloop::wait]" )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->launch( [ runloop ]( void )
    {
        REQUIRE( runloop->is_stopped( ) == false );
        REQUIRE( runloop->is_suspended( ) == false );
        
        runloop->suspend( );
        REQUIRE( runloop->is_suspended( ) == true );
        
        REQUIRE( runloop->wait( ) == make_error_code( std::errc::operation_would_block ) );
        
        runloop->stop( );
        return error_code( );
    } );
    
    REQUIRE( runloop->start( ) == error_code( ) );
}
