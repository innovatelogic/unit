#pragma once

// define MIPL_TEST_COMPILE if you want use some piece of mipl sources in your test project
#if !defined(UNIT_TEST_COMPILE) && defined(_WIN32)
    #ifdef UNIT_CORE_LIB_COMPILE
        #define UNIT_CORE_EXPORT _declspec(dllexport)
    #else
        #define UNIT_CORE_EXPORT _declspec(dllimport)
    #endif

    #pragma warning(disable : 4251)

#else
    #define UNIT_CORE_EXPORT
#if defined(__APPLE__)
    #define POSIX_PLATFORM
#endif
#endif
