#include "../include/uart.h"

// QEMU RISC-V的UART地址
#define UART0 0x10000000ULL

// UART寄存器偏移量
#define RHR 0    // 接收保持寄存器（读取时）
#define THR 0    // 发送保持寄存器（写入时）
#define IER 1    // 中断使能寄存器
#define FCR 2    // FIFO控制寄存器
#define LCR 3    // 线路控制寄存器
#define LSR 5    // 线路状态寄存器

// LSR位
#define LSR_RX_READY (1<<0)  // 输入就绪
#define LSR_TX_IDLE  (1<<5)  // 发送空闲

// 读取UART寄存器
static inline uint8 uart_read_reg(uint8 reg) {
    return *(volatile uint8*)(UART0 + reg);
}

// 写入UART寄存器
static inline void uart_write_reg(uint8 reg, uint8 value) {
    *(volatile uint8*)(UART0 + reg) = value;
}

// 初始化UART设备
void uart_init() {
    // 禁用中断
    uart_write_reg(IER, 0x00);
    
    // 设置波特率（假设QEMU默认设置正确）
    
    // 设置为8位数据位，1位停止位，无奇偶校验
    uart_write_reg(LCR, 0x03);
    
    // 启用FIFO，清除FIFO，设置中断阈值
    uart_write_reg(FCR, 0x07);
    
    // 启用发送和接收中断
    uart_write_reg(IER, 0x03);
}

// 向UART发送一个字符
void uart_putc(char c) {
    // 等待发送缓冲区空闲
    while((uart_read_reg(LSR) & LSR_TX_IDLE) == 0);
    
    // 发送字符
    uart_write_reg(THR, c);
}

// 从UART读取一个字符（如果有）
int uart_getc() {
    if(!uart_has_data())
        return -1;
    
    // 读取字符
    return uart_read_reg(RHR);
}

// 检查UART是否有数据可读
int uart_has_data() {
    return (uart_read_reg(LSR) & LSR_RX_READY) != 0;
}