/*
 * Copyright 2013-2018, Corvusoft Ltd, All Rights Reserved.
 */

#ifndef _CORVUSOFT_CORE_DETAIL_RUN_LOOP_IMPL_H
#define _CORVUSOFT_CORE_DETAIL_RUN_LOOP_IMPL_H 1

//System Includes
#include <map>
#include <mutex>
#include <string>
#include <chrono>
#include <atomic>
#include <vector>
#include <thread>
#include <memory>
#include <ciso646>
#include <csignal>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <system_error>
#include <condition_variable>

//Project Includes
#include "corvusoft/core/detail/task_impl.hpp"

//External Includes

//System Namespaces

//Project Namespaces

//External Namespaces

namespace corvusoft
{
    //Forward Declarations
    
    namespace core
    {
        //Forward Declarations
        
        namespace detail
        {
            //Forward Declarations
            
            static std::atomic< std::sig_atomic_t > raised_signal( 0 );
            
            struct RunLoopImpl
            {
                std::mutex task_lock { };
                
                std::condition_variable pending_work { };
                
                std::vector< TaskImpl > tasks { };
                
                std::vector< std::thread > workers { };
                
                std::atomic< bool > is_stopped { true };
                
                std::atomic< bool > is_suspended { false };
                
                std::map< int, TaskImpl > signal_handlers { };
                
                unsigned int worker_limit = std::thread::hardware_concurrency( );
                
                std::function< std::error_code ( void ) > ready_handler = nullptr;
                
                std::function< void ( const std::string&, const std::error_code&, const std::string& ) > error_handler = nullptr;
                
                std::function< std::error_code ( const std::string&, const std::error_code&, const std::string& ) > log_handler = nullptr;
                
                static void signal_handler( const int value )
                {
                    raised_signal = value;
                }
                
                void wait( const std::function< bool ( void ) >& predicate )
                {
                    std::unique_lock< std::mutex > guard( task_lock );
                    
                    do
                    {
                        pending_work.wait_for( guard, std::chrono::milliseconds( 100 ), predicate );
                    }
                    while ( predicate( ) == false );
                    
                    if ( guard.owns_lock( ) ) guard.unlock( );
                    pending_work.notify_one( );
                }
                
                const std::function< bool ( void ) > until_work_available = [ this ]( void )
                {
                    if ( is_suspended ) return false;
                    if ( is_stopped or raised_signal ) return true;
                    return std::any_of( tasks.begin( ), tasks.end( ), [ ]( auto task )
                    {
                        return not task.inflight and task.timeout <= std::chrono::system_clock::now( );
                    } );
                };
                
                const std::function< void ( void ) > dispatch = [ this ]( void )
                {
                    std::unique_lock< std::mutex > guard( task_lock );
                    
                    do
                    {
                        pending_work.wait_for( guard, std::chrono::milliseconds( 100 ), until_work_available );
                        
                        if ( is_stopped ) break;
                        if ( guard.owns_lock( ) and not is_suspended )
                        {
                            schedule_signal_handlers( );
                            
                            auto position = std::find_if( tasks.begin( ), tasks.end( ), [ ]( auto task )
                            {
                                return not task.inflight and task.timeout <= std::chrono::system_clock::now( );
                            } );
                            if ( position == tasks.end( ) ) continue;
                            
                            position->inflight = true;
                            auto task = *position;
                            
                            guard.unlock( );
                            pending_work.notify_one( );
                            
                            auto status = launch( task );
                            
                            guard.lock( );
                            position = std::find_if( tasks.begin( ), tasks.end( ), [ task ]( auto entry )
                            {
                                return entry.key == task.key;
                            } );
                            if ( position == tasks.end( ) ) continue;
                            
                            if ( status == std::errc::resource_unavailable_try_again )
                            {
                                position->inflight = false;
                                position->timeout = std::chrono::system_clock::now( );
                            }
                            else tasks.erase( position );
                        }
                    }
                    while ( is_stopped == false );
                    
                    if ( guard.owns_lock( ) ) guard.unlock( );
                    pending_work.notify_all( );
                };
                
                void schedule_signal_handlers( void )
                {
                    auto value = raised_signal.exchange( 0 );
                    if ( not value ) return;
                    
                    auto iterator = signal_handlers.find( value );
                    if ( iterator == signal_handlers.end( ) ) return;
                    
                    tasks.insert( tasks.begin( ), iterator->second );
                }
                
                std::error_code launch( TaskImpl& task )
                try
                {
                    auto status = task.operation( );
                    if ( status not_eq std::errc::resource_unavailable_try_again )
                        error( task.key, status, "task execution failed." );
                        
                    return status;
                }
                catch ( const std::exception& ex )
                {
                    auto status = std::error_code( );
                    error( task.key, status, ex.what( ) );
                    return status;
                }
                catch ( ... )
                {
                    auto status = std::error_code( );
                    error( task.key, status, "non-std::exception raised when calling task execution." );
                    return status;
                }
                
                void error( const std::string& key, const std::error_code& status, const std::string& message )
                try
                {
                    if ( error_handler == nullptr ) return;
                    error_handler( key, status, message );
                }
                catch ( const std::exception& ex )
                {
                    log( key, status, message );
                    log( key, status, "std::exception raised when calling error handler: " + std::string( ex.what( ) ) );
                }
                catch ( ... )
                {
                    log( key, status, message );
                    log( key, status, "non-std::exception raised when calling error handler." );
                }
                
                void log( const std::string& key, const std::error_code& code, const std::string& message )
                try
                {
                    if ( log_handler == nullptr ) return;
                    
                    auto status = log_handler( key, code, message );
                    if ( status )
                    {
                        fprintf( stderr, "[%i %s] %s", code.value( ), code.message( ).data( ), message.data( ) );
                        fprintf( stderr, "[%i %s] log handler failed with unsuccessful error state.", status.value( ), status.message( ).data( ) );
                    }
                }
                catch ( const std::exception& ex )
                {
                    fprintf( stderr, "[%i %s] %s", code.value( ), code.message( ).data( ), message.data( ) );
                    fprintf( stderr, "std::exception raised when attempting to log failure: %s", ex.what( ) );
                }
                catch ( ... )
                {
                    fprintf( stderr, "[%i %s] %s", code.value( ), code.message( ).data( ), message.data( ) );
                    fprintf( stderr, "non-std::exception raised when attempting to log failure." );
                }
            };
        }
    }
}

#endif  /* _CORVUSOFT_CORE_DETAIL_RUN_LOOP_IMPL_H */
