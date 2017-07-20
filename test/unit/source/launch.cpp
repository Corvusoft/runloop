//System Includes
#include <string>
#include <memory>
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

TEST_CASE( "Launch a task with inactive loop" )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.launch( nullptr ) );
    REQUIRE_NOTHROW( runloop.launch( nullptr, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch( [ ]( void )
    {
        FAIL( "Runloop should not invoke task until start has been called." );
        return error_code( );
    } ) );
    REQUIRE_NOTHROW( runloop.launch( [ ]( void )
    {
        FAIL( "Runloop should not invoke task until start has been called." );
        return error_code( );
    }, "test-key-value" ) );
}
