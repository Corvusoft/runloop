//System Includes
#include <regex>
#include <functional>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::regex;
using std::function;
using std::error_code;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Call cancel with inactive loop." )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.cancel( ) );
}

TEST_CASE( "Call cancel with unknown task key on inactive loop." )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.cancel( "123456" ) );
}

TEST_CASE( "Call cancel with unknown task key pattern on inactive loop." )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.cancel( regex( "123456" ) ) );
}
