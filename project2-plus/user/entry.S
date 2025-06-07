# 用户程序入口点

.section .text
.global _start

_start:
    # 设置栈指针
    li sp, 0x80800000
    
    # 调用main函数
    call main
    
    # 如果main返回，调用exit系统调用
    li a0, 0      # 退出码为0
    li a7, 2      # SYS_exit
    ecall
    
    # 不应该到达这里
    j .