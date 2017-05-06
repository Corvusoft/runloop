//System Includes
#include <regex>
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
using std::regex;
using std::string;
using std::function;
using std::error_code;
using std::make_shared;

//Project Namespaces
using corvusoft::core::success;
using corvusoft::core::RunLoop;

//External Namespaces

SCENARIO( "Cancel single task", "[runloop::cancel]" )
{
    GIVEN( "I have setup a new runloop" )
    {
        int task_called = 0;
        auto runloop = make_shared< RunLoop >( );
        runloop->set_worker_limit( 1 );
        
        WHEN( "I cancel a task before it has run" )
        {
            runloop->launch( [ &runloop ]( void )
            {
                runloop->cancel( "kill me, pleaseeee." );
                runloop->wait( );
                runloop->stop( );
                return success;
            } );
            
            runloop->launch( [ &runloop, &task_called ]( void )
            {
                task_called++;
                return success;
            }, "kill me, pleaseeee." );
            
            error_code status = runloop->start( );
            
            THEN( "I should not see the task executed" )
            {
                REQUIRE( status == success );
                REQUIRE( task_called == 0 );
            }
        }
    }
}

SCENARIO( "Cancel multiple tasks", "[runloop::cancel]" )
{
    GIVEN( "I have setup a new runloop" )
    {
        int task_called = 0;
        auto runloop = make_shared< RunLoop >( );
        runloop->set_worker_limit( 1 );
        
        WHEN( "I cancel multiple tasks before they have run" )
        {
            runloop->launch( [ &runloop ]( void )
            {
                runloop->cancel( "queue" );
                runloop->cancel( "task 1" );
                runloop->wait( );
                runloop->stop( );
                return success;
            } );
            
            runloop->launch( [ &runloop, &task_called  ]( void )
            {
                task_called++;
                return success;
            }, "task 1" );
            
            runloop->launch( [ &runloop, &task_called  ]( void )
            {
                task_called++;
                return success;
            }, "queue" );
            
            error_code status = runloop->start( );
            
            THEN( "I should not see any tasks executed" )
            {
                REQUIRE( status == success );
                REQUIRE( task_called == 0 );
            }
        }
    }
}

SCENARIO( "Cancel all tasks", "[runloop::cancel]" )
{
    GIVEN( "I have setup a new runloop" )
    {
        int task_called = 0;
        auto runloop = make_shared< RunLoop >( );
        runloop->set_worker_limit( 1 );
        
        WHEN( "I cancel all tasks before they have run" )
        {
            runloop->launch( [ &runloop ]( void )
            {
                runloop->cancel( );
                runloop->wait( );
                runloop->stop( );
                return success;
            } );
            
            runloop->launch( [ &runloop, &task_called  ]( void )
            {
                task_called++;
                return success;
            } );
            
            runloop->launch( [ &runloop, &task_called  ]( void )
            {
                task_called++;
                return success;
            }, "task 1" );
            
            runloop->launch( [ &runloop, &task_called  ]( void )
            {
                task_called++;
                return success;
            }, "queue" );
            
            runloop->launch( [ &runloop, &task_called  ]( void )
            {
                task_called++;
                return success;
            }, "" );
            
            runloop->launch( [ &runloop, &task_called ]( void )
            {
                task_called++;
                return success;
            }, "kill me, pleaseeee." );
            
            error_code status = runloop->start( );
            
            THEN( "I should not see any tasks executed" )
            {
                REQUIRE( status == success );
                REQUIRE( task_called == 0 );
            }
        }
    }
}

SCENARIO( "Cancel single task by key pattern", "[runloop::cancel]" )
{
    GIVEN( "I have setup a new runloop" )
    {
        int task_called = 0;
        auto runloop = make_shared< RunLoop >( );
        runloop->set_worker_limit( 1 );
        
        WHEN( "I cancel a task matching a key pattern before it has run" )
        {
            runloop->launch( [ &runloop ]( void )
            {
                runloop->cancel( regex( "^Job.*" ) );
                runloop->wait( );
                runloop->stop( );
                return success;
            } );
            
            runloop->launch( [ &runloop, &task_called ]( void )
            {
                task_called++;
                return success;
            }, "Job: 1234" );
            
            error_code status = runloop->start( );
            
            THEN( "I should not see the task executed" )
            {
                REQUIRE( status == success );
                REQUIRE( task_called == 0 );
            }
        }
    }
}

SCENARIO( "Cancel multiple tasks by key pattern", "[runloop::cancel]" )
{
    GIVEN( "I have setup a new runloop" )
    {
        int task_called = 0;
        auto runloop = make_shared< RunLoop >( );
        runloop->set_worker_limit( 1 );
        
        WHEN( "I cancel a task matching a key pattern before it has run" )
        {
            runloop->launch( [ &runloop ]( void )
            {
                runloop->cancel( regex( "^Job.*" ) );
                runloop->wait( );
                runloop->stop( );
                return success;
            } );
            
            runloop->launch( [ &runloop, &task_called ]( void )
            {
                task_called++;
                return success;
            }, "Job: 1" );
            
            runloop->launch( [ &runloop, &task_called ]( void )
            {
                task_called++;
                return success;
            }, "Job: 2" );
            
            error_code status = runloop->start( );
            
            THEN( "I should not see the task executed" )
            {
                REQUIRE( status == success );
                REQUIRE( task_called == 0 );
            }
        }
    }
}
