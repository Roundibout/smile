#pragma once

#include <cstdint>

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using f32 = float;
using f64 = double;

constexpr i8 I8_MAX = INT8_MAX;
constexpr i16 I16_MAX = INT16_MAX;
constexpr i32 I32_MAX = INT32_MAX;
constexpr i64 I64_MAX = INT64_MAX;

constexpr i8 I8_MIN = INT8_MIN;
constexpr i16 I16_MIN = INT16_MIN;
constexpr i32 I32_MIN = INT32_MIN;
constexpr i64 I64_MIN = INT64_MIN;

constexpr u8 U8_MAX = UINT8_MAX;
constexpr u16 U16_MAX = UINT16_MAX;
constexpr u32 U32_MAX = UINT32_MAX;
constexpr u64 U64_MAX = UINT64_MAX;

constexpr f32 PI = 3.1415927f;
constexpr f32 ROOT2 = 1.41421356f;