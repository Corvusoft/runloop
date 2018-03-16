//System Includes

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Destructor." )
{
    const auto runloop = new RunLoop;
    REQUIRE_NOTHROW( delete runloop );
}
