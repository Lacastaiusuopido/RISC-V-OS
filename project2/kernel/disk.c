#include "../include/types.h"
#include "../include/console.h"

// 用户程序在磁盘上的位置
#define USER_SECTOR 64
#define SECTOR_SIZE 512

// 简单的磁盘读取函数 - 直接从内存读取
void disk_read(void *dst, uint32 offset, uint32 count) {
    // 整个磁盘镜像已经被QEMU加载到内存中
    // 引导加载器位于 0x80000000
    
    // 计算源地址：引导加载器起始地址 + 偏移量
    char *src = (char*)0x80000000 + offset;
    char *d = (char*)dst;
    
    // 简单的内存拷贝
    for(uint32 i = 0; i < count; i++) {
        d[i] = src[i];
    }
}

// 调试函数：打印内存内容
void dump_memory(void *addr, uint32 count) {
    uint8 *p = (uint8*)addr;
    console_printf("内存内容 [0x%lx - 0x%lx]:\n", (uint64)addr, (uint64)addr + count - 1);
    
    for(uint32 i = 0; i < count; i++) {
        if(i % 16 == 0) {
            console_printf("\n0x%lx: ", (uint64)&p[i]);
        }
        console_printf("%02x ", p[i]);
    }
    console_printf("\n");
}