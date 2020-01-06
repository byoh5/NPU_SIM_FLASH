/*
 * boot.c
 *
 *  Created on: 2018. 11. 29.
 *      Author: BYOH
 */
#include <stdint.h>
#include <string.h>

const unsigned int _bss_s;
const unsigned int _bss_e;

void _init_bss_section(void)
{
	char *pDst = (char*)(intptr_t)(_bss_s);
	if( pDst == 0) return;
	int n = _bss_e - _bss_s;
	int i;
	for(i=0; i<n; i++) *pDst++ = 0;

	return;
}

const unsigned int _data_s;
const unsigned int _data_e;
const unsigned int _data_lma_s;

void _init_data_section(void)
{
	memcpy((void*)(intptr_t)_data_s, (void*)(intptr_t)_data_lma_s, (size_t)(_data_e - _data_s));
}
