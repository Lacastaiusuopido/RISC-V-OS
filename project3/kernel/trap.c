#include "../include/types.h"
#include "../include/riscv.h"
#include "../include/console.h"
#include "../include/syscall.h"
#include "../include/timer.h"

// 声明外部汇编函数trap_vector
extern void trap_vector();

/**
 * 中断处理函数
 * 
 * @param scause 中断/异常原因
 * @param sepc   中断/异常发生时的程序计数器值
 * @param stval  中断/异常相关的附加信息
 * @param regs   保存的寄存器状态数组
 */
void trap_handler(uint64 scause, uint64 sepc, uint64 stval, uint64 *regs) {
    // 获取中断原因，低8位为具体原因码
    uint64 cause = scause & 0xff;
    // 最高位为1表示中断，为0表示异常
    int is_interrupt = (scause >> 63) & 1;
    
    // 添加前缀以区分输出
    console_printf_TRAP("捕获到异常/中断\n");
    console_printf_TRAP("scause=0x%lx, sepc=0x%lx, stval=0x%lx\n", scause, sepc, stval);
    // 检查SPP位判断异常发生前的特权模式（1为S模式，0为U模式）
    console_printf_TRAP("当前模式: %s\n", (r_sstatus() & SSTATUS_SPP) ? "S模式" : "U模式");
    
    // 添加更多寄存器状态信息，帮助调试
    console_printf_TRAP("SSTATUS=0x%lx, SIE=0x%lx, SIP=0x%lx\n", 
                  r_sstatus(), r_sie(), r_sip());


    if (is_interrupt) {
        // 处理中断
        console_printf_TRAP("中断类型: ");
        switch (cause) {
            case 1: // 软件中断
                console_printf_TRAP("软件中断\n");
                // 清除软件中断标志位
                w_sip(r_sip() & ~SIP_SSIP);
                break;
                
            case 5: // 时钟中断
                console_printf_TRAP("时钟中断\n");
                // 调用时钟中断处理函数
                timer_handler();
                break;
                
            case 9: // 外部中断
                console_printf_TRAP("外部中断\n");
                // 清除外部中断标志位
                w_sip(r_sip() & ~SIP_SEIP);
                break;
                
            default:
                console_printf_TRAP("未知中断: %d\n", cause);
                break;
        }
    } else {
        // 处理异常
        console_printf_TRAP("异常类型: ");
        switch (cause) {
            case 0: // 指令地址不对齐
                console_printf_TRAP("指令地址不对齐\n");
                console_printf_TRAP("PC=0x%lx, 地址=0x%lx\n", sepc, stval);
                // 对于严重错误，可以选择修改sepc跳过，或者终止程序
                w_sepc(sepc + 4); // 尝试跳过错误指令
                break;
                
            case 1: // 指令访问错误
                console_printf_TRAP("指令访问错误\n");
                console_printf_TRAP("PC=0x%lx, 地址=0x%lx\n", sepc, stval);
                // 指令访问错误通常是访问了非法地址
                w_sepc(sepc + 4); // 尝试跳过错误指令
                break;
                
            case 2: // 非法指令
                console_printf_TRAP("非法指令\n");
                console_printf_TRAP("PC=0x%lx, 指令=0x%lx\n", sepc, stval);
                // 尝试打印出错指令的内容
                if (sepc != 0) {
                    uint32 inst = *(uint32*)sepc;
                    console_printf_TRAP("错误指令内容: 0x%08x\n", inst);
                }
                // 非法指令异常处理：跳过当前指令
                w_sepc(sepc + 4);
                break;
                
            case 8: // 环境调用（来自用户模式）
                console_printf_TRAP("环境调用（来自用户模式）\n");
                // 打印系统调用信息，系统调用号在a7(x17)寄存器中
                console_printf_TRAP("系统调用号: %d\n", regs[16]);
                console_printf_TRAP("参数1: 0x%lx\n", regs[9]);
                console_printf_TRAP("参数2: 0x%lx\n", regs[10]);
                
                // 处理系统调用
                // 系统调用号在a7寄存器中，参数在a0-a5寄存器中
                // 返回值存放在a0寄存器中
                regs[10] = syscall(regs[16], regs[9], regs[10], regs[11], regs[12], regs[13], regs[14]);
                console_printf_TRAP("系统调用返回值: 0x%lx\n", regs[9]);
                
                // 系统调用返回时，PC需要加4（跳过ecall指令）
                w_sepc(sepc + 4);
                break;
                
            case 12: // 指令页错误
                console_printf_TRAP("指令页错误\n");
                console_printf_TRAP("PC=0x%lx, 地址=0x%lx\n", sepc, stval);
                // 打印当前页表基址，帮助调试
                console_printf_TRAP("当前页表基址: 0x%lx\n", r_satp());
                // 页错误通常是严重错误，暂停系统
                while(1); // 暂停
                break;
                
            case 13: // 加载页错误
                console_printf_TRAP("加载页错误\n");
                console_printf_TRAP("PC=0x%lx, 地址=0x%lx\n", sepc, stval);
                // 打印当前页表基址
                console_printf_TRAP("当前页表基址: 0x%lx\n", r_satp());
                while(1); // 暂停
                break;
                
            case 15: // 存储页错误
                console_printf_TRAP("存储页错误\n");
                console_printf_TRAP("PC=0x%lx, 地址=0x%lx\n", sepc, stval);
                // 打印当前页表基址
                console_printf_TRAP("当前页表基址: 0x%lx\n", r_satp());
                while(1); // 暂停
                break;
                
            default:
                console_printf_TRAP("未知异常: %d\n", cause);
                console_printf_TRAP("PC=0x%lx, 地址=0x%lx\n", sepc, stval);
                
                // 打印所有寄存器状态，帮助调试
                console_printf_TRAP("寄存器状态:\n");
                for (int i = 0; i < 32; i++) {
                    if (i % 4 == 0) {
                        console_printf_TRAP("");
                    }
                    console_printf_TRAP("x%d: 0x%lx ", i, regs[i]);
                    if (i % 4 == 3) {
                        console_printf_TRAP("\n");
                    }
                }
                
                // 未知异常通常是严重错误，暂停系统
                while(1); // 暂停
                break;
        }
    }

    // 处理非法指令异常
    if ((scause & 0x8000000000000000L) == 0 && (scause & 0xff) == 2) {
        // 非法指令异常
        console_printf_TRAP("异常类型: 非法指令\n");
        console_printf_TRAP("PC=0x%lx, 指令=0x%x\n", sepc, *(uint32 *)sepc);
        console_printf_TRAP("错误指令内容: 0x%08x\n", *(uint32 *)sepc);
        
        // 终止用户程序执行，而不是继续执行下一条指令
        console_printf_TRAP("程序遇到非法指令，终止执行\n");
        
        // 可以选择以下方式之一:
        
        // 1. 终止进程 (如果有进程管理)
        // exit(-1);
        
        // 2. 跳转到一个安全的位置
        // tf->epc = (uint64)some_error_handler;
        // return;
        
        // 3. 简单地挂起系统
        console_printf_TRAP("系统挂起\n");
        while(1) {
            // 空循环，防止继续执行
        }
    }
    


    
}

/**
 * 初始化中断处理
 * 设置中断向量表地址和中断使能
 */
void trap_init() {
    // 设置中断向量表地址
    w_stvec((uint64)trap_vector);
    console_printf_TRAP("中断处理初始化完成，STVEC=0x%lx\n", r_stvec());
}