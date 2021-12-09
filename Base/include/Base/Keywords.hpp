#pragma once

#define internal static
#define global   static
#define constant static const

#define cast(type, value) ((type)(value))

#define not !
#define and &&
#define or  ||

#ifdef __cplusplus
#define null nullptr
#else
#define null cast(Pointer, 0)
#endif

#define true  cast(Bool, 1)
#define false cast(Bool, 0)

#ifdef offsetof
#undef offsetof
#endif

#define offsetof(type, member) cast(Uint64, &cast(type*, null)->member)

#ifdef __cplusplus
#define debug   if constexpr (CONFIG_DEBUG)
#define release if constexpr (not CONFIG_DEBUG)
#else
#define debug   if (CONFIG_DEBUG)
#define release if (not CONFIG_DEBUG)
#endif

#ifndef __cplusplus
#define auto __auto_type
#endif

#define then << iterate
#define reverse me() << iterate::trait<iterate::backward>{} << iterate
#define iter me() << iterate::trait<iterate::none>{} << [&]

#define macro inline __attribute__((always_inline))

#define implicit