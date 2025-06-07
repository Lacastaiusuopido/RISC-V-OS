#ifndef _TRAP_H_
#define _TRAP_H_

#include "types.h"

// 初始化中断处理
void trap_init();

// 中断处理函数
void trap_handler();

#endif // _TRAP_H_