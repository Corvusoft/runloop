/*
 * Copyright 2016-2017, Corvusoft Ltd, All Rights Reserved.
 */

//System Includes
#include <mutex>
#include <vector>
#include <thread>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <condition_variable>

//Project Includes
#include "corvusoft/core/error.hpp"
#include "corvusoft/core/run_loop.hpp"
#include "corvusoft/core/detail/task_impl.hpp"
#include "corvusoft/core/detail/run_loop_impl.hpp"

//External Includes

//System Namespaces
using std::regex;
using std::regex_match;
using std::thread;
using std::vector;
using std::string;
using std::function;
using std::exception;
using std::error_code;
using std::make_error_code;
using std::find_if;
using std::remove_if;
using std::mutex;
using std::unique_ptr;
using std::unique_lock;
using std::condition_variable;
using std::chrono::time_point;
using std::chrono::system_clock;
using std::chrono::milliseconds;

//Project Namespaces
using corvusoft::core::detail::RunLoopImpl;

//External Namespaces

namespace corvusoft
{
    namespace core
    {
        RunLoop::RunLoop( void ) : m_pimpl( new RunLoopImpl )
        {
            return;
        }
        
        RunLoop::~RunLoop( void )
        {
            stop( );
        }
        
        bool RunLoop::is_stopped( void ) const
        {
            return m_pimpl->is_stopped;
        }
        
        bool RunLoop::is_suspended( void ) const
        {
            return m_pimpl->is_suspended;
        }
        
        void RunLoop::resume( void )
        {
            m_pimpl->is_suspended = false;
            m_pimpl->pending_work.notify_all( );
        }
        
        void RunLoop::suspend( void )
        {
            if ( is_stopped( ) or is_suspended( ) ) return;
            m_pimpl->is_suspended = true;
        }
        
        error_code RunLoop::stop( void )
        {
            m_pimpl->is_stopped = true;
            m_pimpl->is_suspended = false;
            m_pimpl->pending_work.notify_all( );
            
            return success;
        }
        
        error_code RunLoop::start( void )
        {
            if ( is_suspended( ) ) return make_error_code( std::errc::operation_would_block );
            if ( is_stopped( ) == false ) return make_error_code( std::errc::operation_in_progress );
            
            m_pimpl->is_stopped = false;
            m_pimpl->is_suspended = false;
            
            unique_lock< mutex > guard( m_pimpl->task_lock );
            for ( unsigned int worker_count = 1; worker_count < m_pimpl->worker_limit; worker_count++ )
                m_pimpl->workers.emplace_back( thread( m_pimpl->dispatch ) );
                
            guard.unlock( );
            m_pimpl->pending_work.notify_one( );
            std::this_thread::yield( );
            
            error_code status = success;
            if ( m_pimpl->ready_handler not_eq nullptr ) try
                {
                    status = m_pimpl->ready_handler( );
                }
                catch ( const exception& ex )
                {
                    status = make_error_code( std::errc::operation_canceled );
                    m_pimpl->error( status, "std::exception raised when calling ready handler: " + std::string( ex.what( ) ) );
                }
                catch ( ... )
                {
                    status = make_error_code( std::errc::operation_canceled );
                    m_pimpl->error( status, "non-std::exception raised when calling ready handler." );
                }
                
            if ( status == success ) m_pimpl->dispatch( );
            else
            {
                m_pimpl->error( status, "ready handler execution returned an unsuccessful error condition." );
                stop( );
            }
            
            try
            {
                for ( auto& worker : m_pimpl->workers )
                    if ( worker.joinable( ) ) worker.join( );
            }
            catch ( const std::system_error& se )
            {
                m_pimpl->log( "failed graceful halt on corvusoft::core::runloop: " + string( se.what( ) ), se.code( ) );
                status = se.code( );
            }
            
            m_pimpl->tasks.clear( );
            m_pimpl->workers.clear( );
            
            return status;
        }
        
        error_code RunLoop::wait( const milliseconds& duration )
        {
            if ( is_stopped( ) ) return success;
            if ( is_suspended( ) ) return make_error_code( std::errc::operation_would_block );
            unique_lock< mutex > guard( m_pimpl->task_lock );
            
            if ( duration not_eq milliseconds::min( ) )
            {
                const auto timeout = system_clock::now( ) + duration;
                m_pimpl->pending_work.wait( guard, [ this, timeout ]
                {
                    return m_pimpl->tasks.empty( ) or m_pimpl->is_stopped or timeout <= system_clock::now( );
                } );
            }
            else
                m_pimpl->pending_work.wait( guard, [ this ] { return m_pimpl->tasks.empty( ) or m_pimpl->is_stopped; } );
                
            if ( guard.owns_lock( ) ) guard.unlock( );
            m_pimpl->pending_work.notify_one( );
            return success;
        }
        
        void RunLoop::cancel( const string& key )
        {
            unique_lock< mutex > guard( m_pimpl->task_lock );
            
            if ( key.empty( ) ) m_pimpl->tasks.clear( );
            else
            {
                const auto position = find_if( m_pimpl->tasks.begin( ), m_pimpl->tasks.end( ),
                                               [ &key ]( const TaskImpl & task )
                {
                    return task.key == key;
                } );
                
                if ( position not_eq m_pimpl->tasks.end( ) ) m_pimpl->tasks.erase( position );
            }
            
            if ( guard.owns_lock( ) ) guard.unlock( );
            m_pimpl->pending_work.notify_one( );
        }
        
        void RunLoop::cancel( const regex& key_pattern )
        {
            unique_lock< mutex > guard( m_pimpl->task_lock );
            
            m_pimpl->tasks.erase(
                remove_if( m_pimpl->tasks.begin( ),
                           m_pimpl->tasks.end( ),
                           [ &key_pattern ]( const TaskImpl & task )
            {
                return regex_match( task.key, key_pattern );
            } ),
            m_pimpl->tasks.end( ) );
            
            if ( guard.owns_lock( ) ) guard.unlock( );
            m_pimpl->pending_work.notify_one( );
        }
        
        void RunLoop::launch( const function< error_code ( void ) >& task, const string& key )
        {
            static const std::function< std::error_code ( void ) > event = nullptr;
            launch_if( event, task, key );
        }
        
        void RunLoop::launch_on( const signal_t value, const function< error_code ( void ) >& task, const string& key )
        {
            if ( task == nullptr )
            {
                signal( value, SIG_DFL );
                return;
            }
            
            signal( value, detail::RunLoopImpl::signal_handler );
            
            const function< error_code ( void ) > event = [ value ]
            {
                signal_t expected = value;
                static const int desired = 0;
                const auto signal_raised = detail::raised_signal.compare_exchange_strong( expected, desired );
                return ( signal_raised ) ? success : make_error_code( std::errc::operation_not_permitted );
            };
            
            const function< error_code ( void ) > reaction = [ this, value, task, key ]
            {
                const auto status = task( );
                launch_on( value, task, key );
                return status;
            };
            
            launch_if( event, reaction, key );
        }
        
        void RunLoop::launch_if( const bool condition, const function< error_code ( void ) >& task, const string& key )
        {
            if ( condition == true ) launch( task, key );
        }
        
        void RunLoop::launch_if( const function< error_code ( void ) >& event, const function< error_code ( void ) >& task, const string& key )
        {
            if ( task == nullptr ) return;
            
            TaskImpl work;
            work.key = key;
            work.operation = task;
            work.condition = event;
            
            unique_lock< mutex > guard( m_pimpl->task_lock );
            m_pimpl->tasks.push_back( work );
            if ( guard.owns_lock( ) ) guard.unlock( );
            m_pimpl->pending_work.notify_one( );
        }
        
        void RunLoop::launch_at( const time_point< system_clock >& timestamp, const function< error_code ( void ) >& task, const string& key )
        {
            const function< error_code ( void ) > event = [ timestamp ] { return ( timestamp <= system_clock::now( ) ) ? error_code( ) : make_error_code( std::errc::operation_not_permitted ); };
            launch_if( event, task, key );
        }
        
        void RunLoop::set_worker_limit( const unsigned int value )
        {
            m_pimpl->worker_limit = value;
        }
        
        void RunLoop::set_ready_handler( const function< error_code ( void ) >& value )
        {
            m_pimpl->ready_handler = value;
        }
        
        void RunLoop::set_log_handler( const function< error_code ( const string&, const error_code&, const string& ) >& value )
        {
            suspend( );
            m_pimpl->log_handler = value;
            resume( );
        }
        
        void RunLoop::set_error_handler( const function< error_code ( const string&, const error_code&, const string& ) >& value )
        {
            suspend( );
            m_pimpl->error_handler = value;
            resume( );
        }
    }
}
