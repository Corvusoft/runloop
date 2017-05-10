/*
 * Copyright 2013-2017, Corvusoft Ltd, All Rights Reserved.
 */

#ifndef _CORVUSOFT_CORE_DETAIL_TASK_IMPL_H
#define _CORVUSOFT_CORE_DETAIL_TASK_IMPL_H 1

//System Includes
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
            struct
            {
                std::string message = "";
                
                std::error_code code { };
            } error { };
            
            std::string key = "";
            
            std::function< std::error_code ( void ) > condition = nullptr;
            
            std::function< std::error_code ( void ) > operation = nullptr;
        };
    }
}

#endif  /* _CORVUSOFT_CORE_DETAIL_TASK_IMPL_H */
