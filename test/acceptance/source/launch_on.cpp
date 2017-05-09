//System Includes
#include <memory>
#include <string>
#include <csignal>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::raise;
using std::string;
using std::error_code;
using std::make_shared;
using std::make_error_code;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

SCENARIO( "Launching tasks on operating system signals", "[runloop::launch_on]" )
{
    GIVEN( "I have setup a new runloop with a SIGALRM handler" )
    {
        int sigalrm_handler_called = 0;
        
        auto runloop = make_shared< RunLoop >( );
        runloop->launch_on( SIGALRM, [ &sigalrm_handler_called, runloop ]( void )
        {
            if ( sigalrm_handler_called == 2 ) runloop->stop( );
            else sigalrm_handler_called++;
            
            runloop->launch( [ ]( void )
            {
                raise( SIGALRM );
                return error_code( );
            } );
            return error_code( );
        } );
        
        WHEN( "I raise a SIGALRM signal" )
        {
            runloop->launch( [ ]( void )
            {
                raise( SIGALRM );
                return error_code( );
            } );
            
            error_code status = runloop->start( );
            
            THEN( "I should see the SIGALRM signal handler called" )
            {
                REQUIRE( status == error_code( ) );
                REQUIRE( sigalrm_handler_called == 2 );
            }
        }
    }
}

SCENARIO( "Returning errors from signal handlers", "[runloop::launch_on]" )
{
    GIVEN( "I have setup a new runloop with a error handler" )
    {
        bool error_handler_called = false;
        
        auto runloop = make_shared< RunLoop >( );
        runloop->launch_on( SIGALRM, [ ]( void )
        {
            return make_error_code( std::errc::not_a_socket );
        }, "SIGALRM HANDLER" );
        runloop->set_error_handler( [ runloop, &error_handler_called ]( const string & key, const error_code & code, const string & message )
        {
            runloop->stop( );
            error_handler_called = true;
            REQUIRE( key == "SIGALRM HANDLER" );
            REQUIRE( code == std::errc::not_a_socket );
            REQUIRE( not message.empty( ) );
            return error_code( );
        } );
        
        WHEN( "I launch a task on SIGALRM returning an error status" )
        {
            runloop->set_ready_handler( [ ]( void )
            {
                raise( SIGALRM );
                return error_code( );
            } );
            error_code status = runloop->start( );
            
            THEN( "I should see the error handler called" )
            {
                REQUIRE( status == error_code( ) );
                REQUIRE( error_handler_called == true );
            }
        }
    }
}

SCENARIO( "Throwing exceptions from signal handlers", "[runloop::launch_on]" )
{
    GIVEN( "I have setup a new runloop with a error handler" )
    {
        bool error_handler_called = false;
        
        auto runloop = make_shared< RunLoop >( );
        runloop->launch_on( SIGALRM, [ ]( void )
        {
            throw "error";
            return error_code( );
        }, "SIGALRM HANDLER" );
        runloop->set_error_handler( [ runloop, &error_handler_called ]( const string & key, const error_code & code, const string & message )
        {
            runloop->stop( );
            error_handler_called = true;
            REQUIRE( key == "SIGALRM HANDLER" );
            REQUIRE( code == std::errc::operation_canceled );
            REQUIRE( not message.empty( ) );
            return error_code( );
        } );
        
        WHEN( "I launch a task on SIGALRM throwing an exception" )
        {
            runloop->set_ready_handler( [ ]( void )
            {
                raise( SIGALRM );
                return error_code( );
            } );
            error_code status = runloop->start( );
            
            THEN( "I should see the error handler called" )
            {
                REQUIRE( status == error_code( ) );
                REQUIRE( error_handler_called == true );
            }
        }
    }
}
