
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

void main_0(void)
{

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


unsigned int *pDdr = (unsigned int*)CONV_CMD_ADDR;//(unsigned int*)SFLS_BASE;

  if(*pDdr == 0x6F656E63){
	//  printf("OK 0x6F656E63 == %08x",*pDdr);
  }else{
	  printf("NO 0x6F656E63 != %08x",*pDdr);
	  while(1);
  }


// DdrInit(2);

// int id = read_csr(mhartid);
// _printf("This is CPU : %d\n",id); // ADD some ...
// int targetArea = id + 1;
 unsigned char *pSfls = (unsigned char*)0xc0010000;//(unsigned int*)SFLS_BASE;


//    hwflush_dcache_range_all();

 int offset =0;

//    memset((void *)(intptr_t)0x80000000,0, 0x1000000);

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


