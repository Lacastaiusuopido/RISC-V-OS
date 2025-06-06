#ifndef _DISK_H_
#define _DISK_H_

#include "types.h"

// 从磁盘读取数据
// dst: 目标缓冲区
// offset: 磁盘偏移量
// count: 读取字节数
void disk_read(void *dst, uint32 offset, uint32 count);

#endif // _DISK_H_