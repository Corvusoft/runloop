/*
 * Copyright 2013-2018, Corvusoft Ltd, All Rights Reserved.
 */
#pragma once

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
            int breaker = -1;
            
            bool inflight = false;
            
            std::string key = "";
            
            std::chrono::time_point< std::chrono::system_clock > timeout { };
            
            std::function< std::error_code ( std::error_code ) > operation = nullptr;
        };
    }
}
