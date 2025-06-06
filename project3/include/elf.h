// elf.h - ELF文件格式定义

#ifndef _ELF_H_
#define _ELF_H_

#include "types.h"

// ELF文件魔数
#define ELF_MAGIC 0x464C457F  // "\x7FELF" in little endian

// 程序头类型
#define ELF_PROG_LOAD 1  // 可加载段

// ELF文件头
struct elfhdr {
  uint32 magic;       // 魔数，必须为ELF_MAGIC
  uint8  elf[12];     // ELF标识信息
  uint16 type;        // 文件类型
  uint16 machine;     // 目标架构
  uint32 version;     // 文件版本
  uint64 entry;       // 入口点地址
  uint64 phoff;       // 程序头表偏移
  uint64 shoff;       // 节头表偏移
  uint32 flags;       // 处理器特定标志
  uint16 ehsize;      // ELF头大小
  uint16 phentsize;   // 程序头表项大小
  uint16 phnum;       // 程序头表项数量
  uint16 shentsize;   // 节头表项大小
  uint16 shnum;       // 节头表项数量
  uint16 shstrndx;    // 节名字符串表索引
};

// 程序头
struct proghdr {
  uint32 type;        // 段类型
  uint32 flags;       // 段标志
  uint64 offset;      // 段在文件中的偏移
  uint64 vaddr;       // 段的虚拟地址
  uint64 paddr;       // 段的物理地址
  uint64 filesz;      // 段在文件中的大小
  uint64 memsz;       // 段在内存中的大小
  uint64 align;       // 段对齐
};

#endif // _ELF_H_