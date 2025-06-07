// bootmain.c - 引导加载器的C语言部分
// 负责从磁盘加载内核并跳转执行

#include "../include/types.h"
#include "../include/riscv.h"
#include "../include/elf.h"
#include "../include/disk.h"
#include "../include/uart.h"

extern char _end[]; // 引导加载器结束地址，定义在链接脚本中

// 内核将被加载到的内存地址
#define KERNEL_SECTOR 4  // 从第4个扇区开始，给引导加载器留足空间
#define SECTOR_SIZE 512


// 简单的内存拷贝函数
void memcpy(void *dst, const void *src, uint32 n) {
  const char *s = src;
  char *d = dst;
  while (n-- > 0) {
    *d++ = *s++;
  }
}

// 简单的字符串输出函数
void puts(const char *s) {
  while(*s) {
    uart_putc(*s++);
  }
}

// 从磁盘加载内核并执行
void bootmain() {
  // 初始化串口，用于调试输出
  uart_init();

    // 立即输出一些字符，测试 UART 是否工作
  uart_putc('B');
  uart_putc('O');
  uart_putc('O');
  uart_putc('T');
  uart_putc('\n');
  
  // 输出启动信息
  puts("Bootloader: starting...\n");
    uint64 current_addr = (uint64)bootmain;
  for (int i = 15; i >= 0; i--) {
    uint8 digit = (current_addr >> (i * 4)) & 0xF;
    uart_putc(digit < 10 ? '0' + digit : 'A' + digit - 10);
  }
  puts("\n");

  puts("Bootloader: reading ELF header from sector ");
  uart_putc('0' + (KERNEL_SECTOR / 100) % 10);
  uart_putc('0' + (KERNEL_SECTOR / 10) % 10);
  uart_putc('0' + KERNEL_SECTOR % 10);
  puts("\n");

  // 读取并显示磁盘前16字节
uint8 buffer[16];
puts("Bootloader: First 16 bytes at sector 512:\n");
disk_read(buffer, KERNEL_SECTOR * SECTOR_SIZE, 16);
for (int i = 0; i < 16; i++) {
  uint8 byte = buffer[i];
  uint8 high = (byte >> 4) & 0xF;
  uint8 low = byte & 0xF;
  uart_putc(high < 10 ? '0' + high : 'A' + high - 10);
  uart_putc(low < 10 ? '0' + low : 'A' + low - 10);
  uart_putc(' ');
}
puts("\n");

  // 读取内核ELF文件头
  struct elfhdr elf;
  disk_read(&elf, KERNEL_SECTOR * SECTOR_SIZE, sizeof(elf));

  // 输出读取到的ELF头信息
  puts("Bootloader: ELF header read, size: ");
  uart_putc('0' + (sizeof(elf) / 10) % 10);
  uart_putc('0' + sizeof(elf) % 10);
  puts(" bytes\n");

  puts("Bootloader: ELF magic: 0x");
  // 以十六进制输出魔数
  uint32 magic = elf.magic;
  for (int i = 7; i >= 0; i--) {
    uint8 digit = (magic >> (i * 4)) & 0xF;
    uart_putc(digit < 10 ? '0' + digit : 'A' + digit - 10);
  }
  puts("\n");

  puts("Bootloader: Expected magic: 0x");
  magic = ELF_MAGIC;
  for (int i = 7; i >= 0; i--) {
    uint8 digit = (magic >> (i * 4)) & 0xF;
    uart_putc(digit < 10 ? '0' + digit : 'A' + digit - 10);
  }
  puts("\n");
  
  // 验证ELF魔数
  if (elf.magic != ELF_MAGIC) {
    puts("Bootloader: invalid ELF file\n");
    return; // 无法继续，陷入死循环
  }
  
  // 加载程序头表
  struct proghdr ph;
  uint32 off = elf.phoff + KERNEL_SECTOR * SECTOR_SIZE;
  
  // 遍历所有程序头，加载每个段
  for (int i = 0; i < elf.phnum; i++) {
    // 读取程序头
    disk_read(&ph, off, sizeof(ph));
    off += sizeof(ph);
    
    // 如果是可加载段
    if (ph.type == ELF_PROG_LOAD) {
      // 从磁盘读取段内容到指定内存
      disk_read((void*)ph.paddr, ph.offset + KERNEL_SECTOR * SECTOR_SIZE, ph.filesz);
      
      // 如果内存大小大于文件大小，清零剩余部分
      if (ph.memsz > ph.filesz) {
        uint32 *dst = (uint32*)(ph.paddr + ph.filesz);
        for (uint32 j = 0; j < (ph.memsz - ph.filesz) / 4; j++) {
          dst[j] = 0;
        }
      }
    }
  }
  
  puts("Bootloader: jumping to kernel...\n");
  
  // 跳转到内核入口点执行
  void (*kernel_entry)() = (void(*)())elf.entry;
  kernel_entry();
  
  // 如果内核返回，输出错误信息
  puts("Bootloader: kernel returned!\n");
  while(1) {} // 死循环
}