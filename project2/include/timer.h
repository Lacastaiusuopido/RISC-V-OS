#ifndef _TIMER_H_
#define _TIMER_H_

#include "types.h"

// 初始化时钟
void timer_init();

// 设置下一次时钟中断
void timer_set_next();

// 获取当前时间（毫秒）
uint64 get_time_ms();

// 处理时钟中断
void timer_handler();

#endif // _TIMER_H_