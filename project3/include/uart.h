#ifndef _UART_H_
#define _UART_H_

#include "types.h"

// 初始化UART设备
void uart_init();

// 向UART发送一个字符
void uart_putc(char c);

// 从UART读取一个字符（如果有）
int uart_getc();

// 检查UART是否有数据可读
int uart_has_data();

#endif // _UART_H_