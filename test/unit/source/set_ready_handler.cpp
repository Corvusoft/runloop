//System Includes
#include <functional>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::function;
using std::error_code;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Alter ready handler." )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.set_ready_handler( nullptr ) );
    REQUIRE_NOTHROW( runloop.set_ready_handler( [ ]( void )
    {
        FAIL( "Runloop should not invoke ready handler when calling setter." );
        return error_code( );
    } ) );
}
