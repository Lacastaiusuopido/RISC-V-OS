// types.h - 基本类型定义

#ifndef _TYPES_H_
#define _TYPES_H_

// 无符号整数类型
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

// 有符号整数类型
typedef signed char        int8;
typedef signed short       int16;
typedef signed int         int32;
typedef signed long long   int64;

// 无符号整数类型（标准C类型）
typedef unsigned char      uchar;
typedef unsigned short     ushort;
typedef unsigned int       uint;

// 大小类型
typedef uint64             size_t;
typedef int64              ssize_t;

// 空指针
#define NULL ((void*)0)

// 布尔类型
typedef int                bool;
#define true               1
#define false              0

// 可变参数支持
typedef __builtin_va_list  va_list;
#define va_start(v,l)      __builtin_va_start(v,l)
#define va_end(v)          __builtin_va_end(v)
#define va_arg(v,l)        __builtin_va_arg(v,l)
#define va_copy(d,s)       __builtin_va_copy(d,s)

#endif // _TYPES_H_