//System Includes

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Call resume with inactive loop" )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.resume( ) );
    REQUIRE( runloop.is_stopped( ) == true );
    REQUIRE( runloop.is_suspended( ) == false );
}
