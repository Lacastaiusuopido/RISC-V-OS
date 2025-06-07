// util.c
#include "util.h"
#include "console.h"

/*---- 内存操作 ----*/
void* memset(void* dst, int c, size_t n) {
    char* cdst = (char*)dst;
    for (size_t i = 0; i < n; i++) {
        cdst[i] = (char)c;
    }
    return dst;
}

void* memcpy(void* dst, const void* src, size_t n) {
    char* d = (char*)dst;
    const char* s = (const char*)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dst;
}

/*---- 磁盘操作 ----*/
void disk_read(void* dst, uint32 offset, uint32 count) {
    const char* src = (const char*)0x80000000 + offset;
    char* d = (char*)dst;
    
    for(uint32 i = 0; i < count; i++) {
        d[i] = src[i];
    }
}

/*---- 错误处理 ----*/
__attribute__((noreturn)) 
void panic(const char* msg) {
    console_printf_PANIC("内核错误: %s\n", msg);
    while (1) { 
        asm volatile("wfi"); 
    }
}