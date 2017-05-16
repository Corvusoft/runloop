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

TEST_CASE( "Calling start twice" )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->launch( [ runloop ]( void )
    {
        REQUIRE( runloop->is_stopped( ) == false );
        REQUIRE( runloop->start( ) == make_error_code( std::errc::operation_in_progress ) );
        
        runloop->stop( );
        return error_code( );
    } );
    
    REQUIRE( runloop->start( ) == error_code( ) );
}

TEST_CASE( "Calling start on a suspended loop" )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->launch( [ runloop ]( void )
    {
        REQUIRE( runloop->is_stopped( ) == false );
        REQUIRE( runloop->is_suspended( ) == false );
        
        runloop->suspend( );
        REQUIRE( runloop->is_suspended( ) == true );
        
        REQUIRE( runloop->start( ) == make_error_code( std::errc::operation_would_block ) );
        
        runloop->stop( );
        return error_code( );
    } );
    
    REQUIRE( runloop->start( ) == error_code( ) );
}
