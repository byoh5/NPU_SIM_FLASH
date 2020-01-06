// See LICENSE for license details.

#ifndef _EXBL_H
#define _EXBL_H

#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })


#endif
