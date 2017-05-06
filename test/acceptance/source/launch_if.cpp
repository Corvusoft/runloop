//System Includes
#include <chrono>
#include <memory>
#include <string>
#include <functional>
#include <system_error>

//Project Includes
#include "corvusoft/core/error.hpp"
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::string;
using std::function;
using std::error_code;
using std::make_shared;
using std::make_error_code;
using std::chrono::milliseconds;

//Project Namespaces
using corvusoft::core::success;
using corvusoft::core::RunLoop;

//External Namespaces

SCENARIO( "Launching tasks when a condition is met", "[runloop::launch_if]" )
{
    GIVEN( "I have setup a new runloop" )
    {
        int task_called = 0;
        auto runloop = make_shared< RunLoop >( );
        
        WHEN( "I try to launch a task with a negative condition" )
        {
            runloop->launch_if( false, [ &task_called ]( void )
            {
                task_called++;
                return success;
            } );
            runloop->launch( [ &runloop ]( void )
            {
                runloop->wait( );
                runloop->stop( );
                return success;
            } );
            
            error_code status = runloop->start( );
            
            THEN( "I should not see the task called" )
            {
                REQUIRE( status == success );
                REQUIRE( task_called == 0 );
            }
        }
        
        WHEN( "I try to launch a task with a positive condition" )
        {
            runloop->launch_if( true, [ &task_called, runloop ]( void )
            {
                task_called++;
                runloop->stop( );
                return success;
            } );
            
            error_code status = runloop->start( );
            
            THEN( "I should see the task called" )
            {
                REQUIRE( status == success );
                REQUIRE( task_called == 1 );
            }
        }
    }
}

SCENARIO( "Launching tasks when an event occurs", "[runloop::launch_if]" )
{
    GIVEN( "I have setup a new runloop" )
    {
        auto runloop = make_shared< RunLoop >( );
        
        WHEN( "I launch a task with an event that fires positive" )
        {
            int task_called = 0;
            const function< error_code ( void ) > event = [ ]( void )
            {
                return success;
            };
            
            runloop->launch_if( event, [ &task_called, runloop ]( void )
            {
                task_called++;
                runloop->stop( );
                return success;
            } );
            error_code status = runloop->start( );
            
            THEN( "I should see the task called" )
            {
                REQUIRE( status == success );
                REQUIRE( task_called == 1 );
            }
        }
        
        WHEN( "I launch a task with an event thats fires negative" )
        {
            int task_called = 0;
            const function< error_code ( void ) > event = [ ]( void )
            {
                return make_error_code( std::errc::operation_not_permitted );
            };
            
            runloop->launch_if( event, [ &task_called ]( void )
            {
                task_called++;
                return success;
            } );
            runloop->launch( [ &runloop ]( void )
            {
                runloop->wait( milliseconds( 500 ) );
                runloop->stop( );
                return success;
            } );
            error_code status = runloop->start( );
            
            THEN( "I should not see the task called" )
            {
                REQUIRE( status == success );
                REQUIRE( task_called == 0 );
            }
        }
        
        WHEN( "I launch a task with an event thats cancels the task" )
        {
            int task_called = 0;
            const function< error_code ( void ) > event = [ ]( void )
            {
                return make_error_code( std::errc::operation_canceled );
            };
            
            runloop->launch_if( event, [ &task_called ]( void )
            {
                task_called++;
                return success;
            } );
            runloop->launch( [ &runloop ]( void )
            {
                runloop->wait( );
                runloop->stop( );
                return success;
            } );
            error_code status = runloop->start( );
            
            THEN( "I should not see the task called" )
            {
                REQUIRE( status == success );
                REQUIRE( task_called == 0 );
            }
        }
    }
}

SCENARIO( "Returning errors from task events", "[runloop::launch_if]" )
{
    GIVEN( "I have setup a new runloop with a error handler" )
    {
        bool error_handler_called = false;
        auto runloop = make_shared< RunLoop >( );
        runloop->set_error_handler( [ runloop, &error_handler_called ]( const string & key, const error_code & status, const string & message )
        {
            runloop->stop( );
            error_handler_called = true;
            REQUIRE( key.empty( ) );
            REQUIRE( not message.empty( ) );
            REQUIRE( status == std::errc::not_a_socket );
            return success;
        } );
        
        WHEN( "I launch a task with an event that returns an error" )
        {
            const function< error_code ( void ) > event = [ ]( void )
            {
                return make_error_code( std::errc::not_a_socket );
            };
            runloop->launch_if( event, [ ]( void )
            {
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

SCENARIO( "Throwing exceptions from task events", "[runloop::launch_if]" )
{
    GIVEN( "I have setup a new runloop with a error handler" )
    {
        bool error_handler_called = false;
        auto runloop = make_shared< RunLoop >( );
        runloop->set_error_handler( [ runloop, &error_handler_called ]( const string & key, const error_code & status, const string & message )
        {
            runloop->stop( );
            error_handler_called = true;
            REQUIRE( key.empty( ) );
            REQUIRE( not message.empty( ) );
            REQUIRE( status == std::errc::operation_canceled );
            return success;
        } );
        
        WHEN( "I launch a task with an event that throws an exception" )
        {
            const function< error_code ( void ) > event = [ ]( void )
            {
                throw "arrrr";
                return success;
            };
            runloop->launch_if( event, [ ]( void )
            {
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
