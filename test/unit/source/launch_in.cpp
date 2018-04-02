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
using std::chrono::milliseconds;
using std::chrono::system_clock;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Launch task in delay period with inactive loop." )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.launch_in( seconds( 1 ), nullptr ) );
    REQUIRE_NOTHROW( runloop.launch_in( milliseconds( 500 ), nullptr, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch_in( milliseconds( 200 ), [ ]( error_code )
    {
        FAIL( "Runloop should not invoke task until start has been called." );
        return error_code( );
    } ) );
    REQUIRE_NOTHROW( runloop.launch_in( seconds( 1 ), [ ]( error_code )
    {
        FAIL( "Runloop should not invoke task until start has been called." );
        return error_code( );
    }, "test-key-value" ) );
    
    int circuit_breaker = 1;
    REQUIRE_NOTHROW( runloop.launch_in( seconds( 1 ), [ ]( error_code )
    {
        FAIL( "Runloop should not invoke task until start has been called." );
        return error_code( );
    }, circuit_breaker ) );
    REQUIRE_NOTHROW( runloop.launch_in( seconds( 1 ), [ ]( error_code )
    {
        FAIL( "Runloop should not invoke task until start has been called." );
        return error_code( );
    }, circuit_breaker, "test-key-value" ) );
}
