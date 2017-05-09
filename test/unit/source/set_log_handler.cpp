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

TEST_CASE( "Alter log handler", "[runloop::error-handler]" )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.set_log_handler( nullptr ) );
    REQUIRE_NOTHROW( runloop.set_log_handler( [ ]( const string&, const error_code&, const string& )
    {
        return error_code( );
    } ) );
}
