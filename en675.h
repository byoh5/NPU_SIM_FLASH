#ifndef __EN675_H__
#define __EN675_H__

//******************************************************************************
// 1. TYPE DEF
//------------------------------------------------------------------------------
typedef _Bool					bool;		// 1 bit
typedef _Bool					BOOL;		//  "

typedef char					CHAR;		// 8 bit
typedef unsigned char			UCHAR;		//	"
typedef unsigned char			BYTE;		//	"
typedef unsigned char			uchar;		//	"
typedef signed   char			INT8S;		//	"
typedef unsigned char			INT8U;		//	"
typedef signed   char			s8;			//	"
typedef unsigned char			u8;			//	"
typedef signed   char			__s8;		//	"
typedef unsigned char			__u8;		//	"
typedef signed   char			s_int8;		//	"
typedef unsigned char			u_int8;		//	"
typedef signed   char			int8;		//	"
typedef unsigned char			uint8;		//	"
typedef volatile uint8			vuint8;		//	"

typedef short					SHORT;		// 16 bit
typedef unsigned short			USHORT;		//	"
typedef unsigned short			WORD;		//	"
typedef unsigned short			WCHAR;		//	"
typedef unsigned short			ushort;		//	"
typedef signed   short			INT16S;		//	"
typedef unsigned short			INT16U;		//	"
typedef signed   short			s16;		//	"
typedef unsigned short  		u16;		//	"
typedef signed   short			__s16;		//	"
typedef unsigned short  		__u16;		//	"
typedef signed   short			s_int16;	//	"
typedef unsigned short			u_int16;	//	"
typedef signed   short			int16;		//	"
typedef unsigned short			uint16;		//	"
typedef volatile uint16			vuint16;	//	"

typedef int						INT;		// 32 bit
typedef unsigned int			UINT;		//	"
typedef unsigned int			DWORD;		//	"
typedef unsigned int			uint;		//	"
typedef signed   int			INT32S;		//	"
typedef unsigned int			INT32U;		//	"
typedef signed   int			s32;		//	"
typedef unsigned int			u32;		//	"
typedef signed   int			__s32;		//	"
typedef unsigned int			__u32;		//	"
typedef signed   int			s_int32;	//	"
typedef unsigned int			u_int32;	//	"
typedef signed   int			int32;		//	"
typedef unsigned int			uint32;		//	"
typedef volatile uint32			vuint32;	//	"

typedef long					LONG;		// 64 bit
typedef unsigned long			uint64_t;	// 64 bit
typedef unsigned long			ULONG;		//	"
typedef unsigned long			QWORD;		//	"
typedef unsigned long			ulong;		//	"
typedef signed   long			INT64S;		//	"
typedef unsigned long			INT64U;		//	"
typedef signed   long			s64;		//	"
typedef unsigned long			u64;		//	"
typedef signed   long			__s64;		//	"
typedef unsigned long			__u64;		//	"
typedef signed   long			s_int64;	//	"
typedef unsigned long			u_int64;	//	"
typedef signed   long			int64;		//	"
typedef unsigned long			uint64;		//	"
typedef volatile uint64			vuint64;	//	"

//------------------------------------------------------------------------------
//
#define BF_1(a1) \
					a1;
#define BF_2(a1,a2)\
					a2;a1;
#define BF_3(a1,a2,a3)\
					a3;a2;a1;
#define BF_4(a1,a2,a3,a4)\
					a4;a3;a2;a1;
#define BF_5(a1,a2,a3,a4,a5)\
					a5;a4;a3;a2;a1;
#define BF_6(a1,a2,a3,a4,a5,a6)\
					a6;a5;a4;a3;a2;a1;
#define BF_7(a1,a2,a3,a4,a5,a6,a7)\
					a7;a6;a5;a4;a3;a2;a1;
#define BF_8(a1,a2,a3,a4,a5,a6,a7,a8)\
					a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_9(a1,a2,a3,a4,a5,a6,a7,a8,a9)\
					a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_10(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)\
					a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_11(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11)\
					a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_12(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)\
					a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_13(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13)\
					a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_14(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14)\
					a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_15(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15)\
					a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_16(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16)\
					a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_17(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17)\
					a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_18(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18)\
					a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_19(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19)\
					a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_20(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20)\
					a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_21(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21)\
					a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_22(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22)\
					a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_23(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23)\
					a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_24(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24)\
					a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_25(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25)\
					a25;a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_26(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26)\
					a26;a25;a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_27(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27)\
					a27;a26;a25;a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_28(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28)\
					a28;a27;a26;a25;a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_29(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29)\
					a29;a28;a27;a26;a25;a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_30(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30)\
					a30;a29;a28;a27;a26;a25;a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_31(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,a31)\
					a31;a30;a29;a28;a27;a26;a25;a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define BF_32(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,a31,a32)\
					a32;a31;a30;a29;a28;a27;a26;a25;a24;a23;a22;a21;a20;a19;a18;a17;a16;a15;a14;a13;a12;a11;a10;a9;a8;a7;a6;a5;a4;a3;a2;a1;
#define	_regs_					typedef	union{UINT a; struct{	// Register union start
#define	_rege_					};}							// Register union end
#define	_am(uon,base,addr)		((volatile uon*)(UINT*)(base+(addr)))->a		// All macro
#define	_bm(uon,base,addr,mne)	((volatile uon*)(UINT*)(base+(addr)))->mne	// Bit macro

//******************************************************************************
// 2. Memory Size & Base Address Definition
//------------------------------------------------------------------------------
// Mem size
#define SFLS_SIZE				(16*1024*1024)	//	 16MB
#define DDR0_SIZE				(64*1024*1024)	//	 64MB	-	AP Memory AD251232 , Winbond W979H2KK , Etron EM6KA32HGDA
#define DDR1_SIZE				(64*1024*1024)	//	 64MB	-	AP Memory AD251232 , Winbond W979H2KK , Etron EM6KA32HGDA
//#define DDR_SIZE				(128*1024*1024)	//	128MB	-	AP Memory 1Gb , Fidelix 1Gb, Winbond W97AH2KK
//#define DDR_SIZE				(256*1024*1024)	//	256MB	-	AP Memory AD220032
#define RAM_SIZE				(    256*1024)	//	256kB

// Base address
#define SFLS_BASE				0xC0000000
#define DDR0_BASE				0x80000000
#define DDR1_BASE				0x90000000
#define RAM_BASE				0xA0000000

// RegBase address
#define REG_BASE_ISP			0x40000000
#define REG_BASE_H265			0x40100000
#define REG_BASE_H264       	0x40200000
#define REG_BASE_DDR0        	0x40300000
#define REG_BASE_DDR1       	0x40400000
#define REG_BASE_SFLS       	0x40500000
#define REG_BASE_SDIO0      	0x40600000
#define REG_BASE_SDIO1      	0x40700000
#define REG_BASE_ETH        	0x40800000
#define REG_BASE_AUD        	0x40900000
#define REG_BASE_UTIL       	0x40A00000
#define REG_BASE_GPIO       	0x40B00000
#define REG_BASE_SPI0       	0x40C00000
#define REG_BASE_SPI1       	0x40D00000
#define REG_BASE_SPI2       	0x40E00000
#define REG_BASE_SPI3       	0x40F00000
#define REG_BASE_SPI4       	0x41000000
#define REG_BASE_SPI5       	0x41100000
#define REG_BASE_SPI6       	0x41200000
#define REG_BASE_SPI7       	0x41300000
#define REG_BASE_SPI8       	0x41400000
#define REG_BASE_I2C0       	0x41500000
#define REG_BASE_I2C1       	0x41600000
#define REG_BASE_I2C2       	0x41700000
#define REG_BASE_I2C3       	0x41800000
#define REG_BASE_I2C4       	0x41900000
#define REG_BASE_I2C5       	0x41A00000
#define REG_BASE_I2C6       	0x41B00000
#define REG_BASE_I2C7       	0x41C00000
#define REG_BASE_I2C8       	0x41D00000
#define REG_BASE_TIMER0     	0x41E00000
#define REG_BASE_TIMER1     	0x41F00000
#define REG_BASE_TIMER2     	0x42000000
#define REG_BASE_TIMER3     	0x42100000
#define REG_BASE_TIMER4     	0x42200000
#define REG_BASE_TIMER5     	0x42300000
#define REG_BASE_TIMER6     	0x42400000
#define REG_BASE_TIMER7     	0x42500000
#define REG_BASE_TIMER8     	0x42600000
#define REG_BASE_TIMER9     	0x42700000
#define REG_BASE_TIMER10    	0x42800000
#define REG_BASE_TIMER11    	0x42900000
#define REG_BASE_TIMER12    	0x42A00000
#define REG_BASE_TIMER13    	0x42B00000
#define REG_BASE_TIMER14    	0x42C00000
#define REG_BASE_TIMER15    	0x42D00000
#define REG_BASE_TIMER16    	0x42E00000
#define REG_BASE_TIMER17    	0x42F00000
#define REG_BASE_TIMER18    	0x43000000
#define REG_BASE_TIMER19    	0x43100000
#define REG_BASE_TIMER20    	0x43200000
#define REG_BASE_TIMER21    	0x43300000
#define REG_BASE_TIMER22    	0x43400000
#define REG_BASE_TIMER23    	0x43500000
#define REG_BASE_TIMER24    	0x43600000
#define REG_BASE_TIMER25    	0x43700000
#define REG_BASE_TIMER26    	0x43800000
#define REG_BASE_TIMER27    	0x43900000
#define REG_BASE_TIMER28    	0x43A00000
#define REG_BASE_TIMER29    	0x43B00000
#define REG_BASE_TIMER30    	0x43C00000
#define REG_BASE_TIMER31    	0x43D00000
#define REG_BASE_TIMER32    	0x43E00000
#define REG_BASE_TIMER33    	0x43F00000
#define REG_BASE_TIMER34    	0x44000000
#define REG_BASE_TIMER35    	0x44100000
#define REG_BASE_TIMER36    	0x44200000
#define REG_BASE_TIMER37    	0x44300000
#define REG_BASE_TIMER38    	0x44400000
#define REG_BASE_UART0      	0x44500000
#define REG_BASE_UART1      	0x44600000
#define REG_BASE_UART2      	0x44700000
#define REG_BASE_UART3      	0x44800000
#define REG_BASE_UART4      	0x44900000
#define REG_BASE_UART5      	0x44A00000
#define REG_BASE_UART6      	0x44B00000
#define REG_BASE_UART7      	0x44C00000
#define REG_BASE_UART8      	0x44D00000
#define REG_BASE_AES			0x44E00000
#define REG_BASE_SHA			0x44F00000
#define REG_BASE_CHKSUM			0x45000000
#define REG_BASE_SYS			0x45100000
#define REG_BASE_BDMA0			0x45200000
#define REG_BASE_BDMA1			0x45200020
#define REG_BASE_BDMA2			0x45200040
#define REG_BASE_BDMA3			0x45200060
#define REG_BASE_CDMA0			0x44b00000//0x44b00000 //0x45300000
#define REG_BASE_CDMA1			0x45300020
#define REG_BASE_CDMA2			0x45300040
#define REG_BASE_CDMA3			0x45300060
#define REG_BASE_I2S			0x45400000
#define REG_BASE_IRQ			0x45500000
#define REG_BASE_ADC			0x45600000
#define REG_BASE_BUS			0x45700000


//******************************************************************************
// 3. System frequency
//------------------------------------------------------------------------------

#define OSC_FREQ				(25*1000000)
#define MCK_FREQ				(50*1000000)
#define printf					_printf
#endif // End of __EN675_H__
