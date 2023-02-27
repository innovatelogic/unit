#pragma once

/*
#ifdef _MSC_VER
#define ES_MSVC
#elif defined(__ghs__)
#define ES_GHSC
#elif __GNUC__ || __SNC__
#define ES_GNUC
#elif
#error "Unknown compiler"
#endif
*/

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
