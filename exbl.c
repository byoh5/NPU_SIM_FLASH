
#include "uart.h"
#include "print.h"
#include "boot.h"
#include "ddr.h"
#include "encoding.h"
#include <stdint.h>
#include <string.h>
#include "reg.h"
#include "npu.h"
#include "dma.h"
#include "en675.h"
#include "sfls.h"

#define SIZE_OF_DATA 1024*16
int g_test_data = 0;

char g_key=0x0;


#define SLEEP_CNT 100000

#define BASE_ADDR 0x80003000


void sleep_(int cnt)
{

	for(int i=0;i<cnt;i++){
		asm volatile("nop" : :);
	}

}

//#define SFLS_BASE 0xc0000000
//#define SFLS_SIZE 16*1024*1024
#define BOOT_ADDRESS 0
void main_(void)
{
  printf("exception\n");
  while(1);
}

void call(unsigned long addr)
{
  	asm volatile("jalr\t\t%0" : : "r" (addr));
  	asm volatile("nop" : :);
}

void orbit_writel(volatile unsigned int *addr, unsigned int data)
{
	*((unsigned int *)(addr)) = data;
}

unsigned int orbit_readl(volatile unsigned int *addr)
{
	return *((unsigned int *)(addr));
}


#define CPU_FREQ				(600*1000*1000)



// Sleep ns nano-seconds
void orbit_nsleep(int ns)
{
	WaitXns(ns);
}

// Sleep us micro-seconds
void orbit_usleep(int us)
{
	WaitXus(us);
}

#define OMC_APB_BASE_ADDR	0x46200000
#define PHY_APB_BASE_ADDR	0x46400000
#define OMC_SECURE_APB_BASE_ADDR	0x46201000

void orbit_init_new_1(void)
{
	// New_1
orbit_writel(PHY_APB_BASE_ADDR + 0x0, 0x4708080c);
orbit_writel(PHY_APB_BASE_ADDR + 0xc, 0x42101000);
orbit_writel(PHY_APB_BASE_ADDR + 0x18, 0x0000000f);
orbit_writel(PHY_APB_BASE_ADDR + 0x4c, 0x00000000);
orbit_writel(PHY_APB_BASE_ADDR + 0x40, 0x00000000);
orbit_writel(PHY_APB_BASE_ADDR + 0x44, 0x00000000);
orbit_writel(PHY_APB_BASE_ADDR + 0x0, 0x4707080c);
orbit_writel(PHY_APB_BASE_ADDR + 0xd0, 0x0f0c7304);
orbit_writel(PHY_APB_BASE_ADDR + 0xd0, 0x0f0c7306);
while ((orbit_readl(PHY_APB_BASE_ADDR + 0xd4) & 0x00000001) != 0x00000001) {
  orbit_usleep(1);
}
orbit_writel(PHY_APB_BASE_ADDR + 0xd0, 0x0f0c7304);
orbit_writel(PHY_APB_BASE_ADDR + 0xd0, 0x0f087304);
orbit_writel(OMC_APB_BASE_ADDR + 0x0, 0x00000400);
orbit_writel(OMC_SECURE_APB_BASE_ADDR + 0xf00, 0x40001010);
orbit_writel(OMC_SECURE_APB_BASE_ADDR + 0xf04, 0x00800001);
orbit_writel(OMC_SECURE_APB_BASE_ADDR + 0xf10, 0x00800000);
orbit_writel(OMC_SECURE_APB_BASE_ADDR + 0xf14, 0x009fffff);
orbit_writel(OMC_SECURE_APB_BASE_ADDR + 0xf18, 0x00000000);
orbit_writel(OMC_SECURE_APB_BASE_ADDR + 0xf30, 0x03000011);
orbit_writel(OMC_SECURE_APB_BASE_ADDR + 0x110, 0xc0000001);
orbit_writel(OMC_SECURE_APB_BASE_ADDR + 0x114, 0xffffffff);
orbit_writel(OMC_APB_BASE_ADDR + 0x10c, 0x00000101);
orbit_writel(OMC_APB_BASE_ADDR + 0x11c, 0x00000000);
orbit_writel(OMC_APB_BASE_ADDR + 0x500, 0x00000201);
orbit_writel(OMC_APB_BASE_ADDR + 0x504, 0x40000000);
while ((orbit_readl(OMC_APB_BASE_ADDR + 0x504) & 0x80000000) != 0x80000000) {
  orbit_usleep(1);
}
orbit_writel(OMC_APB_BASE_ADDR + 0x504, 0x00000000);
orbit_writel(PHY_APB_BASE_ADDR + 0x50, 0x01004700);
orbit_writel(PHY_APB_BASE_ADDR + 0x50, 0x00004700);
// tINIT0 is controlled by System
// Drive CKE high
orbit_writel(OMC_APB_BASE_ADDR + 0x10, 0x0000001c);
orbit_writel(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
// Waits tINIT3 (200 us): Minimum idle time after first CKE assertion
orbit_usleep(200);
// Issue RESET (MRW) command
orbit_writel(OMC_APB_BASE_ADDR + 0x4, 0x103f00fc);
orbit_writel(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
// Waits tINIT4 (1 us): Minimum idle time after RESET command
orbit_usleep(1);
// Waits tINIT5 (9 us): Maximum duration of device auto initialization
// You can do polling on MR status to finish auto initialization part early
orbit_usleep(9);
orbit_writel(OMC_APB_BASE_ADDR + 0x4, 0x100a00ff);
orbit_writel(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
// Waits tZQINIT (1 us)
orbit_usleep(1);
// Raise up frequency of PLL for highest working condition
// Waits until frequency is stable
// Write down RCLK-related CRs
orbit_writel(OMC_APB_BASE_ADDR + 0x600, 0x002e0176);
orbit_writel(OMC_APB_BASE_ADDR + 0x604, 0x002e0176);
orbit_writel(OMC_APB_BASE_ADDR + 0x608, 0x001700bb);
orbit_writel(OMC_APB_BASE_ADDR + 0x60c, 0x000b005d);
orbit_writel(OMC_APB_BASE_ADDR + 0x610, 0x0005002e);
orbit_writel(OMC_APB_BASE_ADDR + 0x614, 0x00020017);
orbit_writel(OMC_APB_BASE_ADDR + 0x618, 0x00020017);
orbit_writel(OMC_APB_BASE_ADDR + 0x61c, 0x00020017);
orbit_writel(OMC_APB_BASE_ADDR + 0x678, 0x00000019);
orbit_writel(OMC_APB_BASE_ADDR + 0x100, 0x000000e0);
orbit_writel(OMC_APB_BASE_ADDR + 0x620, 0x00000f11);
orbit_writel(OMC_APB_BASE_ADDR + 0x624, 0x060c0206);
orbit_writel(OMC_APB_BASE_ADDR + 0x628, 0x3022070f);
orbit_writel(OMC_APB_BASE_ADDR + 0x62c, 0x17110928);
orbit_writel(OMC_APB_BASE_ADDR + 0x630, 0x00000c06);
orbit_writel(OMC_APB_BASE_ADDR + 0x634, 0x00100000);
orbit_writel(OMC_APB_BASE_ADDR + 0x638, 0x000c0c04);
orbit_writel(OMC_APB_BASE_ADDR + 0x63c, 0x16100003);
orbit_writel(OMC_APB_BASE_ADDR + 0x640, 0x1070152a);
orbit_writel(OMC_APB_BASE_ADDR + 0x644, 0x00310068);
orbit_writel(OMC_APB_BASE_ADDR + 0x648, 0x00000000);
orbit_writel(OMC_APB_BASE_ADDR + 0x64c, 0x0006170c);
orbit_writel(OMC_APB_BASE_ADDR + 0x650, 0x1033240a);
orbit_writel(OMC_APB_BASE_ADDR + 0x654, 0x00001310);
orbit_writel(OMC_APB_BASE_ADDR + 0x658, 0x00000060);
orbit_writel(OMC_APB_BASE_ADDR + 0x65c, 0x00480120);
orbit_writel(OMC_APB_BASE_ADDR + 0x660, 0x00000000);
orbit_writel(OMC_APB_BASE_ADDR + 0x680, 0x18000418);
orbit_writel(OMC_APB_BASE_ADDR + 0x684, 0x05000502);
orbit_writel(OMC_APB_BASE_ADDR + 0x688, 0x120b0404);
orbit_writel(OMC_APB_BASE_ADDR + 0x68c, 0x20003520);
orbit_writel(OMC_APB_BASE_ADDR + 0x690, 0x00140000);
orbit_writel(OMC_APB_BASE_ADDR + 0x69c, 0x01240074);
orbit_writel(OMC_APB_BASE_ADDR + 0x6a0, 0x00000000);
orbit_writel(OMC_APB_BASE_ADDR + 0x6a4, 0x20350c00);
orbit_writel(OMC_APB_BASE_ADDR + 0x6a8, 0x00040000);
orbit_writel(OMC_APB_BASE_ADDR + 0x4, 0x10010043);
orbit_writel(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
orbit_writel(OMC_APB_BASE_ADDR + 0x4, 0x1002001a);
orbit_writel(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
orbit_writel(PHY_APB_BASE_ADDR + 0x64, 0x0000000e);
orbit_writel(PHY_APB_BASE_ADDR + 0x68, 0x0000000f);
orbit_writel(PHY_APB_BASE_ADDR + 0xb0, 0x10900070);
orbit_readl(PHY_APB_BASE_ADDR + 0xb4);
orbit_writel(PHY_APB_BASE_ADDR + 0xb0, 0x10902ff0);
orbit_writel(PHY_APB_BASE_ADDR + 0xb0, 0x10902fd0);
orbit_writel(PHY_APB_BASE_ADDR + 0x4, 0x04200040);
orbit_writel(PHY_APB_BASE_ADDR + 0xc, 0x6b101000);
orbit_writel(PHY_APB_BASE_ADDR + 0x4, 0x0c200042);
orbit_writel(PHY_APB_BASE_ADDR + 0x64, 0x0000000e);
orbit_writel(PHY_APB_BASE_ADDR + 0x4, 0x0c200242);
while ((orbit_readl(PHY_APB_BASE_ADDR + 0x4) & 0x00020000) != 0x00020000) {
  orbit_usleep(1);
}
orbit_writel(PHY_APB_BASE_ADDR + 0x4, 0x0c200042);
orbit_writel(PHY_APB_BASE_ADDR + 0x10, 0x37ff5000);
orbit_readl(PHY_APB_BASE_ADDR + 0x108);
orbit_readl(PHY_APB_BASE_ADDR + 0xfc);
orbit_readl(PHY_APB_BASE_ADDR + 0x100);
orbit_readl(PHY_APB_BASE_ADDR + 0x104);
orbit_writel(PHY_APB_BASE_ADDR + 0x64, 0x0000000e);
orbit_writel(PHY_APB_BASE_ADDR + 0xc, 0x49001000);
orbit_writel(PHY_APB_BASE_ADDR + 0x4, 0x0c20004a);
orbit_writel(PHY_APB_BASE_ADDR + 0x4, 0x0c20084a);
while ((orbit_readl(PHY_APB_BASE_ADDR + 0x4) & 0x00020000) != 0x00020000) {
  orbit_usleep(1);
}
orbit_writel(PHY_APB_BASE_ADDR + 0x4, 0x0c20004a);
orbit_readl(PHY_APB_BASE_ADDR + 0xec);
orbit_readl(PHY_APB_BASE_ADDR + 0xf0);
orbit_readl(PHY_APB_BASE_ADDR + 0xf4);
orbit_writel(OMC_APB_BASE_ADDR + 0x10, 0x00000014);
orbit_writel(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
orbit_writel(PHY_APB_BASE_ADDR + 0x4, 0x0c20006a);
orbit_writel(PHY_APB_BASE_ADDR + 0x4, 0x0c20206a);
while ((orbit_readl(PHY_APB_BASE_ADDR + 0x4) & 0x00020000) != 0x00020000) {
  orbit_usleep(1);
}
orbit_writel(PHY_APB_BASE_ADDR + 0x4, 0x0c20006a);
orbit_readl(PHY_APB_BASE_ADDR + 0xec);
orbit_readl(PHY_APB_BASE_ADDR + 0xf0);
orbit_readl(PHY_APB_BASE_ADDR + 0xf4);
orbit_readl(PHY_APB_BASE_ADDR + 0xf8);
orbit_writel(OMC_APB_BASE_ADDR + 0x10, 0x00000017);
orbit_writel(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
orbit_writel(PHY_APB_BASE_ADDR + 0x4, 0x0c20002a);
orbit_writel(PHY_APB_BASE_ADDR + 0xb0, 0x10902ff0);
orbit_writel(PHY_APB_BASE_ADDR + 0x4, 0x0c60002a);
orbit_writel(PHY_APB_BASE_ADDR + 0x50, 0x01004700);
orbit_writel(PHY_APB_BASE_ADDR + 0x50, 0x00004700);
orbit_writel(OMC_APB_BASE_ADDR + 0x410, 0x00101010);
orbit_writel(OMC_APB_BASE_ADDR + 0x420, 0x0c181006);
orbit_writel(OMC_APB_BASE_ADDR + 0x424, 0x20200820);
orbit_writel(OMC_APB_BASE_ADDR + 0x428, 0x80000020);
orbit_writel(OMC_APB_BASE_ADDR + 0x0, 0x00000000);
orbit_writel(OMC_APB_BASE_ADDR + 0x108, 0x00003000);
orbit_writel(OMC_SECURE_APB_BASE_ADDR + 0x704, 0x00000007);
orbit_writel(OMC_APB_BASE_ADDR + 0x330, 0x09311fff);
orbit_writel(OMC_APB_BASE_ADDR + 0x334, 0x00000002);
orbit_writel(OMC_APB_BASE_ADDR + 0x324, 0x00002000);
orbit_writel(OMC_APB_BASE_ADDR + 0x104, 0x80000000);
orbit_writel(OMC_APB_BASE_ADDR + 0x510, 0x00180000);
orbit_writel(OMC_APB_BASE_ADDR + 0x514, 0x00000000);
orbit_writel(OMC_SECURE_APB_BASE_ADDR + 0x700, 0x00000003);
orbit_writel(OMC_APB_BASE_ADDR + 0x20, 0x00000001);
// Initialization done
}



void main_0(void)
{

	sfls_init();
	orbit_init_dfs_233MHz();

/*	SYS_PLL2_PWDN = 0;
	SYS_PLL2_P	  = 8;
	SYS_PLL2_M	  = 530;
	SYS_PLL2_S    = 2;
	SYS_PLL2_PWDN = 1;*/



	SYS_GPIO56_MUX = 3;	//	UART7
		SYS_GPIO57_MUX = 3;	//	UART7


//	SYS_REG0 = 0;
 //   _init_data_section();
  //   _init_bss_section();
	Uart_Init(115200);
//	 printf("This is CPU 0\n");


#define CONV_IA_ADDR 0x80400000
#define CONV_OA_ADDR 0x80600000   // 0x80600000
#define CONV_W_ADDR  0x80800000
#define CONV_B_ADDR  0x80a00000
#define CONV_CMD_ADDR 0x80200000

#define CONV_OA_ADDR_COMP 0x80b00000

#define CONV_IA_SIZE 2048
#define CONV_OA_SIZE 2048
#define CONV_W_SIZE  1024
#define CONV_B_SIZE  64
#define CONV_CMD_SIZE 208






	orbit_init_new_1();

// int id = read_csr(mhartid);
// _printf("This is CPU : %d\n",id); // ADD some ...
// int targetArea = id + 1;
 unsigned char *pSfls = (unsigned char*)0xc0010000;//(unsigned int*)SFLS_BASE;


//    hwflush_dcache_range_all();

 int offset =0;

//    memset((void *)(intptr_t)0x80000000,0, 0x1000000);

 // Reset(init) ddr for Simulation
 	 dma_set((void *)(intptr_t)CONV_IA_ADDR,		CONV_IA_SIZE,0);
 	 dma_set((void *)(intptr_t)CONV_OA_ADDR_COMP,	CONV_OA_SIZE,0);
 	 dma_set((void *)(intptr_t)CONV_W_ADDR,			CONV_W_SIZE,0);
 	 dma_set((void *)(intptr_t)CONV_B_ADDR,			CONV_B_SIZE,0);
 	 dma_set((void *)(intptr_t)CONV_CMD_ADDR,		CONV_CMD_SIZE,0);



 	 dma_copy((void *)(intptr_t)CONV_IA_ADDR		,(void *)(intptr_t)pSfls			, CONV_IA_SIZE);
 	offset += CONV_IA_SIZE;
 	dma_copy((void *)(intptr_t)CONV_OA_ADDR_COMP	,(void *)(intptr_t)pSfls+offset	, CONV_OA_SIZE);
 	offset += CONV_OA_SIZE;
 	dma_copy( (void *)(intptr_t)CONV_W_ADDR		,(void *)(intptr_t)pSfls+offset	,CONV_W_SIZE);
 	offset += CONV_W_SIZE;
 	dma_copy((void *)(intptr_t)CONV_B_ADDR		,(void *)(intptr_t)pSfls+offset	, CONV_B_SIZE);
 	offset += CONV_B_SIZE;
 	dma_copy((void *)(intptr_t)CONV_CMD_ADDR		,(void *)(intptr_t)pSfls+offset	, CONV_CMD_SIZE);

 	dma_set((void *)(intptr_t)CONV_OA_ADDR,CONV_OA_SIZE,0);

 	unsigned int *pDdr = (unsigned int*)CONV_CMD_ADDR;//(unsigned int*)SFLS_BASE;

 	  if(*pDdr == 0x6F656E63){
 		//  printf("OK 0x6F656E63 == %08x",*pDdr);
 	  }else{
 		  printf("NO 0x6F656E63 != %08x",*pDdr);
 		  while(1);
 	  }
 //	hwflush_dcache_range_all();
 //	 _printf("DDR done\n");
/*
 	printf("###########################RUN NPU#####################################\n");
 	offset =0;
 	 if(memcmp((void *)(intptr_t)CONV_IA_ADDR,(void *)(intptr_t)pSfls, CONV_IA_SIZE   )){
 	    	printf("########################### IA FFAAIILL!!#####################################");
 	    }else{
 	    	printf("########################### IA Success !!#####################################");
 	    }
 	offset += CONV_IA_SIZE;
 	 if(memcmp((void *)(intptr_t)CONV_OA_ADDR_COMP,(void *)(intptr_t)pSfls+offset, CONV_OA_SIZE   )){
 	    	printf("########################### OA FFAAIILL!!#####################################");
 	    }else{
 	    	printf("########################### OA Success !!#####################################");
 	    }
 	offset += CONV_OA_SIZE;
 	 if(memcmp((void *)(intptr_t)CONV_W_ADDR,(void *)(intptr_t)pSfls+offset, CONV_W_SIZE   )){
 	    	printf("########################### W FFAAIILL!!#####################################");
 	    }else{
 	    	printf("########################### W Success !!#####################################");
 	    }
 	offset += CONV_W_SIZE;
 	 if(memcmp((void *)(intptr_t)CONV_B_ADDR,(void *)(intptr_t)pSfls+offset, CONV_B_SIZE   )){
 	    	printf("########################### B FFAAIILL!!#####################################");
 	    }else{
 	    	printf("########################### B Success !!#####################################");
 	    }
 	offset += CONV_B_SIZE;
 	 if(memcmp((void *)(intptr_t)CONV_CMD_ADDR,(void *)(intptr_t)pSfls+offset, CONV_CMD_SIZE   )){
 	 	    	printf("########################### CMD FFAAIILL!!#####################################");
 	 	    }else{
 	 	    	printf("########################### CMD Success !!#####################################");
 	 	    }

*/


 	run_npu();


    hwflush_dcache_range_all();

    if(memcmp((void *)(intptr_t)CONV_OA_ADDR,(void *)(intptr_t)CONV_OA_ADDR_COMP, CONV_OA_SIZE   )){
    	_printf("###########################FFAAIILL!!#####################################");
    }else{
    	_printf("###########################Success !!#####################################");
    }


 	while(1);

}

void main_1(void)
{
	while(1);
}

void main_2(void)
{
	while(1);
}

void main_3(void)
{
	while(1);
}


