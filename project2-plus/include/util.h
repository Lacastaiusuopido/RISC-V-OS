// util.h
#ifndef _UTIL_H
#define _UTIL_H

#include "types.h"

/* 内存操作 */
void* memset(void* dst, int c, size_t n);
void* memcpy(void* dst, const void* src, size_t n);

/* 磁盘操作 */
void disk_read(void* dst, uint32 offset, uint32 count);

/* 错误处理 */
__attribute__((noreturn)) void panic(const char* msg);

#endif