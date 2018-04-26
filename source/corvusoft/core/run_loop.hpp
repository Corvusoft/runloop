/*
 * Copyright 2013-2018, Corvusoft Ltd, All Rights Reserved.
 */

#pragma once

//System Includes
#include <regex>
#include <chrono>
#include <memory>
#include <string>
#include <functional>
#include <system_error>

//Project Includes

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
            struct RunLoopImpl;
        }
        
        class RunLoop
        {
            public:
                //Friends
                
                //Definitions
                typedef int signal_t;
                
                //Constructors
                RunLoop( void );
                
                virtual ~RunLoop( void );
                
                //Functionality
                bool is_stopped( void ) const;
                
                bool is_suspended( void ) const;
                
                void resume( void );
                
                void suspend( void );
                
                void stop( void );
                
                void start( void );
                
                void cancel( const std::string& key = "" );
                
                void cancel( const std::regex& key_pattern );
                
                void wait( const std::string& key = "" );
                
                void wait( const std::regex& key_pattern );
                
                void wait( const std::chrono::milliseconds& duration );
                
                void launch( const std::function< std::error_code ( const std::error_code ) >& task, const std::string& key = "" );
                
                void launch( const std::function< std::error_code ( const std::error_code ) >& task, const int breaker, const std::string& key = "" );
                
                void launch_if( const bool condition, const std::function< std::error_code ( const std::error_code ) >& task, const std::string& key = "" );
                
                void launch_if( const bool condition, const std::function< std::error_code ( const std::error_code ) >& task, const int breaker, const std::string& key = "" );
                
                void launch_on( const signal_t value, const std::function< std::error_code ( const std::error_code ) >& task, const std::string& key = "" );
                
                void launch_on( const signal_t value, const std::function< std::error_code ( const std::error_code ) >& task, const int breaker, const std::string& key = "" );
                
                void launch_in( const std::chrono::milliseconds& delay, const std::function< std::error_code ( const std::error_code ) >& task, const std::string& key = "" );
                
                void launch_in( const std::chrono::milliseconds& delay, const std::function< std::error_code ( const std::error_code ) >& task, const int breaker, const std::string& key = "" );
                
                void launch_at( const std::chrono::time_point< std::chrono::system_clock >& timestamp, const std::function< std::error_code ( const std::error_code ) >& task, const std::string& key = "" );
                
                void launch_at( const std::chrono::time_point< std::chrono::system_clock >& timestamp, const std::function< std::error_code ( const std::error_code ) >& task, const int breaker, const std::string& key = "" );
                
                //Getters
                
                //Setters
                void set_default_breaker( const int value );
                
                void set_worker_limit( const unsigned int value );
                
                void set_ready_handler( const std::function< std::error_code ( const std::error_code ) >& value );
                
                void set_error_handler( const std::function< void ( const std::string&, const std::error_code&, const std::string& ) >& value );
                
                void set_log_handler( const std::function< std::error_code ( const std::string&, const std::error_code&, const std::string& ) >& value );
                
                //Operators
                
                //Properties
                
            protected:
                //Friends
                
                //Definitions
                
                //Constructors
                
                //Functionality
                
                //Getters
                
                //Setters
                
                //Operators
                
                //Properties
                
            private:
                //Friends
                
                //Definitions
                
                //Constructors
                RunLoop( const RunLoop& original ) = delete;
                
                //Functionality
                
                //Getters
                
                //Setters
                
                //Operators
                RunLoop& operator =( const RunLoop& value ) = delete;
                
                //Properties
                std::unique_ptr< detail::RunLoopImpl > m_pimpl;
        };
    }
}
