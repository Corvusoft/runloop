//System Includes
#include <memory>
#include <string>
#include <system_error>

//Project Includes
#include "corvusoft/core/run_loop.hpp"

//External Includes
#include <catch.hpp>

//System Namespaces
using std::string;
using std::error_code;
using std::make_shared;
using std::make_error_code;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

SCENARIO( "Ready handler" )
{
    GIVEN( "I have setup a new runloop with a ready handler" )
    {
        bool ready_handler_called = false;
        
        auto runloop = make_shared< RunLoop >( );
        runloop->set_ready_handler( [ runloop, &ready_handler_called ]( void )
        {
            ready_handler_called = true;
            REQUIRE( runloop->is_stopped( ) == false );
            REQUIRE( runloop->is_suspended( ) == false );
            runloop->stop( );
            return error_code( );
        } );
        
        WHEN( "I start the runloop" )
        {
            error_code status = runloop->start( );
            
            THEN( "I should see the ready handler called" )
            {
                REQUIRE( status == error_code( ) );
                REQUIRE( ready_handler_called == true );
            }
        }
    }
}

SCENARIO( "Returning errors from the ready handler" )
{
    GIVEN( "I have setup a new runloop with a error handler" )
    {
        auto runloop = make_shared< RunLoop >( );
        runloop->set_ready_handler( [ ]( void )
        {
            return make_error_code( std::errc::connection_aborted );
        } );
        
        bool error_handler_called = false;
        runloop->set_error_handler( [ &error_handler_called ]( const string & key, const error_code & code, const string & message )
        {
            error_handler_called = true;
            REQUIRE( key.empty( ) );
            REQUIRE( not message.empty( ) );
            REQUIRE( code == std::errc::connection_aborted );
            return error_code( );
        } );
        
        WHEN( "I start the runloop and return an error from the ready handler" )
        {
            error_code status = runloop->start( );
            
            THEN( "I should see the error handler called" )
            {
                REQUIRE( error_handler_called == true );
            }
            
            AND_THEN( "I should see runloop::start return an error code" )
            {
                REQUIRE( status == std::errc::connection_aborted );
            }
            
            AND_THEN( "I should see the runloop stop" )
            {
                REQUIRE( runloop->is_stopped( ) );
            }
        }
    }
}

SCENARIO( "Throwing exceptions from the ready handler" )
{
    GIVEN( "I have setup a new runloop with a error handler" )
    {
        auto runloop = make_shared< RunLoop >( );
        runloop->set_ready_handler( [ ]( void )
        {
            throw "error";
            return error_code( );
        } );
        
        bool error_handler_called = false;
        runloop->set_error_handler( [ &error_handler_called ]( const string & key, const error_code & code, const string & message )
        {
            error_handler_called = true;
            REQUIRE( key.empty( ) );
            REQUIRE( not message.empty( ) );
            REQUIRE( code == std::errc::operation_canceled );
            return error_code( );
        } );
        
        WHEN( "I start the runloop and throw an exception from the ready handler" )
        {
            error_code status = runloop->start( );
            
            THEN( "I should see the error handler called" )
            {
                REQUIRE( error_handler_called == true );
            }
            
            AND_THEN( "I should see runloop::start return an error code" )
            {
                REQUIRE( status == std::errc::operation_canceled );
            }
            
            AND_THEN( "I should see the runloop stop" )
            {
                REQUIRE( runloop->is_stopped( ) );
            }
        }
    }
}
