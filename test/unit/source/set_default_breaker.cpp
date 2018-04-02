//System Includes
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::error_code;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Alter default task circuit breaker limit." )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.set_default_breaker( -10 ) );
    REQUIRE_NOTHROW( runloop.set_default_breaker( 0 ) );
    REQUIRE_NOTHROW( runloop.set_default_breaker( 99999 ) );
}
