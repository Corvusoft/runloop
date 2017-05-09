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

TEST_CASE( "Launch task on event with inactive loop", "[runloop::launch-if]" )
{
    const function< error_code ( void ) > empty = nullptr;
    const function< error_code ( void ) > task = [ ]( void )
    {
        return error_code( );
    };
    const function< error_code ( void ) > event = [ ]( void )
    {
        return error_code( );
    };
    
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.launch_if(  true,  task ) );
    REQUIRE_NOTHROW( runloop.launch_if(  true, empty ) );
    REQUIRE_NOTHROW( runloop.launch_if( false, empty ) );
    REQUIRE_NOTHROW( runloop.launch_if( false,  task ) );
    REQUIRE_NOTHROW( runloop.launch_if( empty, empty ) );
    REQUIRE_NOTHROW( runloop.launch_if( event, empty ) );
    REQUIRE_NOTHROW( runloop.launch_if( empty,  task ) );
    REQUIRE_NOTHROW( runloop.launch_if( event,  task ) );
    
    REQUIRE_NOTHROW( runloop.launch_if(  true,  task, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch_if(  true, empty, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch_if( false, empty, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch_if( false,  task, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch_if( empty, empty, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch_if( event, empty, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch_if( empty,  task, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch_if( event,  task, "test-key-value" ) );
}
