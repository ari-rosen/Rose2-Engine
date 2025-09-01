#pragma once

#ifdef _WIN32
    #ifdef ROSE_EXPORTS
        #define ROSE_V2_API __declspec(dllexport)
    #else
        #define ROSE_V2_API __declspec(dllimport)
    #endif
#else
    #ifdef ROSE_EXPORTS
        #define ROSE_V2_API __attribute__((visibility("default")))
    #else
        #define ROSE_V2_API
    #endif
#endif
