//System Includes
#include <atomic>
#include <memory>
#include <string>
#include <system_error>

//Project Includes
#include "corvusoft/core/error.hpp"
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::atomic;
using std::string;
using std::error_code;
using std::make_shared;
using std::make_error_code;

//Project Namespaces
using corvusoft::core::success;
using corvusoft::core::RunLoop;

//External Namespaces

SCENARIO( "Launching tasks on a single thread", "[runloop::launch]" )
{
    GIVEN( "I have setup a new runloop" )
    {
        auto runloop = make_shared< RunLoop >( );
        runloop->set_worker_limit( 1 );
        
        WHEN( "I launch a task to increment a counter" )
        {
            int count = 0;
            const auto counter = [ &count ]( void )
            {
                count++;
                return success;
            };
            
            runloop->launch( counter );
            runloop->launch( counter );
            runloop->launch( counter );
            runloop->launch( [ runloop ]( void )
            {
                runloop->wait( );
                runloop->stop( );
                return success;
            } );
            error_code status = runloop->start( );
            
            THEN( "I should see the counter incremented" )
            {
                REQUIRE( count == 3 );
                REQUIRE( status == success );
            }
        }
    }
}

SCENARIO( "Launching tasks on multiple threads", "[runloop::launch]" )
{
    GIVEN( "I have setup a new runloop" )
    {
        auto runloop = make_shared< RunLoop >( );
        
        WHEN( "I launch a task to increment a counter" )
        {
            int count = 0;
            const auto counter = [ &count ]( void )
            {
                count++;
                return success;
            };
            
            runloop->launch( counter );
            runloop->launch( counter );
            runloop->launch( counter );
            runloop->launch( [ runloop ]( void )
            {
                runloop->wait( );
                runloop->stop( );
                return success;
            } );
            error_code status = runloop->start( );
            
            THEN( "I should see the counter incremented" )
            {
                REQUIRE( count == 3 );
                REQUIRE( status == success );
            }
        }
    }
}

SCENARIO( "Launching tasks within other tasks", "[runloop::launch]" )
{
    GIVEN( "I have setup a new runloop" )
    {
        auto runloop = make_shared< RunLoop >( );
        
        WHEN( "I launch a task to increment a counter from another task" )
        {
            atomic< int > count( 0 );
            const auto counter = [ &count ]( void )
            {
                count++;
                return success;
            };
            
            runloop->launch( counter );
            runloop->launch( counter );
            runloop->launch( counter );
            runloop->launch( [ runloop, &counter ]( void )
            {
                runloop->launch( counter );
                runloop->launch( counter );
                
                runloop->wait( );
                runloop->stop( );
                return success;
            } );
            error_code status = runloop->start( );
            
            THEN( "I should see the counter incremented" )
            {
                REQUIRE( count == 5 );
                REQUIRE( status == success );
            }
        }
    }
}

SCENARIO( "Returning errors from launched tasks", "[runloop::launch]" )
{
    GIVEN( "I have setup a new runloop with a error handler" )
    {
        bool error_handler_called = false;
        auto runloop = make_shared< RunLoop >( );
        
        WHEN( "I launch a task returning an error status" )
        {
            runloop->launch( [ ]( void )
            {
                return make_error_code( std::errc::value_too_large );
            }, "my-key" );
            runloop->set_error_handler( [ runloop, &error_handler_called ]( const string & key, const error_code & status, const string & message )
            {
                runloop->stop( );
                error_handler_called = true;
                REQUIRE( key == "my-key" );
                REQUIRE( not message.empty( ) );
                REQUIRE( status == std::errc::value_too_large );
                return success;
            } );
            error_code status = runloop->start( );
            
            THEN( "I should see the error handler called" )
            {
                REQUIRE( status == success );
                REQUIRE( error_handler_called == true );
            }
        }
    }
}

SCENARIO( "Throwing exceptions from launched tasks", "[runloop::launch]" )
{
    GIVEN( "I have setup a new runloop with a error handler" )
    {
        bool error_handler_called = false;
        auto runloop = make_shared< RunLoop >( );
        
        WHEN( "I launch a task throwing an exception" )
        {
            runloop->launch( [ ]( void )
            {
                throw "error";
                return success;
            }, "my-id" );
            runloop->set_error_handler( [ runloop, &error_handler_called ]( const string & key, const error_code & status, const string & message )
            {
                runloop->stop( );
                error_handler_called = true;
                REQUIRE( key == "my-id" );
                REQUIRE( not message.empty( ) );
                REQUIRE( status == std::errc::operation_canceled );
                return success;
            } );
            error_code status = runloop->start( );
            
            THEN( "I should see the error handler called" )
            {
                REQUIRE( status == success );
                REQUIRE( error_handler_called == true );
            }
        }
    }
}
