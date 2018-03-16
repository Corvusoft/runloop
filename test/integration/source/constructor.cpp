//System Includes

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Constructor." )
{
    RunLoop runloop;
    REQUIRE( runloop.is_stopped( ) == true );
    REQUIRE( runloop.is_suspended( ) == false );
}
