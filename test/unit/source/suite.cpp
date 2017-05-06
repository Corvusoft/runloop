//System Includes
#include <regex>
#include <string>
#include <chrono>
#include <limits>
#include <functional>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::regex;
using std::string;
using std::function;
using std::error_code;
using std::numeric_limits;
using std::chrono::time_point;
using std::chrono::system_clock;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Setup", "[runloop::setup]" )
{
    const RunLoop runloop;
    REQUIRE( runloop.is_stopped( ) == true );
    REQUIRE( runloop.is_suspended( ) == false );
}

TEST_CASE( "Clean-up", "[runloop::clean-up]" )
{
    const auto runloop = new RunLoop;
    REQUIRE_NOTHROW( delete runloop );
}

TEST_CASE( "Call wait with inactive loop", "[runloop::wait]" )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.wait( ) );
}

TEST_CASE( "Call stop with inactive loop", "[runloop::stop]" )
{
    RunLoop runloop;
    auto error = runloop.stop( );
    REQUIRE( error == error_code( ) );
    REQUIRE( runloop.is_stopped( ) == true );
    REQUIRE( runloop.is_suspended( ) == false );
}

TEST_CASE( "Call suspend with inactive loop", "[runloop::suspend]" )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.suspend( ) );
    REQUIRE( runloop.is_stopped( ) == true );
    REQUIRE( runloop.is_suspended( ) == false );
}

TEST_CASE( "Call resume with inactive loop", "[runloop::resume]" )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.resume( ) );
    REQUIRE( runloop.is_stopped( ) == true );
    REQUIRE( runloop.is_suspended( ) == false );
}

TEST_CASE( "Call cancel with inactive loop", "[runloop::cancel]" )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.cancel( ) );
}

TEST_CASE( "Call cancel with unknown task key on inactive loop", "[runloop::cancel]" )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.cancel( "123456" ) );
}

TEST_CASE( "Call cancel with known task key on inactive loop", "[runloop::cancel]" )
{
    RunLoop runloop;
    runloop.launch( [ ]( void )
    {
        return error_code( );
    }, "test-key-value" );
    
    REQUIRE_NOTHROW( runloop.cancel( "test-key-value" ) );
}

TEST_CASE( "Call cancel with unknown task key pattern on inactive loop", "[runloop::cancel]" )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.cancel( regex( "123456" ) ) );
}

TEST_CASE( "Call cancel with known task key pattern on inactive loop", "[runloop::cancel]" )
{
    RunLoop runloop;
    runloop.launch( [ ]( void )
    {
        return error_code( );
    }, "test-key-value" );
    
    REQUIRE_NOTHROW( runloop.cancel( regex( "^test\\-key\\-value$" ) ) );
}

TEST_CASE( "Launch a task with inactive loop", "[runloop::launch]" )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.launch( nullptr ) );
    REQUIRE_NOTHROW( runloop.launch( nullptr, "test-key-value" ) );
    REQUIRE_NOTHROW( runloop.launch( [ ]( void )
    {
        return error_code( );
    } ) );
    REQUIRE_NOTHROW( runloop.launch( [ ]( void )
    {
        return error_code( );
    }, "test-key-value" ) );
}

TEST_CASE( "Launch task at timestamp with inactive loop", "[runloop::launch-at]" )
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

TEST_CASE( "Launch task on event with inactive loop", "[runloop::launch-if]" )
{
    const std::function< error_code ( void ) > empty = nullptr;
    const std::function< error_code ( void ) > task = [ ]( void )
    {
        return error_code( );
    };
    const std::function< error_code ( void ) > event = [ ]( void )
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

TEST_CASE( "Alter worker limit", "[runloop::worker-limit]" )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.set_worker_limit( numeric_limits< unsigned int >::min( ) ) );
    REQUIRE_NOTHROW( runloop.set_worker_limit( numeric_limits< unsigned int >::max( ) / 2 ) );
    REQUIRE_NOTHROW( runloop.set_worker_limit( numeric_limits< unsigned int >::max( ) ) );
}

TEST_CASE( "Alter ready handler", "[runloop::ready-handler]" )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.set_ready_handler( nullptr ) );
    REQUIRE_NOTHROW( runloop.set_ready_handler( [ ]( void )
    {
        return error_code( );
    } ) );
}

TEST_CASE( "Alter error handler", "[runloop::error-handler]" )
{
    RunLoop runloop;
    REQUIRE_NOTHROW( runloop.set_error_handler( nullptr ) );
    REQUIRE_NOTHROW( runloop.set_error_handler( [ ]( const string&, const error_code&, const string& )
    {
        return error_code( );
    } ) );
}
