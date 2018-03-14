/*
 * Copyright 2013-2018, Corvusoft Ltd, All Rights Reserved.
 */

#ifndef _CORVUSOFT_CORE_DETAIL_TASK_IMPL_H
#define _CORVUSOFT_CORE_DETAIL_TASK_IMPL_H 1

//System Includes
#include <chrono>
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
    namespace core
    {
        struct TaskImpl
        {
            bool inflight = false;
            
            std::string key = "";
            
            std::function< std::error_code ( void ) > operation = nullptr;
            
            std::chrono::time_point< std::chrono::system_clock > timeout { };
        };
    }
}

#endif  /* _CORVUSOFT_CORE_DETAIL_TASK_IMPL_H */
