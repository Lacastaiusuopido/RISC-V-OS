// boot/uart.c - 引导加载器的UART实现
// 简化版，仅提供基本的输入输出功能

#include "../include/uart.h"

// UART寄存器地址（QEMU virt机器）
#define UART0 0x10000000ULL  // 使用ULL后缀表示64位无符号长整型
#define UART_REG_RBR 0x00  // 接收缓冲寄存器（读取时）
#define UART_REG_THR 0x00  // 发送保持寄存器（写入时）
#define UART_REG_LSR 0x05  // 线路状态寄存器
#define UART_LSR_RX_READY 0x01  // 接收就绪
#define UART_LSR_TX_IDLE  0x20  // 发送空闲

// 读取UART寄存器
static inline uint8 uart_reg_read(uint8 reg) {
  return *(volatile uint8*)(UART0 + reg);
}

// 写入UART寄存器
static inline void uart_reg_write(uint8 reg, uint8 v) {
  *(volatile uint8*)(UART0 + reg) = v;
}

// 初始化UART设备
void uart_init() {
  // 简化版初始化，假设QEMU已经配置好了UART
}

// 向UART发送一个字符
void uart_putc(char c) {
  // 等待发送缓冲区空闲
  while((uart_reg_read(UART_REG_LSR) & UART_LSR_TX_IDLE) == 0);
  
  // 发送字符
  uart_reg_write(UART_REG_THR, c);
}

// 从UART读取一个字符（如果有）
int uart_getc() {
  if(!uart_has_data())
    return -1;
  return uart_reg_read(UART_REG_RBR);
}

// 检查UART是否有数据可读
int uart_has_data() {
  return (uart_reg_read(UART_REG_LSR) & UART_LSR_RX_READY) != 0;
}