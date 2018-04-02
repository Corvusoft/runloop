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

TEST_CASE( "Cancel single task." )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->set_worker_limit( 1 );
    
    runloop->launch( [ &runloop ]( error_code )
    {
        runloop->cancel( "kill me, pleaseeee." );
        return error_code( );
    } );
    
    int task_called = 0;
    runloop->launch( [ &task_called ]( error_code )
    {
        task_called++;
        return error_code( );
    }, "kill me, pleaseeee." );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( task_called == 0 );
}

TEST_CASE( "Cancel multiple tasks." )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->set_worker_limit( 1 );
    
    runloop->launch( [ &runloop ]( error_code )
    {
        runloop->cancel( "queue" );
        runloop->cancel( "task 1" );
        return error_code( );
    } );
    
    int task_called = 0;
    runloop->launch( [ &task_called  ]( error_code )
    {
        task_called++;
        return error_code( );
    }, "task 1" );
    
    runloop->launch( [ &task_called  ]( error_code )
    {
        task_called++;
        return error_code( );
    }, "queue" );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( task_called == 0 );
}

TEST_CASE( "Cancel all tasks." )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->set_worker_limit( 1 );
    
    runloop->launch( [ &runloop ]( error_code )
    {
        runloop->cancel( );
        return error_code( );
    } );
    
    int task_called = 0;
    runloop->launch( [ &task_called  ]( error_code )
    {
        task_called++;
        return error_code( );
    } );
    
    runloop->launch( [ &task_called  ]( error_code )
    {
        task_called++;
        return error_code( );
    }, "task 1" );
    
    runloop->launch( [ &task_called  ]( error_code )
    {
        task_called++;
        return error_code( );
    }, "queue" );
    
    runloop->launch( [ &task_called  ]( error_code )
    {
        task_called++;
        return error_code( );
    }, "" );
    
    runloop->launch( [ &task_called ]( error_code )
    {
        task_called++;
        return error_code( );
    }, "kill me, pleaseeee." );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( task_called == 0 );
}

TEST_CASE( "Cancel single task by key pattern." )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->set_worker_limit( 1 );
    
    runloop->launch( [ &runloop ]( error_code )
    {
        runloop->cancel( regex( "^Job.*" ) );
        return error_code( );
    } );
    
    int task_called = 0;
    runloop->launch( [ &task_called ]( error_code )
    {
        task_called++;
        return error_code( );
    }, "Job: 1234" );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( task_called == 0 );
}

TEST_CASE( "Cancel multiple tasks by key pattern." )
{
    auto runloop = make_shared< RunLoop >( );
    runloop->set_worker_limit( 1 );
    
    runloop->launch( [ &runloop ]( error_code )
    {
        runloop->cancel( regex( "^Job.*" ) );
        return error_code( );
    } );
    
    int task_called = 0;
    runloop->launch( [ &task_called ]( error_code )
    {
        task_called++;
        return error_code( );
    }, "Job: 1" );
    
    runloop->launch( [ &task_called ]( error_code )
    {
        task_called++;
        return error_code( );
    }, "Job: 2" );
    
    runloop->start( );
    runloop->wait( );
    runloop->stop( );
    
    REQUIRE( task_called == 0 );
}
