#include "../include/console.h"
#include "../include/uart.h"
#include "../include/types.h"

// 初始化控制台
void console_init() {
    uart_init();
}

// 向控制台输出一个字符
void console_putc(char c) {
    // 处理换行符，在UNIX/Linux中，换行是'\n'
    // 但在串口中，我们需要发送'\r\n'
    if (c == '\n') {
        uart_putc('\r');
    }
    uart_putc(c);
}

// 向控制台输出字符串
void console_puts(const char *s) {
    while (*s) {
        console_putc(*s++);
    }
}

// 从控制台读取一个字符（如果有）
int console_getc() {
    return uart_getc();
}

// 将整数转换为字符串
static char* itoa(int value, char* result, int base) {
    // 检查base是否支持
    if (base < 2 || base > 36) {
        *result = '\0';
        return result;
    }

    char* ptr = result, *ptr1 = result;
    char tmp_char;
    int tmp_value;

    // 处理负数
    if (value < 0 && base == 10) {
        *ptr++ = '-';
        value = -value;
    }

    // 处理0
    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return result;
    }

    // 转换整数为字符串
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[tmp_value - value * base];
    } while (value);

    // 添加字符串结束符
    *ptr-- = '\0';

    // 反转字符串
    if (*result == '-') {
        ptr1++;
    }
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }

    return result;
}

// 将长整数转换为字符串
static char* ltoa(unsigned long value, char* result, int base) {
    // 检查base是否支持
    if (base < 2 || base > 36) {
        *result = '\0';
        return result;
    }

    char* ptr = result, *ptr1 = result;
    char tmp_char;
    unsigned long tmp_value;

    // 处理0
    if (value == 0) {
        *ptr++ = '0';
        *ptr = '\0';
        return result;
    }

    // 转换整数为字符串
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[tmp_value - value * base];
    } while (value);

    // 添加字符串结束符
    *ptr-- = '\0';

    // 反转字符串
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }

    return result;
}


// 格式化输出到控制台

int console_printf(const char *fmt, ...) {
    char c;
    int count = 0;
    char buf[24];
    va_list args;
    
    va_start(args, fmt);
    
    while ((c = *fmt++)) {
        if (c != '%') {
            console_putc(c);
            count++;
            continue;
        }
        
        // 处理格式说明符
        c = *fmt++;
        if (!c) break;
        
        // 处理宽度和填充
        int width = 0;
        int zero_padding = 0;
        
        if (c == '0') {
            zero_padding = 1;
            c = *fmt++;
            if (!c) break;
        }
        
        // 解析宽度
        while (c >= '0' && c <= '9') {
            width = width * 10 + (c - '0');
            c = *fmt++;
            if (!c) break;
        }
        
        // 检查长整数标志
        int is_long = 0;
        if (c == 'l') {
            is_long = 1;
            c = *fmt++;
            if (!c) break;
        }
        
        switch (c) {
            case 'c': {
                // 字符
                char ch = (char)va_arg(args, int);
                console_putc(ch);
                count++;
                break;
            }
            
            case 's': {
                // 字符串
                char* s = va_arg(args, char*);
                if (!s) s = "(null)";
                
                // 计算字符串长度
                int len = 0;
                char* p = s;
                while (*p++) len++;
                
                // 处理宽度填充
                if (width > len && !zero_padding) {
                    for (int i = 0; i < width - len; i++) {
                        console_putc(' ');
                        count++;
                    }
                }
                
                // 输出字符串
                while (*s) {
                    console_putc(*s++);
                    count++;
                }
                break;
            }
            
            case 'd': {
                // 有符号十进制整数
                if (is_long) {
                    long value = va_arg(args, long);
                    if (value < 0) {
                        console_putc('-');
                        count++;
                        value = -value;
                    }
                    ltoa(value, buf, 10);
                } else {
                    int value = va_arg(args, int);
                    itoa(value, buf, 10);
                }
                
                // 计算数字长度
                int len = 0;
                char* p = buf;
                while (*p++) len++;
                
                // 处理零填充
                if (width > len && zero_padding) {
                    for (int i = 0; i < width - len; i++) {
                        console_putc('0');
                        count++;
                    }
                }
                
                // 输出数字
                p = buf;
                while (*p) {
                    console_putc(*p++);
                    count++;
                }
                break;
            }
            
            case 'x': {
                // 十六进制整数
                if (is_long) {
                    unsigned long value = va_arg(args, unsigned long);
                    ltoa(value, buf, 16);
                } else {
                    unsigned int value = va_arg(args, unsigned int);
                    itoa(value, buf, 16);
                }
                
                // 计算数字长度
                int len = 0;
                char* p = buf;
                while (*p++) len++;
                
                // 处理零填充
                if (width > len && zero_padding) {
                    for (int i = 0; i < width - len; i++) {
                        console_putc('0');
                        count++;
                    }
                }
                
                // 输出数字
                p = buf;
                while (*p) {
                    console_putc(*p++);
                    count++;
                }
                break;
            }
            
            case 'p': {
                // 指针（十六进制）
                unsigned long value = (unsigned long)va_arg(args, void*);
                console_putc('0');
                console_putc('x');
                ltoa(value, buf, 16);
                
                // 计算数字长度
                int len = 0;
                char* p = buf;
                while (*p++) len++;
                
                // 处理零填充
                if (width > len && zero_padding) {
                    for (int i = 0; i < width - len; i++) {
                        console_putc('0');
                        count++;
                    }
                }
                
                // 输出数字
                p = buf;
                while (*p) {
                    console_putc(*p++);
                    count++;
                }
                count += 2; // 为"0x"前缀
                break;
            }
            
            case '%': {
                // 百分号
                console_putc('%');
                count++;
                break;
            }
            
            default: {
                // 未知格式，输出原样
                console_putc('%');
                if (zero_padding) console_putc('0');
                if (width > 0) {
                    char width_buf[8];
                    itoa(width, width_buf, 10);
                    char* p = width_buf;
                    while (*p) {
                        console_putc(*p++);
                    }
                }
                if (is_long) console_putc('l');
                console_putc(c);
                count += 2;
                break;
            }
        }
    }
    
    va_end(args);
    return count;
}

// 测试函数，用于验证console_printf是否正常工作
void console_test() {
    console_printf("测试整数: %d\n", 12345);
    console_printf("测试十六进制: 0x%x\n", 0xabcd);
    console_printf("测试长整数: 0x%lx\n", 0x8040000000000000UL);
    console_printf("测试字符串: %s\n", "Hello, World!");
    console_printf("测试字符: %c\n", 'A');
}