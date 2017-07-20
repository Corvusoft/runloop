//System Includes
#include <regex>
#include <memory>
#include <string>
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
using std::make_shared;

//Project Namespaces
using corvusoft::core::RunLoop;

//External Namespaces

TEST_CASE( "Cancel single task" )
{
    int task_called = 0;
    auto runloop = make_shared< RunLoop >( );
    runloop->set_worker_limit( 1 );
    
    runloop->launch( [ &runloop ]( void )
    {
        runloop->cancel( "kill me, pleaseeee." );
        runloop->wait( );
        runloop->stop( );
        return error_code( );
    } );
    
    runloop->launch( [ &runloop, &task_called ]( void )
    {
        task_called++;
        return error_code( );
    }, "kill me, pleaseeee." );
    
    error_code status = runloop->start( );
    
    REQUIRE( status == error_code( ) );
    REQUIRE( task_called == 0 );
}

TEST_CASE( "Cancel multiple tasks" )
{
    int task_called = 0;
    auto runloop = make_shared< RunLoop >( );
    runloop->set_worker_limit( 1 );
    
    runloop->launch( [ &runloop ]( void )
    {
        runloop->cancel( "queue" );
        runloop->cancel( "task 1" );
        runloop->wait( );
        runloop->stop( );
        return error_code( );
    } );
    
    runloop->launch( [ &runloop, &task_called  ]( void )
    {
        task_called++;
        return error_code( );
    }, "task 1" );
    
    runloop->launch( [ &runloop, &task_called  ]( void )
    {
        task_called++;
        return error_code( );
    }, "queue" );
    
    error_code status = runloop->start( );
    
    REQUIRE( status == error_code( ) );
    REQUIRE( task_called == 0 );
}

TEST_CASE( "Cancel all tasks" )
{
    int task_called = 0;
    auto runloop = make_shared< RunLoop >( );
    runloop->set_worker_limit( 1 );
    
    runloop->launch( [ &runloop ]( void )
    {
        runloop->cancel( );
        runloop->wait( );
        runloop->stop( );
        return error_code( );
    } );
    
    runloop->launch( [ &runloop, &task_called  ]( void )
    {
        task_called++;
        return error_code( );
    } );
    
    runloop->launch( [ &runloop, &task_called  ]( void )
    {
        task_called++;
        return error_code( );
    }, "task 1" );
    
    runloop->launch( [ &runloop, &task_called  ]( void )
    {
        task_called++;
        return error_code( );
    }, "queue" );
    
    runloop->launch( [ &runloop, &task_called  ]( void )
    {
        task_called++;
        return error_code( );
    }, "" );
    
    runloop->launch( [ &runloop, &task_called ]( void )
    {
        task_called++;
        return error_code( );
    }, "kill me, pleaseeee." );
    
    error_code status = runloop->start( );
    
    REQUIRE( status == error_code( ) );
    REQUIRE( task_called == 0 );
}

TEST_CASE( "Cancel single task by key pattern" )
{
    int task_called = 0;
    auto runloop = make_shared< RunLoop >( );
    runloop->set_worker_limit( 1 );
    
    runloop->launch( [ &runloop ]( void )
    {
        runloop->cancel( regex( "^Job.*" ) );
        runloop->wait( );
        runloop->stop( );
        return error_code( );
    } );
    
    runloop->launch( [ &runloop, &task_called ]( void )
    {
        task_called++;
        return error_code( );
    }, "Job: 1234" );
    
    error_code status = runloop->start( );
    
    REQUIRE( status == error_code( ) );
    REQUIRE( task_called == 0 );
}

TEST_CASE( "Cancel multiple tasks by key pattern" )
{
    int task_called = 0;
    auto runloop = make_shared< RunLoop >( );
    runloop->set_worker_limit( 1 );
    
    runloop->launch( [ &runloop ]( void )
    {
        runloop->cancel( regex( "^Job.*" ) );
        runloop->wait( );
        runloop->stop( );
        return error_code( );
    } );
    
    runloop->launch( [ &runloop, &task_called ]( void )
    {
        task_called++;
        return error_code( );
    }, "Job: 1" );
    
    runloop->launch( [ &runloop, &task_called ]( void )
    {
        task_called++;
        return error_code( );
    }, "Job: 2" );
    
    error_code status = runloop->start( );
    
    REQUIRE( status == error_code( ) );
    REQUIRE( task_called == 0 );
}
