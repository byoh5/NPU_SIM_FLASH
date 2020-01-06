#ifndef _NPU_H
#define _NPU_H

#include "print.h"

#define NPU_DEBUG

#ifdef NPU_DEBUG
#   define npu_debug(format, ...)  do {_printf(format, __VA_ARGS__);} while(0)
#else
#   define npu_debug(format, ...)  do {} while(0)
#endif

#define NPU_MIN(a,b) (((a)<(b))?(a):(b))
#define NPU_MAX(a,b) (((a)>(b))?(a):(b))


typedef  volatile unsigned int npu_reg_t;

#define NPU_APB_BASE		      (0x46300000)
/* ADDR_NPU_IRQ_CONTROL
    [0] : run
    [1] : reset */
#define ADDR_NPU_CONTROL          ((npu_reg_t *)(NPU_APB_BASE + 0x00))
#define ADDR_NPU_STATUS           ((npu_reg_t *)(NPU_APB_BASE + 0x04))
#define ADDR_NPU_APB_COMMAND      ((npu_reg_t *)(NPU_APB_BASE + 0x08))
#define ADDR_NPU_RESERVED0        ((npu_reg_t *)(NPU_APB_BASE + 0x0C))
/* ADDR_NPU_IRQ_XXX
    [0] : full_empty  
    [1] : half_empty  
    [2] : trap  */
#define ADDR_NPU_IRQ_REASON       ((npu_reg_t *)(NPU_APB_BASE + 0x10))
#define ADDR_NPU_IRQ_ENABLE       ((npu_reg_t *)(NPU_APB_BASE + 0x14))
#define ADDR_NPU_IRQ_MASK         ((npu_reg_t *)(NPU_APB_BASE + 0x18))
#define ADDR_NPU_IRQ_CLEAR        ((npu_reg_t *)(NPU_APB_BASE + 0x1C))
#define ADDR_NPU_COLOR_CONV_0     ((npu_reg_t *)(NPU_APB_BASE + 0x20))
#define ADDR_NPU_COLOR_CONV_1     ((npu_reg_t *)(NPU_APB_BASE + 0x24))
#define ADDR_NPU_COLOR_CONV_2     ((npu_reg_t *)(NPU_APB_BASE + 0x28))
#define ADDR_NPU_COLOR_CONV_BIAS  ((npu_reg_t *)(NPU_APB_BASE + 0x2C))

#define ADDR_NPU_READ_INT_REG     ((npu_reg_t *)(NPU_APB_BASE + 0x30))
#define ADDR_NPU_INT_REG_RDATA    ((npu_reg_t *)(NPU_APB_BASE + 0x34))
#define ADDR_NPU_CURR_CMD         ((npu_reg_t *)(NPU_APB_BASE + 0x38))

#define ADDR_NPU_BASE_ADDR0       ((npu_reg_t *)(NPU_APB_BASE + 0x40))
#define ADDR_NPU_BASE_ADDR1       ((npu_reg_t *)(NPU_APB_BASE + 0x44))
#define ADDR_NPU_BASE_ADDR2       ((npu_reg_t *)(NPU_APB_BASE + 0x48))
#define ADDR_NPU_BASE_ADDR3       ((npu_reg_t *)(NPU_APB_BASE + 0x4C))
#define ADDR_NPU_BASE_ADDR4       ((npu_reg_t *)(NPU_APB_BASE + 0x50))
#define ADDR_NPU_BASE_ADDR5       ((npu_reg_t *)(NPU_APB_BASE + 0x54))
#define ADDR_NPU_BASE_ADDR6       ((npu_reg_t *)(NPU_APB_BASE + 0x58))
#define ADDR_NPU_BASE_ADDR7       ((npu_reg_t *)(NPU_APB_BASE + 0x5C))

#define NPU_READ_REG(ADDR)        ({npu_reg_t val; val = *ADDR; \
	                                npu_debug("RD_APB [%08x]=%08x\r\n", ADDR, val); \
                                    val;})
#define NPU_WRITE_REG(ADDR, DATA) ({ (*ADDR=(DATA)); \
	                                npu_debug("WR_APB [%08x]=%08x\r\n", ADDR, DATA);})

enum {
    NPU_CTRL_RUN        = 0x1,
    NPU_CTRL_RESET      = 0x2,
};

enum {
    NPU_IRQ_FULL_EMPTY  = 0x1,
    NPU_IRQ_HALF_EMPTY  = 0x2,
    NPU_IRQ_TRAP        = 0x4,
};

enum {
    NPU_OPCODE_NOP      = 0x0,
    NPU_OPCODE_RUN      = 0x1,
    NPU_OPCODE_WR_REG   = 0x2,
    NPU_OPCODE_TRAP     = 0x3,
};

enum {
    NPU_WAIT_COND_DMA   = 0x1,
    NPU_WAIT_COND_COMP  = 0x2,
};

//  Internal APB address map
//  |addr[10:8] : master |
//  | 0         : DMA    |
//  | 1         : MM     |
//  | 2         : DW     |
//  | 3         : MISC   |
//  | 4         : ACT    |

#define ADDR_NPU_INT_APB_DMA    (0x000)
#define SIZE_NPU_INT_APB_DMA    (5)
#define ADDR_NPU_INT_APB_MM     (0x100)
#define SIZE_NPU_INT_APB_MM     (5)
#define ADDR_NPU_INT_APB_DW     (0x200)
#define SIZE_NPU_INT_APB_DW     (5)
#define ADDR_NPU_INT_APB_MISC   (0x300)
#define SIZE_NPU_INT_APB_MISC   (5)
#define ADDR_NPU_INT_APB_ACT    (0x400)
#define SIZE_NPU_INT_APB_ACT    (5)
#define RDATA_NOT_READY         (0xFFFFFFFF)
#define INVALID_ADDR_ACCESS     (0xBAB0BAB0)


#define CMD_QUE_NUM             (32)
#define FEED_CMD_AT_FULL_EMTPY  (32)
#define FEED_CMD_AT_HALF_EMTPY  (16)

void probe_npu_int_apb_regs(void);

// npu
typedef struct {
    unsigned int command_num;
    unsigned int command_curr;
    unsigned int* command_buf;

    unsigned char* ext_c_base;
    unsigned char* ext_i_base;
    unsigned char* ext_o_base;
    unsigned char* ext_w_base;
    unsigned char* ext_b_base;
} npu_t;

// api
npu_t* npu_get_instance();
void npu_init(npu_t* npu_inst);
void npu_set_buf(npu_t* npu_inst,
                 unsigned char* cmd_addr,
                 unsigned char* ia_addr,
                 unsigned char* oa_addr,
                 unsigned char* w_addr,
                 unsigned char* b_addr);
void npu_interrupt_hanlder(void);
void npu_run_pic(npu_t* npu_inst);

// helper
int prepare_npu_cmd();
void load_cmd(int num);

// app
void run_npu(void);


#endif /*_NPU_H */
