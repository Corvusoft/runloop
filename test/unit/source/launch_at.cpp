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
using std::chrono::system_clock;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Launch task at timestamp with inactive loop" )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.launch_at( system_clock::now( ), nullptr ) );
    REQUIRE_NOTHROW( runloop.launch_at( system_clock::now( ), nullptr, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch_at( system_clock::now( ), [ ]( void )
    {
        return error_code( );
    } ) );
    REQUIRE_NOTHROW( runloop.launch_at( system_clock::now( ), [ ]( void )
    {
        return error_code( );
    }, "test-key-value" ) );
}
