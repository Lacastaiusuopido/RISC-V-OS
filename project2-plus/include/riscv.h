#ifndef _RISCV_H_
#define _RISCV_H_

#include "types.h"

// RISC-V CSR寄存器操作宏
#define READ_CSR(reg) ({ uint64 __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })

#define WRITE_CSR(reg, val) ({ \
  asm volatile ("csrw " #reg ", %0" :: "r"(val)); })

// sstatus寄存器位
#define SSTATUS_SIE (1L << 1)  // Supervisor Interrupt Enable
#define SSTATUS_SPIE (1L << 5) // Supervisor Previous Interrupt Enable
#define SSTATUS_SPP (1L << 8)  // Supervisor Previous Privilege

// sie寄存器位
#define SIE_SSIE (1L << 1)     // Software Interrupt Enable
#define SIE_STIE (1L << 5)     // Timer Interrupt Enable
#define SIE_SEIE (1L << 9)     // External Interrupt Enable

// sip寄存器位
#define SIP_SSIP (1L << 1)     // Software Interrupt Pending
#define SIP_STIP (1L << 5)     // Timer Interrupt Pending
#define SIP_SEIP (1L << 9)     // External Interrupt Pending

// sstatus寄存器操作
static inline uint64 r_sstatus() {
  return READ_CSR(sstatus);
}

static inline void w_sstatus(uint64 x) {
  WRITE_CSR(sstatus, x);
}

// sie寄存器操作
static inline uint64 r_sie() {
  return READ_CSR(sie);
}

static inline void w_sie(uint64 x) {
  WRITE_CSR(sie, x);
}

// sip寄存器操作
static inline uint64 r_sip() {
  return READ_CSR(sip);
}

static inline void w_sip(uint64 x) {
  WRITE_CSR(sip, x);
}

// stvec寄存器操作
static inline uint64 r_stvec() {
  return READ_CSR(stvec);
}

static inline void w_stvec(uint64 x) {
  WRITE_CSR(stvec, x);
}

// sepc寄存器操作
static inline uint64 r_sepc() {
  return READ_CSR(sepc);
}

static inline void w_sepc(uint64 x) {
  WRITE_CSR(sepc, x);
}

// scause寄存器操作
static inline uint64 r_scause() {
  return READ_CSR(scause);
}

// stval寄存器操作
static inline uint64 r_stval() {
  return READ_CSR(stval);
}

// satp寄存器操作
static inline uint64 r_satp() {
  return READ_CSR(satp);
}

static inline void w_satp(uint64 x) {
  WRITE_CSR(satp, x);
}

// 刷新TLB
static inline void sfence_vma() {
  asm volatile("sfence.vma zero, zero");
}

#endif // _RISCV_H_