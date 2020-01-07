OUTPUT_ARCH( "riscv" )

MEMORY
{
	FLASH   : ORIGIN = 0xC0000000,                              LENGTH = 0x10000
	FLAG    : ORIGIN = 0xC0000000 + LENGTH(FLASH) - 0x10,       LENGTH = 0x10
	DDR     : ORIGIN = 0x80000000,                              LENGTH = 0x100000
	RAM     : ORIGIN = 0xa0000000,                              LENGTH = 0x10000
}

/* stack and heap size */
stack_size  = 4*1024;
heap_size   = 256;

__ram_size  = ORIGIN(RAM) + LENGTH(RAM);
__stack_s   = __ram_size - 4;
__stack_e   = __ram_size - stack_size;
ENTRY( reset_vector )

SECTIONS
{

  /*--------------------------------------------------------------------*/
  /* Code and read-only segment                                         */
  /*--------------------------------------------------------------------*/

  /* Begining of code and text segment */

  _ftext = .;
  PROVIDE( eprol = . );

  .reset :
  {
  	. = ALIGN(32);
  	__reset_s = .;
    *(.text.init)
    . = ALIGN(32);
    __reset_e = .;
  } > FLASH

	__reset_lma_s = LOADADDR(.reset);
	__reset_lma_e = ( (LOADADDR(.reset) + SIZEOF(.reset) + 31) & ~ (32-1) );
	
  /* text: Program code section */
  .text : AT ( (LOADADDR (.reset) + SIZEOF(.reset) + 31) & ~ (32-1))
  {
  	. = ALIGN(32);
  	__text_s = .;
    *(.text)
    *(.text.*)
    *(.gnu.linkonce.t.*)
    . = ALIGN(32);
    __text_e = .;
  } > FLASH

	__text_lma_s = LOADADDR(.text);
	__text_lma_e = ( (LOADADDR(.text) + SIZEOF(.text) + 31) & ~ (32-1) );

	.ispm_text : AT ( (LOADADDR (.text) + SIZEOF(.text) + 31) & ~ (32-1))
	{
		. = ALIGN(32);
		__ispm_text_s = .;
		*(.ispmtext)
		*(.ispm_text_dummy)
		. = ALIGN(32);
		__ispm_text_e = .;
	} > RAM

	__ispm_text_lma_s = LOADADDR(.ispm_text);
	__ispm_text_lma_e = ( (LOADADDR(.ispm_text) + SIZEOF(.ispm_text) + 31) & ~ (32-1) );

	.ispm_data : AT ( (LOADADDR (.ispm_text) + SIZEOF(.ispm_text) + 31) & ~ (32-1))
	{
		. = ALIGN(32);
		__ispm_data_s = .;
		*(.ispmdata)
		. = ALIGN(32);
		__ispm_data_e = .;
	} > RAM

	__ispm_data_lma_s = LOADADDR(.ispm_data);
	__ispm_data_lma_e = ( (LOADADDR(.ispm_data) + SIZEOF(.ispm_data) + 31) & ~ (32-1) );
  /* rodata: Read-only data */
  .rodata : AT ( (LOADADDR (.ispm_data) + SIZEOF(.ispm_data) + 31) & ~ (32-1))
  {
  	. = ALIGN(32);
	__rodata_s = .;
    *(.rdata)
    *(.rodata)
    *(.rodata.*)
    *(.gnu.linkonce.r.*)
    . = ALIGN(32);
	__rodata_e = .;
  } > FLASH

  	__rodata_lma_s = LOADADDR(.rodata);
	__rodata_lma_e = ( (LOADADDR(.rodata) + SIZEOF(.rodata) + 31) & ~ (32-1) );

  /* End of code and read-only segment */
  PROVIDE( etext = . );
  _etext = .;

  /*--------------------------------------------------------------------*/
  /* Initialized data segment                                           */
  /*--------------------------------------------------------------------*/

  /* Start of initialized data segment */
  . = ALIGN(16);
   _fdata = .;

  /* data: Writable data */
  .data : AT ( (LOADADDR(.rodata) + SIZEOF(.rodata) + 31) & ~ (32-1) )
  {
  	. = ALIGN(32);
	__data_s = .;
    *(.data)
    *(.data.*)
    *(.srodata*)
    *(.gnu.linkonce.d.*)
    *(.comment)
    . = ALIGN(32);
	__data_e = .;
  } >RAM

   __data_lma_s = LOADADDR(.data);
   __data_lma_e = ( (LOADADDR(.data) + SIZEOF(.data) + 31) & ~ (32-1) );


  /* End of initialized data segment */
  . = ALIGN(4);
  PROVIDE( edata = . );
  _edata = .;

  /*--------------------------------------------------------------------*/
  /* Uninitialized data segment                                         */
  /*--------------------------------------------------------------------*/

  /* Start of uninitialized data segment */
  . = .;
  _fbss = .;

  /* sbss: Uninitialized writeable small data section */
  . = .;


	.flag_data :	/* for Dummy Data Gen */
	{
		*(.flagdata)
	} > FLAG

	__lma_size = ( (LOADADDR(.flag_data) + SIZEOF(.flag_data) + 31) & ~ (32-1) ) - LOADADDR(.reset);
	__lma_start = 0;	/* reserved */

  /* bss: Uninitialized writeable data section */
  . = .;
  _bss_start = .;
  .bss : 
  {
  	. = ALIGN(32);
  	__bss_s = .;
    *(.bss)
   
    *(.bss.*)
    *(.sbss*)
    *(.gnu.linkonce.b.*)
    *(COMMON)
    . = ALIGN(32);
 	 __bss_e = .; 
  } >RAM

  .sbi :
  {
    *(.sbi)
  }

  .payload :
  {
    *(.payload)
  }

  _end = .;
}
