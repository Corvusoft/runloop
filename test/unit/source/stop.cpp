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

TEST_CASE( "Call stop with inactive loop." )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.stop( ) );
}
