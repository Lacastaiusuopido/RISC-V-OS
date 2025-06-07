# bootasm.S - 引导汇编代码
# 这是系统启动时执行的第一段代码，负责初始化基本硬件并跳转到C代码

.section .text
.globl _start
_start:
  # 关闭中断，防止启动过程中被打断
  csrw mie, zero
  
  # 设置栈指针
  # 栈放在引导加载器后面的安全位置
  li sp, 0x80100000    # 设置栈指针到一个安全的高地址
  
  # 跳转到C语言的bootmain函数继续执行引导过程
  call bootmain

# 如果bootmain返回，进入无限循环
spin:
  j spin