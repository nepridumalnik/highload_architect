#pragma once

#if defined(_MSC_VER)
//  Microsoft
#define EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define EXPORT __attribute__((visibility("default")))
#else
#define EXPORT
#pragma warning Unknown dynamic link import / export semantics.
#endif
