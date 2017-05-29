//System Includes
#include <string>
#include <chrono>
#include <functional>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::string;
using std::function;
using std::error_code;
using std::chrono::seconds;
using std::chrono::nanoseconds;
using std::chrono::milliseconds;
using std::chrono::system_clock;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Launch task in delay period with inactive loop" )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.launch_in( seconds( 1 ), nullptr ) );
    REQUIRE_NOTHROW( runloop.launch_in( milliseconds( 500 ), nullptr, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch_in( milliseconds( 200 ), [ ]( void )
    {
        return error_code( );
    } ) );
    REQUIRE_NOTHROW( runloop.launch_in( seconds( 1 ), [ ]( void )
    {
        return error_code( );
    }, "test-key-value" ) );
}
