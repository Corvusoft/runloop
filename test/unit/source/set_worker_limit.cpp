//System Includes
#include <limits>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::numeric_limits;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Alter worker limit" )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.set_worker_limit( numeric_limits< unsigned int >::min( ) ) );
    REQUIRE_NOTHROW( runloop.set_worker_limit( numeric_limits< unsigned int >::max( ) / 2 ) );
    REQUIRE_NOTHROW( runloop.set_worker_limit( numeric_limits< unsigned int >::max( ) ) );
}
