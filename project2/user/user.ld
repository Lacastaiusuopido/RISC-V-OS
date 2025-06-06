OUTPUT_ARCH("riscv")
ENTRY(_start)

SECTIONS
{
  . = 0x80400000; /* 用户程序加载地址0x80400000 */
  
  .text : {
    *(.text .text.*)
  }
  
  .rodata : {
    *(.rodata .rodata.*)
  }
  
  .data : {
    *(.data .data.*)
  }
  
  .bss : {
    *(.bss .bss.*)
  }
}