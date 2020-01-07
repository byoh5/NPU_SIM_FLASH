
#include "npu.h"
#include <stdint.h>
#include <string.h>
#include "print.h"

#include "asm.h"
#include "rv_utils.h"

npu_t npu_instance;

/* NPU APIs */
npu_t* npu_get_instance()
{
    return &npu_instance;
}

#define EMEM_BASE   (0x80000000)
#define EMEM_C_ADDR (0x80200000)
#define EMEM_I_ADDR (0x80400000)
#define EMEM_O_ADDR (0x80600000)
#define EMEM_W_ADDR (0x80800000)
#define EMEM_B_ADDR (0x80a00000)

void npu_init(npu_t* npu_inst)
{
    unsigned int data;

    npu_inst->command_num = 0;
    npu_inst->command_curr = 0;
    npu_inst->command_buf = NULL;


    NPU_WRITE_REG(ADDR_NPU_CONTROL, NPU_CTRL_RESET);

    //NPU_WRITE_REG(ADDR_NPU_STATUS, 0x0);
    //NPU_WRITE_REG(ADDR_NPU_APB_COMMAND, 0x0);
    //NPU_WRITE_REG(ADDR_NPU_IRQ_REASON, 0x0);

    data = NPU_IRQ_FULL_EMPTY | NPU_IRQ_HALF_EMPTY | NPU_IRQ_TRAP;
    NPU_WRITE_REG(ADDR_NPU_IRQ_ENABLE, data); 
    NPU_WRITE_REG(ADDR_NPU_IRQ_MASK, data);
    //NPU_WRITE_REG(ADDR_NPU_IRQ_CLEAR, 0x0);
    NPU_WRITE_REG(ADDR_NPU_COLOR_CONV_0, 0x00200031);
    NPU_WRITE_REG(ADDR_NPU_COLOR_CONV_1, 0x0020FAF1);
    NPU_WRITE_REG(ADDR_NPU_COLOR_CONV_2, 0x00203A00);
    data = 0x33B  << 20;
    data += 0x054 << 10;
    data += 0x318 << 0;
    NPU_WRITE_REG(ADDR_NPU_COLOR_CONV_BIAS, data);
    //NPU_WRITE_REG(ADDR_NPU_COLOR_CONV_0, 0);
    //NPU_WRITE_REG(ADDR_NPU_COLOR_CONV_1, 0);
    //NPU_WRITE_REG(ADDR_NPU_COLOR_CONV_2, 0);
    //NPU_WRITE_REG(ADDR_NPU_COLOR_CONV_BIAS, 0x0);

    NPU_WRITE_REG(ADDR_NPU_BASE_ADDR0, 0);
    NPU_WRITE_REG(ADDR_NPU_BASE_ADDR1, 0);
    NPU_WRITE_REG(ADDR_NPU_BASE_ADDR2, 0);
    NPU_WRITE_REG(ADDR_NPU_BASE_ADDR3, 0);
    NPU_WRITE_REG(ADDR_NPU_BASE_ADDR4, 0);
    NPU_WRITE_REG(ADDR_NPU_BASE_ADDR5, 0);
    NPU_WRITE_REG(ADDR_NPU_BASE_ADDR6, 0);
    NPU_WRITE_REG(ADDR_NPU_BASE_ADDR7, 0);
}


void npu_set_buf(npu_t* npu_inst,
                 unsigned char* cmd_addr,
                 unsigned char* ia_addr,
                 unsigned char* oa_addr,
                 unsigned char* w_addr,
                 unsigned char* b_addr)
{
    npu_inst->ext_c_base = cmd_addr;
    npu_inst->ext_i_base = ia_addr;
    npu_inst->ext_o_base = oa_addr;
    npu_inst->ext_w_base = w_addr;
    npu_inst->ext_b_base = b_addr;
    
    npu_debug("%08x : cmd buf addr\r\n", cmd_addr);
    npu_debug("%08x : ia  buf addr\r\n", ia_addr);
    npu_debug("%08x : oa  buf addr\r\n", oa_addr);
    npu_debug("%08x : w   buf addr\r\n", w_addr);
    npu_debug("%08x : b   buf addr\r\n", b_addr);
}


void npu_interrupt_hanlder(void)
{
    unsigned int reason;
    unsigned int reason_clear;

    reason = NPU_READ_REG(ADDR_NPU_IRQ_REASON);

    if (reason & NPU_IRQ_FULL_EMPTY) {
        npu_debug("f isr %08x \r\n", reason);
        reason_clear = NPU_IRQ_FULL_EMPTY;
        load_cmd(FEED_CMD_AT_FULL_EMTPY);
    }
    else if (reason & NPU_IRQ_HALF_EMPTY) {
        npu_debug("h isr %08x \r\n", reason);
        reason_clear = NPU_IRQ_HALF_EMPTY;
        load_cmd(FEED_CMD_AT_HALF_EMTPY);
    }
    else {
        reason_clear = NPU_IRQ_TRAP;
    }

    NPU_WRITE_REG(ADDR_NPU_IRQ_REASON, reason_clear);
    NPU_WRITE_REG(ADDR_NPU_IRQ_CLEAR, 1);
}


void npu_run_pic(npu_t* npu_inst)
{
#ifdef NPU_DEBUG
    int wait_time_out = 50;
#endif
    int cmd_num = prepare_npu_cmd(npu_inst->ext_c_base);

    load_cmd(FEED_CMD_AT_FULL_EMTPY);
    NPU_WRITE_REG(ADDR_NPU_CONTROL, NPU_CTRL_RUN);

    do { 
        npu_reg_t reason_poll = NPU_READ_REG(ADDR_NPU_IRQ_REASON);
        NPU_READ_REG(ADDR_NPU_CURR_CMD);
        NPU_READ_REG(ADDR_NPU_STATUS);

        if (reason_poll)
            npu_interrupt_hanlder();

        if (reason_poll == NPU_IRQ_TRAP) {
            npu_debug("trap %08x \r\n", reason_poll);
            break;
        }
#ifdef NPU_DEBUG
        if (--wait_time_out < 0) {
            npu_debug("wait time out\r\n", 0);
            break;
        }
#endif

    } while(1);

    NPU_READ_REG(ADDR_NPU_CURR_CMD);
    NPU_READ_REG(ADDR_NPU_STATUS);

    NPU_WRITE_REG(ADDR_NPU_CONTROL, 0);

#ifdef NPU_DEBUG
    probe_npu_int_apb_regs();
#endif
}


/* NPU Helpers */
int prepare_npu_cmd(void)
{
    int i;
    npu_t* npu_inst = npu_get_instance();

    npu_inst->command_curr = 0;

    unsigned int* command_buf = (unsigned int*)npu_inst->ext_c_base;
    unsigned int magic_code = command_buf[0];
    npu_inst->command_num = command_buf[1];
    npu_inst->command_buf = &command_buf[2];

    npu_debug("%08X #%d\r\n", magic_code, npu_inst->command_num);
    
    for (i = 0; i < npu_inst->command_num; i++)
        npu_debug("%08x\r\n", npu_inst->command_buf[i]);

    return npu_inst->command_num;

}

void load_cmd(int num)
{
    int i;
    npu_t* npu_inst = npu_get_instance();
    const unsigned int trap_cmd = 
        (NPU_OPCODE_TRAP << 28) + ((NPU_WAIT_COND_DMA | NPU_WAIT_COND_COMP) << 24);

    unsigned int  cmd_num = npu_inst->command_num;
    unsigned int  cmd_cur = npu_inst->command_curr;
    unsigned int* cmd_buf = &npu_inst->command_buf[cmd_cur];

    int feed_num = NPU_MIN((cmd_num - cmd_cur), num);

    if (cmd_cur < cmd_num) {
        for (i = 0; i < feed_num; i++) {
            NPU_WRITE_REG(ADDR_NPU_APB_COMMAND, cmd_buf[i]);
            cmd_cur++;
        }

        npu_inst->command_curr = cmd_cur;
    }

    npu_debug("%d %d \r\n", cmd_cur, cmd_num);

    if (cmd_cur >= cmd_num) {
        NPU_WRITE_REG(ADDR_NPU_APB_COMMAND, trap_cmd);
    }

    npu_debug("load_cmd done %d %d %d\r\n", cmd_cur, cmd_num, num);
}


/* NPU Apps */
void wait_ready() 
{ 
    //
}


void flush_dcache_all()
{
    ulong sadr = 0x80000000;
    ulong size = 0x20000000;
    hwflush_dcache_range(sadr, size);
}


void run_npu(void)
{
    // Host need to provide buffer addrs
    unsigned char* c_addr = (unsigned char*)EMEM_C_ADDR;
    unsigned char* i_addr = (unsigned char*)EMEM_I_ADDR;
    unsigned char* o_addr = (unsigned char*)EMEM_O_ADDR;
    unsigned char* w_addr = (unsigned char*)EMEM_W_ADDR;
    unsigned char* b_addr = (unsigned char*)EMEM_B_ADDR;

    unsigned int  len    = 20*1024*1024;
    unsigned char* base   = (unsigned char*)EMEM_BASE;
    unsigned char* end    = base + len;

//#define CLEAR_MEM
#ifdef CLEAR_MEM
    {
        int i;
        unsigned char* start = (unsigned char*)EMEM_I_ADDR;
        unsigned char* end = (unsigned char*)EMEM_W_ADDR;
        int size = EMEM_W_ADDR - EMEM_I_ADDR;
        npu_debug("Start clear mem %08x - %08x\r\n", start, end);
        for (i = 0; i < size; i++)
            start[i] = 0;
        npu_debug("done\r\n", 0);
    }
#endif

    npu_t* npu_inst = npu_get_instance();

    npu_init(npu_inst);
    npu_set_buf(npu_inst, c_addr, i_addr, o_addr, w_addr, b_addr);

    int run_count = 0;

    if(1) {  // While(1)

        flush_dcache_all();

        wait_ready();

#ifdef ACT_PRINT
        {
            int i;
            int len = 128;
            npu_debug("ia mem %08x - %08x\r\n", i_addr, i_addr + len);
            for (i = 0; i < len/16; i++) {
                npu_debug("%02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x\r\n", 
                   i_addr[i*16 +  3],  i_addr[i*16 +  2], i_addr[i*16 +  1], i_addr[i*16 +  0], 
                   i_addr[i*16 +  7],  i_addr[i*16 +  6], i_addr[i*16 +  5], i_addr[i*16 +  4], 
                   i_addr[i*16 + 11],  i_addr[i*16 + 10], i_addr[i*16 +  9], i_addr[i*16 +  8], 
                   i_addr[i*16 + 15],  i_addr[i*16 + 14], i_addr[i*16 + 13], i_addr[i*16 + 12]);
            }
        }
#endif
        npu_debug("[%d] npu_run_pic\r\n", run_count);

        npu_run_pic(npu_inst);

#ifdef ACT_PRINT
        {
            int i;
            int len = 128;
            npu_debug("oa mem %08x - %08x\r\n", o_addr, o_addr + len);
            for (i = 0; i < len/16; i++) {
                npu_debug("%02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x\r\n", 
                   o_addr[i*16 +  3],  o_addr[i*16 +  2], o_addr[i*16 +  1], o_addr[i*16 +  0], 
                   o_addr[i*16 +  7],  o_addr[i*16 +  6], o_addr[i*16 +  5], o_addr[i*16 +  4], 
                   o_addr[i*16 + 11],  o_addr[i*16 + 10], o_addr[i*16 +  9], o_addr[i*16 +  8], 
                   o_addr[i*16 + 15],  o_addr[i*16 + 14], o_addr[i*16 + 13], o_addr[i*16 + 12]);
            }
        }
#endif
        run_count++;

      //  flush_dcache_all();
    }
}

