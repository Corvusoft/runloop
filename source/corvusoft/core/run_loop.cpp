/*
 * Copyright 2013-2018, Corvusoft Ltd, All Rights Reserved.
 */

//System Includes
#include <mutex>
#include <vector>
#include <thread>
#include <algorithm>
#include <stdexcept>
#include <condition_variable>

//Project Includes
#include "corvusoft/core/run_loop.hpp"
#include "corvusoft/core/detail/task_impl.hpp"
#include "corvusoft/core/detail/run_loop_impl.hpp"

//External Includes

//System Namespaces
using std::mutex;
using std::regex;
using std::thread;
using std::vector;
using std::string;
using std::any_of;
using std::function;
using std::remove_if;
using std::error_code;
using std::regex_match;
using std::unique_lock;
using std::condition_variable;
using std::chrono::time_point;
using std::chrono::system_clock;
using std::chrono::milliseconds;
using std::regex_constants::icase;

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
            m_pimpl->is_suspended = true;
        }
        
        void RunLoop::stop( void )
        {
            m_pimpl->is_stopped = true;
            m_pimpl->is_suspended = false;
            m_pimpl->pending_work.notify_all( );
            
            try
            {
                for ( auto& worker : m_pimpl->workers )
                    if ( worker.joinable( ) ) worker.join( );
            }
            catch ( const std::system_error& se )
            {
                m_pimpl->log( "corvusoft::core::runloop::stop", se.code( ), "failed graceful halt on corvusoft::core::runloop: " + string( se.what( ) ) );
            }
            
            m_pimpl->tasks.clear( );
            m_pimpl->workers.clear( );
        }
        
        void RunLoop::start( void )
        {
            if ( is_suspended( ) or is_stopped( ) == false ) return;
            
            m_pimpl->is_stopped = false;
            m_pimpl->is_suspended = false;
            
            launch( m_pimpl->ready_handler, "corvusoft::core::runloop::ready_handler" );
            
            unique_lock< mutex > guard( m_pimpl->task_lock );
            for ( unsigned int worker_count = 0; worker_count < m_pimpl->worker_limit; worker_count++ )
                m_pimpl->workers.emplace_back( thread( m_pimpl->dispatch ) );
            guard.unlock( );
            m_pimpl->pending_work.notify_one( );
        }
        
        void RunLoop::cancel( const string& key )
        {
            if ( key.empty( ) )
                cancel( regex( ".*", icase ) );
            else
                cancel( regex( "^" + key + "$", icase ) );
        }
        
        void RunLoop::cancel( const regex& key_pattern )
        {
            unique_lock< mutex > guard( m_pimpl->task_lock );
            m_pimpl->tasks.erase( remove_if( m_pimpl->tasks.begin( ), m_pimpl->tasks.end( ), [ key_pattern ]( auto task )
            {
                return regex_match( task.key, key_pattern );
            } ), m_pimpl->tasks.end( ) );
            
            if ( guard.owns_lock( ) ) guard.unlock( );
            m_pimpl->pending_work.notify_one( );
        }
        
        void RunLoop::wait( const string& key )
        {
            if ( key.empty( ) )
                wait( regex( ".*", icase ) );
            else
                wait( regex( "^" + key + "$", icase ) );
        }
        
        void RunLoop::wait( const regex& key_pattern )
        {
            unique_lock< mutex > guard( m_pimpl->task_lock );
            m_pimpl->pending_work.wait( guard, [ this, key_pattern ]
            {
                const auto found = any_of( m_pimpl->tasks.begin( ), m_pimpl->tasks.end( ), [ key_pattern ]( auto task )
                {
                    return regex_match( task.key, key_pattern );
                } );
                return not found or m_pimpl->is_stopped;
            } );
            
            if ( guard.owns_lock( ) ) guard.unlock( );
            m_pimpl->pending_work.notify_one( );
        }
        
        void RunLoop::wait( const milliseconds& duration )
        {
            const auto timeout = system_clock::now( ) + duration;
            
            unique_lock< mutex > guard( m_pimpl->task_lock );
            m_pimpl->pending_work.wait( guard, [ this, timeout ]
            {
                return m_pimpl->tasks.empty( ) or m_pimpl->is_stopped or timeout <= system_clock::now( );
            } );
            
            if ( guard.owns_lock( ) ) guard.unlock( );
            m_pimpl->pending_work.notify_one( );
        }
        
        void RunLoop::launch( const function< error_code ( void ) >& task, const string& key )
        {
            launch_at( system_clock::now( ), task, key );
        }
        
        void RunLoop::launch_if( const bool condition, const function< error_code ( void ) >& task, const string& key )
        {
            if ( condition ) launch( task, key );
        }
        
        void RunLoop::launch_on( const signal_t value, const function< error_code ( void ) >& task, const string& key )
        {
            TaskImpl work;
            work.key = key;
            work.operation = task;
            m_pimpl->signal_handlers[ value ] = work;
            
            if ( task == nullptr )
                signal( value, SIG_DFL );
            else
                signal( value, detail::RunLoopImpl::signal_handler );
        }
        
        void RunLoop::launch_in( const milliseconds& delay, const function< error_code ( void ) >& task, const string& key )
        {
            launch_at( system_clock::now( ) + delay, task, key );
        }
        
        void RunLoop::launch_at( const time_point< system_clock >& timestamp, const function< error_code ( void ) >& task, const string& key )
        {
            if ( task == nullptr ) return;
            
            TaskImpl work;
            work.key = key;
            work.operation = task;
            work.timeout = timestamp;
            
            unique_lock< mutex > guard( m_pimpl->task_lock );
            m_pimpl->tasks.push_back( work );
            if ( guard.owns_lock( ) ) guard.unlock( );
            m_pimpl->pending_work.notify_one( );
        }
        
        void RunLoop::set_worker_limit( const unsigned int value )
        {
            m_pimpl->worker_limit = value;
        }
        
        void RunLoop::set_ready_handler( const function< error_code ( void ) >& value )
        {
            m_pimpl->ready_handler = value;
        }
        
        void RunLoop::set_error_handler( const function< void ( const string&, const error_code&, const string& ) >& value )
        {
            m_pimpl->error_handler = value;
        }
        
        void RunLoop::set_log_handler( const function< error_code ( const string&, const error_code&, const string& ) >& value )
        {
            m_pimpl->log_handler = value;
        }
    }
}
