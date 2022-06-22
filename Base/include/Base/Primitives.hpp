/**
 * @file Primitives.hpp
 * @author Daniel Atanasov (daniel.a.atanasov97@gmail.com)
 * @brief
 * @version 0.1
 * @date 2021-12-10
 *
 * @copyright Copyright (c) 2021
 *
 */

// NOTE@Daniel:
//   Macro definitions are only to highlight `uint` the same as `int`

#define uint uint
using uint = unsigned int;

#define i8  i8
#define i16 i16
#define i32 i32
#define i64 i64
using i8 = char;
using i16 = short;
using i32 = int;
using i64 = long long;

#define u8  u8
#define u16 u16
#define u32 u32
#define u64 u64
using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

#define c8  c8
#define c16 c16
#define c32 c32
#define c64 c64
using c8 = i8;
using c16 = i16;
using c32 = i32;
using c64 = i64;

#define f32 f32
#define f64 f64
using f32 = float;
using f64 = double;

#define byte byte
using byte = u8;
