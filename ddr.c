#include "en675.h"
#include "reg.h"
#include "ddr.h"
#include "rv_utils.h" // dmwrite32, dmread32

#define TEST_DDR0_SIZE DDR0_SIZE
//#define TEST_DDR0_SIZE 1*1024

char DdrMapTest(void)
{
	volatile ULONG *pDDR = (volatile ULONG *)DDR0_BASE;
	//printf("DDR MAP Test - Write\n");
	for(pDDR = (volatile ULONG *)DDR0_BASE; pDDR < (volatile ULONG *)(DDR0_BASE+TEST_DDR0_SIZE); pDDR++)
	{
	  if ((ULONG)pDDR % 64 == 0) hwflush_dcache_range((ULONG)pDDR, 0x40);
	*pDDR = (ULONG)pDDR;
	}
	ULONG ok = 0, fail = 0;
	for(pDDR = (volatile ULONG *)DDR0_BASE; pDDR < (volatile ULONG *)(DDR0_BASE+TEST_DDR0_SIZE); pDDR++)
	{
		if ((ULONG)pDDR % 64 == 0) hwflush_dcache_range((ULONG)pDDR, 0x40);
		ULONG getdata = *pDDR;
		if (getdata != (ULONG)pDDR)
		{
                printf("DDR Test - X: %08lx 0x%08lx\n", pDDR, getdata);
                fail++;
		}
		else
		{
			ok++;
		}
     }
    printf("DDR Test - Done(O:%lu X:%lu)\n", ok, fail);
	return ((fail)? 1 : 0);
}

void DqG0OutDly(UINT var)
{
	DDR0_DLY_DQ7OUT  = var;
	DDR0_DLY_DQ6OUT  = var;
	DDR0_DLY_DQ5OUT  = var;
	DDR0_DLY_DQ4OUT  = var;
	DDR0_DLY_DQ3OUT  = var;
	DDR0_DLY_DQ2OUT  = var;
	DDR0_DLY_DQ1OUT  = var;
	DDR0_DLY_DQ0OUT  = var;
}
void DqG1OutDly(UINT var)
{
	DDR0_DLY_DQ15OUT = var;
	DDR0_DLY_DQ14OUT = var;
	DDR0_DLY_DQ13OUT = var;
	DDR0_DLY_DQ12OUT = var;
	DDR0_DLY_DQ11OUT = var;
	DDR0_DLY_DQ10OUT = var;
	DDR0_DLY_DQ9OUT  = var;
	DDR0_DLY_DQ8OUT  = var;
}
void DqG2OutDly(UINT var)
{
	DDR0_DLY_DQ23OUT = var;
	DDR0_DLY_DQ22OUT = var;
	DDR0_DLY_DQ21OUT = var;
	DDR0_DLY_DQ20OUT = var;
	DDR0_DLY_DQ19OUT = var;
	DDR0_DLY_DQ18OUT = var;
	DDR0_DLY_DQ17OUT = var;
	DDR0_DLY_DQ16OUT = var;
}
void DqG3OutDly(UINT var)
{
	DDR0_DLY_DQ31OUT = var;
	DDR0_DLY_DQ30OUT = var;
	DDR0_DLY_DQ29OUT = var;
	DDR0_DLY_DQ28OUT = var;
	DDR0_DLY_DQ27OUT = var;
	DDR0_DLY_DQ26OUT = var;
	DDR0_DLY_DQ25OUT = var;
	DDR0_DLY_DQ24OUT = var;
}
void DqOutDly(UINT var)
{
	DqG0OutDly(var);
	DqG1OutDly(var);
	DqG2OutDly(var);
	DqG3OutDly(var);
}
void DmOutDly(UINT var)
{
	DDR0_DLY_DM0OUT  = var;
	DDR0_DLY_DM1OUT  = var;
	DDR0_DLY_DM2OUT  = var;
	DDR0_DLY_DM3OUT  = var;
}
void CaOutDly(int var)
{
	DDR0_DLY_CA9OUT  = var;
	DDR0_DLY_CA8OUT  = var;
	DDR0_DLY_CA7OUT  = var;
	DDR0_DLY_CA6OUT  = var;
	DDR0_DLY_CA5OUT  = var;
	DDR0_DLY_CA4OUT  = var;
	DDR0_DLY_CA3OUT  = var;
	DDR0_DLY_CA2OUT  = var;
	DDR0_DLY_CA1OUT  = var;
	DDR0_DLY_CA0OUT  = var;
}

char ddr_cal_test()
{
	char result = 0;
	DDR0_MR_ADR = 32;
	DDR0_MR_WE  = 0;
	DDR0_MR_REQ = 1;
	while(DDR0_MR_REQ);
	if(DDR0_MR_RDDAT0!=0x00000000) result++;
	if(DDR0_MR_RDDAT1!=0x00000000) result++;
	if(DDR0_MR_RDDAT2!=0xffffffff) result++;
	if(DDR0_MR_RDDAT3!=0xffffffff) result++;
	//if(result>0) printf("%08x %08x %08x %08x ", DDR0_MR_RDDAT0,DDR0_MR_RDDAT1,DDR0_MR_RDDAT2,DDR0_MR_RDDAT3);
	DDR0_MR_ADR = 40;
	DDR0_MR_WE  = 0;
	DDR0_MR_REQ = 1;
	while(DDR0_MR_REQ);
	if(DDR0_MR_RDDAT0!=0xffffffff) result++;
	if(DDR0_MR_RDDAT1!=0x00000000) result++;
	if(DDR0_MR_RDDAT2!=0xffffffff) result++;
	if(DDR0_MR_RDDAT3!=0x00000000) result++;
	//if(result>0) printf("%08x %08x %08x %08x\n", DDR0_MR_RDDAT0,DDR0_MR_RDDAT1,DDR0_MR_RDDAT2,DDR0_MR_RDDAT3);
	DDR0_MR_ADR = 32;
	DDR0_MR_WE  = 0;
	DDR0_MR_REQ = 1;
	while(DDR0_MR_REQ);
	if(DDR0_MR_RDDAT0!=0x00000000) result++;
	if(DDR0_MR_RDDAT1!=0x00000000) result++;
	if(DDR0_MR_RDDAT2!=0xffffffff) result++;
	if(DDR0_MR_RDDAT3!=0xffffffff) result++;
	//if(result>0) printf("%08x %08x %08x %08x ", DDR0_MR_RDDAT0,DDR0_MR_RDDAT1,DDR0_MR_RDDAT2,DDR0_MR_RDDAT3);
	DDR0_MR_ADR = 40;
	DDR0_MR_WE  = 0;
	DDR0_MR_REQ = 1;
	while(DDR0_MR_REQ);
	if(DDR0_MR_RDDAT0!=0xffffffff) result++;
	if(DDR0_MR_RDDAT1!=0x00000000) result++;
	if(DDR0_MR_RDDAT2!=0xffffffff) result++;
	if(DDR0_MR_RDDAT3!=0x00000000) result++;
	//if(result>0) printf("%08x %08x %08x %08x\n", DDR0_MR_RDDAT0,DDR0_MR_RDDAT1,DDR0_MR_RDDAT2,DDR0_MR_RDDAT3);
	return result;
}
char ddr_rw_test()
{
	char result = 0;
	UINT *ADDR0_BASE = 0x80000000;
	hwflush_dcache_range(0x80000000, 0x40);
	ADDR0_BASE[0] = 0xffffffff;
	ADDR0_BASE[1] = 0;
	ADDR0_BASE[2] = 0xffffffff;
	ADDR0_BASE[3] = 0;
	ADDR0_BASE[4] = 0xffffffff;
	ADDR0_BASE[5] = 0xffffffff;
	ADDR0_BASE[6] = 0;
	ADDR0_BASE[7] = 0;
	ADDR0_BASE[8] = 0xaaaaaaaa;
	ADDR0_BASE[9] = 0x55555555;
	ADDR0_BASE[10] = 0xaaaaaaaa;
	ADDR0_BASE[11] = 0x55555555;
	ADDR0_BASE[12] = 0x55555555;
	ADDR0_BASE[13] = 0xaaaaaaaa;
	ADDR0_BASE[14] = 0x55555555;
	ADDR0_BASE[15] = 0xaaaaaaaa;
	hwflush_dcache_range(0x80000000, 0x40);
	if(ADDR0_BASE[0]!=0xffffffff) result++;
	if(ADDR0_BASE[1]!=0x00000000) result++;
	if(ADDR0_BASE[2]!=0xffffffff) result++;
	if(ADDR0_BASE[3]!=0x00000000) result++;
	if(ADDR0_BASE[4]!=0xffffffff) result++;
	if(ADDR0_BASE[5]!=0xffffffff) result++;
	if(ADDR0_BASE[6]!=0x00000000) result++;
	if(ADDR0_BASE[7]!=0x00000000) result++;
	if(ADDR0_BASE[8]!=0xaaaaaaaa) result++;
	if(ADDR0_BASE[9]!=0x55555555) result++;
	if(ADDR0_BASE[10]!=0xaaaaaaaa) result++;
	if(ADDR0_BASE[11]!=0x55555555) result++;
	if(ADDR0_BASE[12]!=0x55555555) result++;
	if(ADDR0_BASE[13]!=0xaaaaaaaa) result++;
	if(ADDR0_BASE[14]!=0x55555555) result++;
	if(ADDR0_BASE[15]!=0xaaaaaaaa) result++;
	//printf("%08x %08x %08x %08x %08x %08x %08x %08x\n", ADDR0_BASE[0], ADDR0_BASE[1], ADDR0_BASE[2], ADDR0_BASE[3], ADDR0_BASE[4], ADDR0_BASE[5], ADDR0_BASE[6], ADDR0_BASE[7]);
	return result;
}

void DdrGetId()
{
	DDR0_MR_ADR = 5;
	DDR0_MR_WE  = 0;
	DDR0_MR_REQ = 1;
	while(DDR0_MR_REQ);
	printf("MR5:%08x\n", DDR0_MR_RDDAT0);
	DDR0_MR_ADR = 8;
	DDR0_MR_WE  = 0;
	DDR0_MR_REQ = 1;
	while(DDR0_MR_REQ);
	printf("MR8:%08x\n", DDR0_MR_RDDAT0);
}

void DdrOn()
{
	if(!DDR0_PWR_CUR)
	{
		DDR0_PWR_REQ = 1;
		while(DDR0_PWR_REQ);
	}
}
void DdrOff()
{
	if(DDR0_PWR_CUR)
	{
		DDR0_PWR_REQ = 1;
		while(DDR0_PWR_REQ);
	}
}
void DdrOffOn()
{
	DdrOff();
	DdrOn();
}

void DdrInit(BYTE Sel)
{
	int i;
	char min=15, max=0;

	DdrOff();

	DDR0_MEM_SEL = Sel;// (0:128Mb S2/S4, 1:256Mb S2/S4, 2:512Mb S2/S4, 3:1Gb S2, 4:1Gb S4, 5: 2Gb S2, 6:2Gb S4, 7:4Gb S2/S4)
	DDR0_RD_EN_MASK  = 0x08;
	DDR0_RD_VAL_EDGE = 1;
	DDR0_RD_VAL_LTC	= 6;
	DDR0_MRR_VAL_LTC	= 6;
	DDR0_WR_LTC 		= 1;

	DdrOn();

	for(i=0;i<16;i++)
	{
		DDR0_DLY_CSOUT = i;
		CaOutDly(i);
		char temp = ddr_cal_test();
		if(temp!=0) printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
		DdrOffOn();
	}
	if(min==15){ printf("CA FAIL\n"); while(1);}
	CaOutDly((max+min+1)>>1);
	DDR0_DLY_CSOUT = (max+min+1)>>1;
	printf("DDR0_DLY_CA*OUT : %d  ",DDR0_DLY_CA9OUT);
	printf("DDR0_DLY_CSOUT : %d\n",DDR0_DLY_CSOUT);
	DdrOffOn();
	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DDR0_DLY_DQS0IN = i;
		char temp = ddr_cal_test();
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	if(min==15){ printf("DDR0_DLY_DQS0IN FAIL\n"); while(1);}
	DDR0_DLY_DQS0IN = min+2;
	printf("DDR0_DLY_DQS0IN : %d\n",DDR0_DLY_DQS0IN);

	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DDR0_DLY_DQS1IN = i;
		char temp = ddr_cal_test();
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	if(min==15){ printf("DDR0_DLY_DQS1IN FAIL\n"); while(1);}
	DDR0_DLY_DQS1IN = min+2;
	printf("DDR0_DLY_DQS1IN : %d\n",DDR0_DLY_DQS1IN);

	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DDR0_DLY_DQS2IN = i;
		char temp = ddr_cal_test();
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	if(min==15){ printf("DDR0_DLY_DQS2IN FAIL\n"); while(1);}
	DDR0_DLY_DQS2IN = min+2;
	printf("DDR0_DLY_DQS2IN : %d\n",DDR0_DLY_DQS2IN);

	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DDR0_DLY_DQS3IN = i;
		char temp = ddr_cal_test();
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	if(min==15){ printf("DDR0_DLY_DQS3IN FAIL\n"); while(1);}
	DDR0_DLY_DQS3IN = min+2;
	DDR0_RD_VAL_LTC	= 7;

	printf("DDR0_DLY_DQS3IN : %d\n",DDR0_DLY_DQS3IN);

	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DmOutDly(i);
		DqOutDly(i);
		char temp = ddr_rw_test();
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	if(min==15){ printf("DDR0_DMDQ_OUT FAIL\n"); while(1);}
	DmOutDly(min+2);
	DqOutDly(min+2);
	printf("DDR0_DMDQ_OUT : %d\n",min+2);

	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DDR0_DLY_DM0OUT = i;
		char temp = ddr_rw_test();
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	DDR0_DLY_DM0OUT = min+2;
	printf("DDR0_DLY_DM0OUT : %d\n",DDR0_DLY_DM0OUT);

	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DDR0_DLY_DM1OUT = i;
		char temp = ddr_rw_test();
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	DDR0_DLY_DM1OUT = min+2;
	printf("DDR0_DLY_DM1OUT : %d\n",DDR0_DLY_DM1OUT);
	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DDR0_DLY_DM2OUT = i;
		char temp = ddr_rw_test();
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	DDR0_DLY_DM2OUT = min+2;
	printf("DDR0_DLY_DM2OUT : %d\n",DDR0_DLY_DM2OUT);
	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DDR0_DLY_DM3OUT = i;
		char temp = ddr_rw_test();
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	DDR0_DLY_DM3OUT = min+2;
	printf("DDR0_DLY_DM3OUT : %d\n",DDR0_DLY_DM3OUT);

	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DqG0OutDly(i);
		char temp = ddr_rw_test();
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	DqG0OutDly(min+2);
	printf("DDR0_DQG0_OUT : %d\n",min+2);
	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DqG1OutDly(i);
		char temp = ddr_rw_test();
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	DqG1OutDly(min+2);
	printf("DDR0_DQG1_OUT : %d\n",min+2);
	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DqG2OutDly(i);
		char temp = ddr_rw_test();
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	DqG2OutDly(min+2);
	printf("DDR0_DQG2_OUT : %d\n",min+2);



	min=15, max=0;
	for(i=0;i<16;i++)
	{
		DqG3OutDly(i);
		char temp = ddr_rw_test();
		if(temp!=0)  printf("_");
		else
		{
			printf("*");
			if(min>i) min = i;
			if(max<i) max = i;
		}
	}
	DqG3OutDly(min+2);
	printf("DDR0_DQG3_OUT : %d\n",min+2);

	//DdrMapTest();
}

