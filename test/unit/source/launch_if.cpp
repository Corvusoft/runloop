//System Includes
#include <string>
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

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Launch task on event with inactive loop." )
{
    const function< error_code ( error_code ) > task = [ ]( error_code )
    {
        FAIL( "Runloop should not invoke task until start has been called." );
        return error_code( );
    };
    
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.launch_if(  true,  task ) );
    REQUIRE_NOTHROW( runloop.launch_if(  true, nullptr ) );
    REQUIRE_NOTHROW( runloop.launch_if( false, nullptr ) );
    REQUIRE_NOTHROW( runloop.launch_if( false,  task ) );
    REQUIRE_NOTHROW( runloop.launch_if(  true,  task, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch_if(  true, nullptr, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch_if( false, nullptr, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch_if( false,  task, "test-key-value" ) );
    
    int circuit_breaker = 23;
    REQUIRE_NOTHROW( runloop.launch_if(  true,  task, circuit_breaker ) );
    REQUIRE_NOTHROW( runloop.launch_if(  true, nullptr, circuit_breaker ) );
    REQUIRE_NOTHROW( runloop.launch_if( false, nullptr, circuit_breaker ) );
    REQUIRE_NOTHROW( runloop.launch_if( false,  task, circuit_breaker ) );
    REQUIRE_NOTHROW( runloop.launch_if(  true,  task, circuit_breaker, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch_if(  true, nullptr, circuit_breaker, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch_if( false, nullptr, circuit_breaker, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch_if( false,  task, circuit_breaker, "test-key-value" ) );
}
