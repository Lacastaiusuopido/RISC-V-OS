// boot/disk.c - 引导加载器的磁盘访问实现
// 简化版本，直接从内存读取

#include "../include/disk.h"
#include "../include/types.h"

// 简单的磁盘读取函数 - 直接从内存读取
void disk_read(void *dst, uint32 offset, uint32 count) {
    // 我们假设整个磁盘镜像已经被QEMU加载到内存中
    // 引导加载器位于 0x80000000
    // 内核位于 0x80000000 + 4*512 (第4个扇区开始)
    
    // 计算源地址：引导加载器起始地址 + 偏移量
    char *src = (char*)0x80000000 + offset;
    char *d = (char*)dst;
    
    // 简单的内存拷贝
    for(uint32 i = 0; i < count; i++) {
        d[i] = src[i];
    }
}