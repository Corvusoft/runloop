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

TEST_CASE( "Call cancel with known task key on inactive loop." )
{
    RunLoop runloop;
    runloop.launch( [ ]( error_code )
    {
        FAIL( "Runloop should not invoke task until start has been called." );
        return error_code( );
    }, "test-key-value" );
    
    REQUIRE_NOTHROW( runloop.cancel( "test-key-value" ) );
}

TEST_CASE( "Call cancel with known task key pattern on inactive loop." )
{
    RunLoop runloop;
    runloop.launch( [ ]( error_code )
    {
        FAIL( "Runloop should not invoke task until start has been called." );
        return error_code( );
    }, "test-key-value" );
    
    REQUIRE_NOTHROW( runloop.cancel( regex( "^test\\-key\\-value$" ) ) );
}
