#include "en675.h"
#include "reg.h"
#include "dma.h"
#include "rv_utils.h"

void dma_copy(UINT dst, UINT src, UINT len)
{
	hwflush_dcache_range(src,src+len);
	hwflush_dcache_range(dst,dst+len);
	BDMA0_JOB_PTR++;
	BDMA0_MODE = 0;
	BDMA0_SRC = src;
	BDMA0_DST = dst;
	BDMA0_LEN = len;
	BDMA0_GO = 1;
	while(BDMA0_DONE_PTR!=BDMA0_JOB_PTR){
		//_printf("!");
	}

}

void dma_set(UINT dst, UINT len, BYTE value)
{
	BDMA0_JOB_PTR++;
	BDMA0_VALUE = value;
	BDMA0_MODE = 1;
	BDMA0_DST = dst;
	BDMA0_LEN = len;
	BDMA0_GO = 1;
	while(BDMA0_DONE_PTR!=BDMA0_JOB_PTR);
}
void cdma_copy(UINT dst, UINT src, UINT len)
{
//	hwflush_dcache_range_(src,len);
	CDMA0_JOB_PTR++;
	CDMA0_MODE = 0;
	CDMA0_SRC = src;
	CDMA0_DST = dst;
	CDMA0_LEN = len;
	CDMA0_GO = 1;
	while(CDMA0_DONE_PTR!=CDMA0_JOB_PTR);
}

void cdma_set(UINT dst, UINT len, BYTE value)
{
	CDMA0_JOB_PTR++;
	CDMA0_VALUE = value;
	CDMA0_MODE = 1;
	CDMA0_DST = dst;
	CDMA0_LEN = len;
	CDMA0_GO = 1;
	while(CDMA0_DONE_PTR!=CDMA0_JOB_PTR);
}
