#pragma once

#include "Log/Log.h" // Include for OL_ASSERT_MESSAGE_BOX

// This header is used to define macros


// Debug break (cross compiler)
#ifdef _MSC_VER
    #define OL_DEBUGBREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
    #define OL_DEBUGBREAK() raise(SIGTRAP)
#else
    #define OL_DEBUGBREAK() void(0)
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Assert and Verify                                                                                                //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef OL_DEBUG
#define OL_ENABLE_ASSERT
#endif

#ifdef OL_ENABLE_ASSERT
// Assert
    #ifdef __clang__
        #define OL_API_MESSAGE_ASSERT_INTERNAL(...) ::OllamaCore::Log::MessageAssert(OllamaCore::Log::Type::API, "Assertion failed", ##__VA_ARGS__)
        #define OL_MESSAGE_ASSERT_INTERNAL(...) ::OllamaCore::Log::MessageAssert(OllamaCore::Log::Type::App, "Assertion failed", ##__VA_ARGS__)
    #else
        #define OL_API_MESSAGE_ASSERT_INTERNAL(...) ::OllamaCore::Log::MessageAssert(OllamaCore::Log::Type::API, "Assertion failed" __VA_OPT__(,) __VA_ARGS__)
        #define OL_MESSAGE_ASSERT_INTERNAL(...) ::OllamaCore::Log::MessageAssert(OllamaCore::Log::Type::App, "Assertion failed" __VA_OPT__(,) __VA_ARGS__)
    #endif

    #define OL_API_ASSERT(condition, ...) {if(!(condition)) {OL_API_MESSAGE_ASSERT_INTERNAL(__VA_ARGS__); OL_DEBUGBREAK();}}
    #define OL_ASSERT(condition, ...) {if(!(condition)) {OL_MESSAGE_ASSERT_INTERNAL(__VA_ARGS__); OL_DEBUGBREAK();}}

#else
    #define OL_API_ASSERT(condition, ...)
    #define OL_ASSERT(condtion, ...)


#endif // Enable assert

#define OL_ENBALE_VERIFY

#ifdef OL_ENBALE_VERIFY
// Verify
    #ifdef __clang__
        #define OL_API_VERIFY_MESSAGE_INTERNAL(...) ::OllamaCore::Log::MessageAssert(OllamaCore::Log::Type::API, "Assertion failed", ##__VA_ARGS__)
        #define OL_VERIFY_MESSAGE_INTERNAL(...) ::OllamaCore::Log::MessageAssert(OllamaCore::Log::Type::App, "Assertion failed", ##__VA_ARGS__)
    #else
        #define OL_API_VERIFY_MESSAGE_INTERNAL(...) ::OllamaCore::Log::MessageAssert(OllamaCore::Log::Type::API, "Assertion failed" __VA_OPT__(,) __VA_ARGS__)
        #define OL_VERIFY_MESSAGE_INTERNAL(...) ::OllamaCore::Log::MessageAssert(OllamaCore::Log::Type::App, "Assertion failed" __VA_OPT__(,) __VA_ARGS__)
    #endif

    #define OL_API_VERIFY(condition, ...) {if(!(condition)) {OL_API_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); OL_DEBUGBREAK();}}
    #define OL_VERIFY(condition, ...) {if(!(condition)) {OL_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); OL_DEBUGBREAK();}}
#else
    #define OL_API_VERIFY(condition, ...)
    #define OL_VERIFY(condtion, ...)
#endif // Enable verify


// Custom assert for http request
#ifdef OL_ENABLE_ASSERT
    #define OL_HTTP_ASSERT(response, ...) {if(!(response.status_code == 200)) { OL_API_MESSAGE_ASSERT_INTERNAL(__VA_ARGS__); OL_DEBUGBREAK();}} // Assert in debug build
#elif defined(OL_RELEASE)
    #define OL_HTTP_ASSERT(response, ...) {if(!(response.status_code == 200)) { OL_API_MESSAGE_ASSERT_INTERNAL(__VA_ARGS__);}} // Show message box without breaking in release build
#endif