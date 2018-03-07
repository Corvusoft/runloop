//System Includes
#include <string>
#include <memory>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::string;
using std::make_shared;
using std::error_code;
using std::make_error_code;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Calling start twice" )
{
    auto runloop = make_shared< RunLoop >( );
    
    error_code status = runloop->start( );
    REQUIRE( status == error_code( ) );
    
    status = runloop->start( );
    REQUIRE( status == make_error_code( std::errc::operation_in_progress ) );
    
    status = runloop->stop( );
    REQUIRE( status == error_code( ) );
}

TEST_CASE( "Calling start on a suspended loop" )
{
    auto runloop = make_shared< RunLoop >( );
    error_code status = runloop->start( );
    REQUIRE( status == error_code( ) );
    
    runloop->suspend( );
    REQUIRE( status == error_code( ) );
    
    status = runloop->start( );
    REQUIRE( status == make_error_code( std::errc::operation_would_block ) );
    
    status = runloop->stop( );
    REQUIRE( status == error_code( ) );
}
