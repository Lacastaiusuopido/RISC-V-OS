#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "types.h"

// 初始化控制台
void console_init();

// 向控制台输出一个字符
void console_putc(char c);

// 向控制台输出字符串
void console_puts(const char *s);

// 从控制台读取一个字符（如果有）
int console_getc();

// 格式化输出到控制台
int console_printf(const char *fmt, ...);

void console_test();

#endif // _CONSOLE_H_