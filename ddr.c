//orbit_init_new_1();
//orbit_init_new_2();	// DFS Driver included

#include "reg.h"
#include "en675.h"
#include "encoding.h"

#define		OMC_APB_BASE_ADDR			0x46200000
#define		PHY_APB_BASE_ADDR			0x46400000
#define		OMC_SECURE_APB_BASE_ADDR	0x46201000

//#define CPU_FREQ				(75*1000*1000)
#define CPU_FREQ                (600*1000*1000)

void WaitXns(uint64_t ns)
{    // Wait X usec
    uint64_t clk_period = ns;
    uint64_t start_clk = rdcycle();
    uint64_t clk;
    do {
        uint64_t stop_clk = rdcycle();
        clk = stop_clk - start_clk;
        if (stop_clk < start_clk) {
            clk = ~clk + 1;    // neg --> abs
        }
    } while(clk < clk_period);
}

void WaitXus(uint64_t us)
{	// Wait X usec
	uint64_t clk_period = (CPU_FREQ / (1000 * 1000)) * us;
	uint64_t start_clk = rdcycle();
	uint64_t clk;
	do {
		uint64_t stop_clk = rdcycle();
		clk = stop_clk - start_clk;
		if (stop_clk < start_clk) {
			clk = ~clk + 1;	// neg --> abs
		}
	} while(clk < clk_period);
}

void WaitXms(uint64_t ms)
{	// Wait X msec
	WaitXus(ms * 1000);
}

// Sleep us micro-seconds
void wait_usleep(int us)
{
    WaitXus(us);
}

void apb_write(volatile unsigned int *addr, unsigned int data)
{
	*((unsigned int *)(addr)) = data;
}

unsigned int apb_read(volatile unsigned int *addr)
{
	return *((unsigned int *)(addr));
}

void orbit_init(void)
{
	// New_1
apb_write(PHY_APB_BASE_ADDR + 0x0, 0x4708080c);
apb_write(PHY_APB_BASE_ADDR + 0xc, 0x42101000);
apb_write(PHY_APB_BASE_ADDR + 0x18, 0x0000000f);
apb_write(PHY_APB_BASE_ADDR + 0x4c, 0x00000000);
apb_write(PHY_APB_BASE_ADDR + 0x40, 0x00000000);
apb_write(PHY_APB_BASE_ADDR + 0x44, 0x00000000);
apb_write(PHY_APB_BASE_ADDR + 0x0, 0x4707080c);
apb_write(PHY_APB_BASE_ADDR + 0xd0, 0x0f0c7304);
apb_write(PHY_APB_BASE_ADDR + 0xd0, 0x0f0c7306);
while ((apb_read(PHY_APB_BASE_ADDR + 0xd4) & 0x00000001) != 0x00000001) {
  wait_usleep(1);
}
apb_write(PHY_APB_BASE_ADDR + 0xd0, 0x0f0c7304);
apb_write(PHY_APB_BASE_ADDR + 0xd0, 0x0f087304);
apb_write(OMC_APB_BASE_ADDR + 0x0, 0x00000400);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf00, 0x40001010);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf04, 0x00800001);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf10, 0x00800000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf14, 0x008fffff);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf18, 0x00000000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf30, 0x03000010);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf30, 0x03000000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0x110, 0xc0000001);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0x114, 0xffffffff);
apb_write(OMC_APB_BASE_ADDR + 0x10c, 0x00000101);
apb_write(OMC_APB_BASE_ADDR + 0x11c, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0x500, 0x00000201);
apb_write(OMC_APB_BASE_ADDR + 0x504, 0x40000000);
while ((apb_read(OMC_APB_BASE_ADDR + 0x504) & 0x80000000) != 0x80000000) {
  wait_usleep(1);
}
apb_write(OMC_APB_BASE_ADDR + 0x504, 0x00000000);
apb_write(PHY_APB_BASE_ADDR + 0x50, 0x01004700);
apb_write(PHY_APB_BASE_ADDR + 0x50, 0x00004700);
// tINIT0 is controlled by System
// Drive CKE high
apb_write(OMC_APB_BASE_ADDR + 0x10, 0x0000001c);
apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
// Waits tINIT3 (200 us): Minimum idle time after first CKE assertion
wait_usleep(200);
// Issue RESET (MRW) command
apb_write(OMC_APB_BASE_ADDR + 0x4, 0x103f00fc);
apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
// Waits tINIT4 (1 us): Minimum idle time after RESET command
wait_usleep(1);
// Waits tINIT5 (9 us): Maximum duration of device auto initialization
// You can do polling on MR status to finish auto initialization part early
wait_usleep(9);
apb_write(OMC_APB_BASE_ADDR + 0x4, 0x100a00ff);
apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
// Waits tZQINIT (1 us)
wait_usleep(1);
// Raise up frequency of PLL for highest working condition
// Waits until frequency is stable
// Write down RCLK-related CRs
apb_write(OMC_APB_BASE_ADDR + 0x600, 0x002e0176);
apb_write(OMC_APB_BASE_ADDR + 0x604, 0x002e0176);
apb_write(OMC_APB_BASE_ADDR + 0x608, 0x001700bb);
apb_write(OMC_APB_BASE_ADDR + 0x60c, 0x000b005d);
apb_write(OMC_APB_BASE_ADDR + 0x610, 0x0005002e);
apb_write(OMC_APB_BASE_ADDR + 0x614, 0x00020017);
apb_write(OMC_APB_BASE_ADDR + 0x618, 0x00020017);
apb_write(OMC_APB_BASE_ADDR + 0x61c, 0x00020017);
apb_write(OMC_APB_BASE_ADDR + 0x678, 0x00000019);
apb_write(OMC_APB_BASE_ADDR + 0x100, 0x000000e0);
apb_write(OMC_APB_BASE_ADDR + 0x620, 0x00000f11);
apb_write(OMC_APB_BASE_ADDR + 0x624, 0x060c0206);
apb_write(OMC_APB_BASE_ADDR + 0x628, 0x3022070f);
apb_write(OMC_APB_BASE_ADDR + 0x62c, 0x17110928);
apb_write(OMC_APB_BASE_ADDR + 0x630, 0x00000c06);
apb_write(OMC_APB_BASE_ADDR + 0x634, 0x00100000);
apb_write(OMC_APB_BASE_ADDR + 0x638, 0x000c0c04);
apb_write(OMC_APB_BASE_ADDR + 0x63c, 0x16100003);
apb_write(OMC_APB_BASE_ADDR + 0x640, 0x1070152a);
apb_write(OMC_APB_BASE_ADDR + 0x644, 0x00310068);
apb_write(OMC_APB_BASE_ADDR + 0x648, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0x64c, 0x0006170c);
apb_write(OMC_APB_BASE_ADDR + 0x650, 0x1033240a);
apb_write(OMC_APB_BASE_ADDR + 0x654, 0x00001310);
apb_write(OMC_APB_BASE_ADDR + 0x658, 0x00000060);
apb_write(OMC_APB_BASE_ADDR + 0x65c, 0x00480120);
apb_write(OMC_APB_BASE_ADDR + 0x660, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0x680, 0x18000418);
apb_write(OMC_APB_BASE_ADDR + 0x684, 0x05000502);
apb_write(OMC_APB_BASE_ADDR + 0x688, 0x120b0404);
apb_write(OMC_APB_BASE_ADDR + 0x68c, 0x20003520);
apb_write(OMC_APB_BASE_ADDR + 0x690, 0x00140000);
apb_write(OMC_APB_BASE_ADDR + 0x69c, 0x01240074);
apb_write(OMC_APB_BASE_ADDR + 0x6a0, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0x6a4, 0x20350c00);
apb_write(OMC_APB_BASE_ADDR + 0x6a8, 0x00040000);
apb_write(OMC_APB_BASE_ADDR + 0x4, 0x10010043);
apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
apb_write(OMC_APB_BASE_ADDR + 0x4, 0x1002001a);
apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
apb_write(PHY_APB_BASE_ADDR + 0x64, 0x0000000e);
apb_write(PHY_APB_BASE_ADDR + 0x68, 0x0000000f);
apb_write(PHY_APB_BASE_ADDR + 0xb0, 0x10900070);
apb_read(PHY_APB_BASE_ADDR + 0xb4);
apb_write(PHY_APB_BASE_ADDR + 0xb0, 0x10902ff0);
apb_write(PHY_APB_BASE_ADDR + 0xb0, 0x10902fd0);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x04200040);
apb_write(PHY_APB_BASE_ADDR + 0xc, 0x6b101000);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c200042);
apb_write(PHY_APB_BASE_ADDR + 0x64, 0x0000000e);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c200242);
while ((apb_read(PHY_APB_BASE_ADDR + 0x4) & 0x00020000) != 0x00020000) {
  wait_usleep(1);
}
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c200042);
apb_write(PHY_APB_BASE_ADDR + 0x10, 0x37ff5000);
apb_read(PHY_APB_BASE_ADDR + 0x108);
apb_read(PHY_APB_BASE_ADDR + 0xfc);
apb_read(PHY_APB_BASE_ADDR + 0x100);
apb_read(PHY_APB_BASE_ADDR + 0x104);
apb_write(PHY_APB_BASE_ADDR + 0x64, 0x0000000e);
apb_write(PHY_APB_BASE_ADDR + 0xc, 0x49001000);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20004a);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20084a);
while ((apb_read(PHY_APB_BASE_ADDR + 0x4) & 0x00020000) != 0x00020000) {
  wait_usleep(1);
}
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20004a);
apb_read(PHY_APB_BASE_ADDR + 0xec);
apb_read(PHY_APB_BASE_ADDR + 0xf0);
apb_read(PHY_APB_BASE_ADDR + 0xf4);
apb_write(OMC_APB_BASE_ADDR + 0x10, 0x00000014);
apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20006a);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20206a);
while ((apb_read(PHY_APB_BASE_ADDR + 0x4) & 0x00020000) != 0x00020000) {
  wait_usleep(1);
}
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20006a);
apb_read(PHY_APB_BASE_ADDR + 0xec);
apb_read(PHY_APB_BASE_ADDR + 0xf0);
apb_read(PHY_APB_BASE_ADDR + 0xf4);
apb_read(PHY_APB_BASE_ADDR + 0xf8);
apb_write(OMC_APB_BASE_ADDR + 0x10, 0x00000017);
apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20002a);
apb_write(PHY_APB_BASE_ADDR + 0xb0, 0x10902ff0);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c60002a);
apb_write(PHY_APB_BASE_ADDR + 0x50, 0x01004700);
apb_write(PHY_APB_BASE_ADDR + 0x50, 0x00004700);
apb_write(OMC_APB_BASE_ADDR + 0x410, 0x00101010);
apb_write(OMC_APB_BASE_ADDR + 0x420, 0x0c181006);
apb_write(OMC_APB_BASE_ADDR + 0x424, 0x20200820);
apb_write(OMC_APB_BASE_ADDR + 0x428, 0x80000020);
apb_write(OMC_APB_BASE_ADDR + 0x0, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0x108, 0x00003000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0x704, 0x00000007);
apb_write(OMC_APB_BASE_ADDR + 0x330, 0x09311fff);
apb_write(OMC_APB_BASE_ADDR + 0x334, 0x00000002);
apb_write(OMC_APB_BASE_ADDR + 0x324, 0x00002000);
apb_write(OMC_APB_BASE_ADDR + 0x104, 0x80000000);
apb_write(OMC_APB_BASE_ADDR + 0x510, 0x00180000);
apb_write(OMC_APB_BASE_ADDR + 0x514, 0x00000000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0x700, 0x00000003);
apb_write(OMC_APB_BASE_ADDR + 0x20, 0x00000001);
// Initialization done
}

void orbit_init_dfs(void)
{
//New_2
apb_write(PHY_APB_BASE_ADDR + 0x0, 0x4708080c);
apb_write(PHY_APB_BASE_ADDR + 0xc, 0x42101000);
apb_write(PHY_APB_BASE_ADDR + 0x18, 0x0000000f);
apb_write(PHY_APB_BASE_ADDR + 0x4c, 0x00000000);
apb_write(PHY_APB_BASE_ADDR + 0x40, 0x00000000);
apb_write(PHY_APB_BASE_ADDR + 0x44, 0x00000000);
apb_write(PHY_APB_BASE_ADDR + 0x0, 0x4707080c);
apb_write(PHY_APB_BASE_ADDR + 0xd0, 0x0f0c7304);
apb_write(PHY_APB_BASE_ADDR + 0xd0, 0x0f0c7306);
while ((apb_read(PHY_APB_BASE_ADDR + 0xd4) & 0x00000001) != 0x00000001) {
  wait_usleep(1);
}
apb_write(PHY_APB_BASE_ADDR + 0xd0, 0x0f0c7304);
apb_write(PHY_APB_BASE_ADDR + 0xd0, 0x0f087304);
apb_write(OMC_APB_BASE_ADDR + 0x0, 0x00000400);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf00, 0x40001010);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf04, 0x00800001);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf10, 0x00800000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf14, 0x009fffff);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf18, 0x00000000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf30, 0x00000000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0x110, 0xc0000001);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0x114, 0xffffffff);
apb_write(OMC_APB_BASE_ADDR + 0x10c, 0x00000101);
apb_write(OMC_APB_BASE_ADDR + 0x11c, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0x500, 0x00000201);
apb_write(OMC_APB_BASE_ADDR + 0x504, 0x40000000);
while ((apb_read(OMC_APB_BASE_ADDR + 0x504) & 0x80000000) != 0x80000000) {
  wait_usleep(1);
}
apb_write(OMC_APB_BASE_ADDR + 0x504, 0x00000000);
apb_write(PHY_APB_BASE_ADDR + 0x50, 0x01004700);
apb_write(PHY_APB_BASE_ADDR + 0x50, 0x00004700);
// tINIT0 is controlled by System
// Drive CKE high
apb_write(OMC_APB_BASE_ADDR + 0x10, 0x0000001c);
apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
// Waits tINIT3 (200 us): Minimum idle time after first CKE assertion
wait_usleep(200);
// Issue RESET (MRW) command
apb_write(OMC_APB_BASE_ADDR + 0x4, 0x103f00fc);
apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
// Waits tINIT4 (1 us): Minimum idle time after RESET command
wait_usleep(1);
// Waits tINIT5 (9 us): Maximum duration of device auto initialization
// You can do polling on MR status to finish auto initialization part early
wait_usleep(9);
apb_write(OMC_APB_BASE_ADDR + 0x4, 0x100a00ff);
apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
// Waits tZQINIT (1 us)
wait_usleep(1);
// Raise up frequency of PLL for highest working condition
// Waits until frequency is stable
// Write down RCLK-related CRs
apb_write(OMC_APB_BASE_ADDR + 0x600, 0x002e0176);
apb_write(OMC_APB_BASE_ADDR + 0x604, 0x002e0176);
apb_write(OMC_APB_BASE_ADDR + 0x608, 0x001700bb);
apb_write(OMC_APB_BASE_ADDR + 0x60c, 0x000b005d);
apb_write(OMC_APB_BASE_ADDR + 0x610, 0x0005002e);
apb_write(OMC_APB_BASE_ADDR + 0x614, 0x00020017);
apb_write(OMC_APB_BASE_ADDR + 0x618, 0x00020017);
apb_write(OMC_APB_BASE_ADDR + 0x61c, 0x00020017);
apb_write(OMC_APB_BASE_ADDR + 0x678, 0x00000019);
apb_write(OMC_APB_BASE_ADDR + 0x100, 0x000000e0);
apb_write(OMC_APB_BASE_ADDR + 0x620, 0x00000f11);
apb_write(OMC_APB_BASE_ADDR + 0x624, 0x060c0206);
apb_write(OMC_APB_BASE_ADDR + 0x628, 0x3022070f);
apb_write(OMC_APB_BASE_ADDR + 0x62c, 0x17110928);
apb_write(OMC_APB_BASE_ADDR + 0x630, 0x00000c06);
apb_write(OMC_APB_BASE_ADDR + 0x634, 0x00100000);
apb_write(OMC_APB_BASE_ADDR + 0x638, 0x000c0c04);
apb_write(OMC_APB_BASE_ADDR + 0x63c, 0x16100003);
apb_write(OMC_APB_BASE_ADDR + 0x640, 0x1070152a);
apb_write(OMC_APB_BASE_ADDR + 0x644, 0x00310068);
apb_write(OMC_APB_BASE_ADDR + 0x648, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0x64c, 0x0006170c);
apb_write(OMC_APB_BASE_ADDR + 0x650, 0x1033240a);
apb_write(OMC_APB_BASE_ADDR + 0x654, 0x00001310);
apb_write(OMC_APB_BASE_ADDR + 0x658, 0x00000060);
apb_write(OMC_APB_BASE_ADDR + 0x65c, 0x00480120);
apb_write(OMC_APB_BASE_ADDR + 0x660, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0x680, 0x18000418);
apb_write(OMC_APB_BASE_ADDR + 0x684, 0x05000502);
apb_write(OMC_APB_BASE_ADDR + 0x688, 0x120b0404);
apb_write(OMC_APB_BASE_ADDR + 0x68c, 0x20003520);
apb_write(OMC_APB_BASE_ADDR + 0x690, 0x00140000);
apb_write(OMC_APB_BASE_ADDR + 0x69c, 0x01240074);
apb_write(OMC_APB_BASE_ADDR + 0x6a0, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0x6a4, 0x20350c00);
apb_write(OMC_APB_BASE_ADDR + 0x6a8, 0x00040000);
apb_write(OMC_APB_BASE_ADDR + 0x4, 0x10010043);
apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
apb_write(OMC_APB_BASE_ADDR + 0x4, 0x1002001a);
apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
apb_write(PHY_APB_BASE_ADDR + 0x64, 0x0000000e);
apb_write(PHY_APB_BASE_ADDR + 0x68, 0x0000000f);
apb_write(PHY_APB_BASE_ADDR + 0xb0, 0x10900070);
apb_read(PHY_APB_BASE_ADDR + 0xb4);
apb_write(PHY_APB_BASE_ADDR + 0xb0, 0x10902ff0);
apb_write(PHY_APB_BASE_ADDR + 0xb0, 0x10902fd0);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x04200040);
apb_write(PHY_APB_BASE_ADDR + 0xc, 0x6b101000);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c200042);
apb_write(PHY_APB_BASE_ADDR + 0x64, 0x0000000e);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c200242);
while ((apb_read(PHY_APB_BASE_ADDR + 0x4) & 0x00020000) != 0x00020000) {
  wait_usleep(1);
}
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c200042);
apb_write(PHY_APB_BASE_ADDR + 0x10, 0x37ff5000);
apb_read(PHY_APB_BASE_ADDR + 0x108);
apb_read(PHY_APB_BASE_ADDR + 0xfc);
apb_read(PHY_APB_BASE_ADDR + 0x100);
apb_read(PHY_APB_BASE_ADDR + 0x104);
apb_write(PHY_APB_BASE_ADDR + 0x64, 0x0000000e);
apb_write(PHY_APB_BASE_ADDR + 0xc, 0x49001000);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20004a);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20084a);
while ((apb_read(PHY_APB_BASE_ADDR + 0x4) & 0x00020000) != 0x00020000) {
  wait_usleep(1);
}
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20004a);
apb_read(PHY_APB_BASE_ADDR + 0xec);
apb_read(PHY_APB_BASE_ADDR + 0xf0);
apb_read(PHY_APB_BASE_ADDR + 0xf4);
apb_write(OMC_APB_BASE_ADDR + 0x10, 0x00000014);
apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20006a);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20206a);
while ((apb_read(PHY_APB_BASE_ADDR + 0x4) & 0x00020000) != 0x00020000) {
  wait_usleep(1);
}
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20006a);
apb_read(PHY_APB_BASE_ADDR + 0xec);
apb_read(PHY_APB_BASE_ADDR + 0xf0);
apb_read(PHY_APB_BASE_ADDR + 0xf4);
apb_read(PHY_APB_BASE_ADDR + 0xf8);
apb_write(OMC_APB_BASE_ADDR + 0x10, 0x00000017);
apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20002a);
apb_write(PHY_APB_BASE_ADDR + 0xb0, 0x10902ff0);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c60002a);
apb_write(PHY_APB_BASE_ADDR + 0x50, 0x01004700);
apb_write(PHY_APB_BASE_ADDR + 0x50, 0x00004700);
apb_write(OMC_APB_BASE_ADDR + 0x410, 0x00101010);
apb_write(OMC_APB_BASE_ADDR + 0x420, 0x0c181006);
apb_write(OMC_APB_BASE_ADDR + 0x424, 0x20200820);
apb_write(OMC_APB_BASE_ADDR + 0x428, 0x80000020);
apb_write(OMC_APB_BASE_ADDR + 0x0, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0x108, 0x00003000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0x704, 0x00000007);
apb_write(OMC_APB_BASE_ADDR + 0x330, 0x09311fff);
apb_write(OMC_APB_BASE_ADDR + 0x334, 0x00000002);
apb_write(OMC_APB_BASE_ADDR + 0x324, 0x00002000);
apb_write(OMC_APB_BASE_ADDR + 0x104, 0x80000000);
apb_write(OMC_APB_BASE_ADDR + 0x510, 0x00180000);
apb_write(OMC_APB_BASE_ADDR + 0x514, 0x00000000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0x700, 0x00000003);
apb_write(OMC_APB_BASE_ADDR + 0x20, 0x00000001);

apb_write(OMC_APB_BASE_ADDR + 0x900, 0x000000f0);
apb_write(OMC_APB_BASE_ADDR + 0xe20, 0x00000809);
apb_write(OMC_APB_BASE_ADDR + 0xe24, 0x03060203);
apb_write(OMC_APB_BASE_ADDR + 0xe28, 0x18110408);
apb_write(OMC_APB_BASE_ADDR + 0xe2c, 0x0e0c0414);
apb_write(OMC_APB_BASE_ADDR + 0xe30, 0x00000603);
apb_write(OMC_APB_BASE_ADDR + 0xe34, 0x000b0000);
apb_write(OMC_APB_BASE_ADDR + 0xe38, 0x000a0a04);
apb_write(OMC_APB_BASE_ADDR + 0xe3c, 0x0e100002);
apb_write(OMC_APB_BASE_ADDR + 0xe40, 0x10380b10);
apb_write(OMC_APB_BASE_ADDR + 0xe44, 0x30180034);
apb_write(OMC_APB_BASE_ADDR + 0xe48, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0xe4c, 0x00040e06);
apb_write(OMC_APB_BASE_ADDR + 0xe50, 0x081a1205);
apb_write(OMC_APB_BASE_ADDR + 0xe54, 0x00000a08);
apb_write(OMC_APB_BASE_ADDR + 0xe58, 0x00000060);
apb_write(OMC_APB_BASE_ADDR + 0xe5c, 0x00240090);
apb_write(OMC_APB_BASE_ADDR + 0xe60, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0xe80, 0x0e00040e);
apb_write(OMC_APB_BASE_ADDR + 0xe84, 0x02000202);
apb_write(OMC_APB_BASE_ADDR + 0xe88, 0x120b0404);
apb_write(OMC_APB_BASE_ADDR + 0xe8c, 0x20003020);
apb_write(OMC_APB_BASE_ADDR + 0xe90, 0x00140000);
apb_write(OMC_APB_BASE_ADDR + 0xe9c, 0x01240074);
apb_write(OMC_APB_BASE_ADDR + 0xea0, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0xea4, 0x20300c00);
apb_write(OMC_APB_BASE_ADDR + 0xea8, 0x00040000);
apb_write(OMC_APB_BASE_ADDR + 0x310, 0x40000030);
apb_write(OMC_APB_BASE_ADDR + 0x340, 0x80010083);
apb_write(OMC_APB_BASE_ADDR + 0x344, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0x348, 0x80020004);
apb_write(OMC_APB_BASE_ADDR + 0x34c, 0x00000000);
// dvfs_level: 2, dvfs_mode: 2
apb_write(PHY_APB_BASE_ADDR + 0xb8, 0x8a000000);
apb_write(PHY_APB_BASE_ADDR + 0xe0, 0x00000000);
apb_write(PHY_APB_BASE_ADDR + 0xbc, 0x060c0122);
apb_write(PHY_APB_BASE_ADDR + 0xc0, 0x63f00221);
apb_write(PHY_APB_BASE_ADDR + 0x3d4, 0x00000000);
apb_write(PHY_APB_BASE_ADDR + 0x3d8, 0x00000000);

// Initialization done

SYS_OMC_DFS_SET = 0x7;
while(!SYS_OMC_PDONE);
SYS_OMC_DFS_SET = 0x1;

//while(SYS_OMC_DFS_SET==0x6 && SYS_OMC_PACCETP);
// SYS_OMC_DFS_SET = 0x1;
}

void orbit_init_dfs_233MHz(void)
{
UINT Temp0;
UINT Temp1;

apb_write(PHY_APB_BASE_ADDR + 0x0, 0x4708080c);
apb_write(PHY_APB_BASE_ADDR + 0xc, 0x42101000);
apb_write(PHY_APB_BASE_ADDR + 0x18, 0x0000000f);
apb_write(PHY_APB_BASE_ADDR + 0x4c, 0x00000000);
apb_write(PHY_APB_BASE_ADDR + 0x40, 0x00000000);
apb_write(PHY_APB_BASE_ADDR + 0x44, 0x00000000);
apb_write(PHY_APB_BASE_ADDR + 0x0, 0x4707080c);
apb_write(PHY_APB_BASE_ADDR + 0xd0, 0x0f0c7304);
apb_write(PHY_APB_BASE_ADDR + 0xd0, 0x0f0c7306);
while ((apb_read(PHY_APB_BASE_ADDR + 0xd4) & 0x00000001) != 0x00000001) {
  wait_usleep(1);
}
apb_write(PHY_APB_BASE_ADDR + 0xd0, 0x0f0c7304);
apb_write(PHY_APB_BASE_ADDR + 0xd0, 0x0f087304);
apb_write(OMC_APB_BASE_ADDR + 0x0, 0x00000400);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf00, 0x40001010);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf04, 0x00800001);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf10, 0x00800000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf14, 0x0087ffff);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf18, 0x00000000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0xf30, 0x00000000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0x110, 0xc0000001);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0x114, 0xffffffff);
apb_write(OMC_APB_BASE_ADDR + 0x10c, 0x00000101);
apb_write(OMC_APB_BASE_ADDR + 0x11c, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0x500, 0x00000201);
apb_write(OMC_APB_BASE_ADDR + 0x504, 0x40000000);
while ((apb_read(OMC_APB_BASE_ADDR + 0x504) & 0x80000000) != 0x80000000) {
  wait_usleep(1);
}
apb_write(OMC_APB_BASE_ADDR + 0x504, 0x00000000);
apb_write(PHY_APB_BASE_ADDR + 0x50, 0x01004700);
apb_write(PHY_APB_BASE_ADDR + 0x50, 0x00004700);
// tINIT0 is controlled by System
// Drive CKE high
apb_write(OMC_APB_BASE_ADDR + 0x10, 0x0000001c);
apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
// Waits tINIT3 (200 us): Minimum idle time after first CKE assertion
wait_usleep(200);
// Issue RESET (MRW) command
apb_write(OMC_APB_BASE_ADDR + 0x4, 0x103f00fc);
apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
// Waits tINIT4 (1 us): Minimum idle time after RESET command
wait_usleep(1);
// Waits tINIT5 (9 us): Maximum duration of device auto initialization
// You can do polling on MR status to finish auto initialization part early
wait_usleep(9);
apb_write(OMC_APB_BASE_ADDR + 0x4, 0x100a00ff);
apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
// Waits tZQINIT (1 us)
wait_usleep(1);
// Raise up frequency of PLL for highest working condition
// Waits until frequency is stable
// Write down RCLK-related CRs
apb_write(OMC_APB_BASE_ADDR + 0x600, 0x002e0176);
apb_write(OMC_APB_BASE_ADDR + 0x604, 0x002e0176);
apb_write(OMC_APB_BASE_ADDR + 0x608, 0x001700bb);
apb_write(OMC_APB_BASE_ADDR + 0x60c, 0x000b005d);
apb_write(OMC_APB_BASE_ADDR + 0x610, 0x0005002e);
apb_write(OMC_APB_BASE_ADDR + 0x614, 0x00020017);
apb_write(OMC_APB_BASE_ADDR + 0x618, 0x00020017);
apb_write(OMC_APB_BASE_ADDR + 0x61c, 0x00020017);
apb_write(OMC_APB_BASE_ADDR + 0x678, 0x00000019);
#if 1
apb_write(OMC_APB_BASE_ADDR + 0x100, 0x000000e0);
apb_write(OMC_APB_BASE_ADDR + 0x620, 0x00000f11);
apb_write(OMC_APB_BASE_ADDR + 0x624, 0x060c0206);
apb_write(OMC_APB_BASE_ADDR + 0x628, 0x3022060f);
apb_write(OMC_APB_BASE_ADDR + 0x62c, 0x17110828);
apb_write(OMC_APB_BASE_ADDR + 0x630, 0x00000c06);
apb_write(OMC_APB_BASE_ADDR + 0x634, 0x00100000);
apb_write(OMC_APB_BASE_ADDR + 0x638, 0x000c0c04);
apb_write(OMC_APB_BASE_ADDR + 0x63c, 0x16100003);
apb_write(OMC_APB_BASE_ADDR + 0x640, 0x1070152a);
apb_write(OMC_APB_BASE_ADDR + 0x644, 0x00300068);
apb_write(OMC_APB_BASE_ADDR + 0x648, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0x64c, 0x0006170c);
apb_write(OMC_APB_BASE_ADDR + 0x650, 0x1033240a);
apb_write(OMC_APB_BASE_ADDR + 0x654, 0x00001310);
apb_write(OMC_APB_BASE_ADDR + 0x658, 0x00000060);
apb_write(OMC_APB_BASE_ADDR + 0x65c, 0x00480120);
apb_write(OMC_APB_BASE_ADDR + 0x660, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0x680, 0x18000418);
apb_write(OMC_APB_BASE_ADDR + 0x684, 0x05000502);
apb_write(OMC_APB_BASE_ADDR + 0x688, 0x120b0404);
apb_write(OMC_APB_BASE_ADDR + 0x68c, 0x20003520);
apb_write(OMC_APB_BASE_ADDR + 0x690, 0x00140000);
apb_write(OMC_APB_BASE_ADDR + 0x69c, 0x01240074);
apb_write(OMC_APB_BASE_ADDR + 0x6a0, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0x6a4, 0x20350c00);
apb_write(OMC_APB_BASE_ADDR + 0x6a8, 0x00040000);
#endif
apb_write(OMC_APB_BASE_ADDR + 0x4, 0x10010043);
apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
apb_write(OMC_APB_BASE_ADDR + 0x4, 0x1002001a);
apb_write(OMC_APB_BASE_ADDR + 0xc, 0x00000002);
#if 0
//	Prepare
apb_write(PHY_APB_BASE_ADDR + 0x64, 0x0000000e);
apb_write(PHY_APB_BASE_ADDR + 0x68, 0x0000000f);
apb_write(PHY_APB_BASE_ADDR + 0xb0, 0x10900070);
apb_read(PHY_APB_BASE_ADDR + 0xb4);
apb_write(PHY_APB_BASE_ADDR + 0xb0, 0x10902ff0);
apb_write(PHY_APB_BASE_ADDR + 0xb0, 0x10902fd0);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x04200040);
//	Gate leveling
apb_write(PHY_APB_BASE_ADDR + 0xc, 0x6b101000);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c200042);
apb_write(PHY_APB_BASE_ADDR + 0x64, 0x0000000e);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c200242);
while ((apb_read(PHY_APB_BASE_ADDR + 0x4) & 0x00020000) != 0x00020000) {
  wait_usleep(1);
}
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c200042);
apb_write(PHY_APB_BASE_ADDR + 0x10, 0x37ff5000);
apb_read(PHY_APB_BASE_ADDR + 0x108);
apb_read(PHY_APB_BASE_ADDR + 0xfc);
apb_read(PHY_APB_BASE_ADDR + 0x100);
apb_read(PHY_APB_BASE_ADDR + 0x104);
apb_write(PHY_APB_BASE_ADDR + 0x64, 0x0000000e);
apb_write(PHY_APB_BASE_ADDR + 0xc, 0x49001000);
//read dq calibration
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20004a);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20084a);
while ((apb_read(PHY_APB_BASE_ADDR + 0x4) & 0x00020000) != 0x00020000) {
  wait_usleep(1);
}
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20004a);
apb_read(PHY_APB_BASE_ADDR + 0xec);
apb_read(PHY_APB_BASE_ADDR + 0xf0);
apb_read(PHY_APB_BASE_ADDR + 0xf4);
apb_write(OMC_APB_BASE_ADDR + 0x10, 0x00000014);
apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
// write dq calibration
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20006a);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20206a);
while ((apb_read(PHY_APB_BASE_ADDR + 0x4) & 0x00020000) != 0x00020000) {
  wait_usleep(1);
}
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20006a);
apb_read(PHY_APB_BASE_ADDR + 0xec);
apb_read(PHY_APB_BASE_ADDR + 0xf0);
apb_read(PHY_APB_BASE_ADDR + 0xf4);
apb_read(PHY_APB_BASE_ADDR + 0xf8);
apb_write(OMC_APB_BASE_ADDR + 0x10, 0x00000017);
apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
//	training finalization
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c20002a);
apb_write(PHY_APB_BASE_ADDR + 0xb0, 0x10902ff0);
apb_write(PHY_APB_BASE_ADDR + 0x4, 0x0c60002a);
//	v6r3 update
apb_write(PHY_APB_BASE_ADDR + 0x50, 0x01004700);
apb_write(PHY_APB_BASE_ADDR + 0x50, 0x00004700);
//
#endif
apb_write(OMC_APB_BASE_ADDR + 0x410, 0x00101010);
apb_write(OMC_APB_BASE_ADDR + 0x420, 0x0c181006);
apb_write(OMC_APB_BASE_ADDR + 0x424, 0x20200820);
apb_write(OMC_APB_BASE_ADDR + 0x428, 0x80000020);
apb_write(OMC_APB_BASE_ADDR + 0x0, 0x00000000);
apb_write(OMC_APB_BASE_ADDR + 0x108, 0x00003000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0x704, 0x00000007);
apb_write(OMC_APB_BASE_ADDR + 0x330, 0x09311fff);
apb_write(OMC_APB_BASE_ADDR + 0x334, 0x00000002);
apb_write(OMC_APB_BASE_ADDR + 0x324, 0x00002000);
apb_write(OMC_APB_BASE_ADDR + 0x104, 0x80000000);
apb_write(OMC_APB_BASE_ADDR + 0x510, 0x00180000);
apb_write(OMC_APB_BASE_ADDR + 0x514, 0x00000000);
apb_write(OMC_SECURE_APB_BASE_ADDR + 0x700, 0x00000003);
apb_write(OMC_APB_BASE_ADDR + 0x20, 0x00000001);

//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x100, 0x000000f0);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x620, 0x00000505);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x624, 0x03040203);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x628, 0x0e0a0405);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x62c, 0x0c0c030c);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x630, 0x00000603);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x634, 0x000a0000);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x638, 0x000a0a04);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x63c, 0x0d000002);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x640, 0x10200805);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x644, 0x300e001e);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x648, 0x00000000);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x64c, 0x00040e06);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x650, 0x050f0a03);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x654, 0x00000605);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x658, 0x00000060);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x65c, 0x00150051);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x660, 0x00000000);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x680, 0x0c00040c);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x684, 0x02000202);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x688, 0x120b0404);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x68c, 0x20002f20);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x690, 0x00140000);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x69c, 0x01240074);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x6a0, 0x00000000);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x6a4, 0x202f0c00);
//apb_write(OMC_SECURE_APB_BASE_ADDR + 0x6a8, 0x00040000);
//apb_write(OMC_APB_BASE_ADDR + 0x310, 0x80000030);
//apb_write(OMC_APB_BASE_ADDR + 0x340, 0x80010083);
//apb_write(OMC_APB_BASE_ADDR + 0x344, 0x00000000);
//apb_write(OMC_APB_BASE_ADDR + 0x348, 0x80020004);
//apb_write(OMC_APB_BASE_ADDR + 0x34c, 0x00000000);
//apb_write(PHY_APB_BASE_ADDR + 0xb0, 0x10902ff0);
//apb_write(PHY_APB_BASE_ADDR + 0xb0, 0x1090a9f0);
//apb_write(PHY_APB_BASE_ADDR + 0xb0, 0x1010a9f0);
//// dvfs_level: 1, dvfs_mode: 2
//apb_write(PHY_APB_BASE_ADDR + 0xb8, 0x02000000);
//apb_write(PHY_APB_BASE_ADDR + 0xe0, 0x00000000);
//apb_write(PHY_APB_BASE_ADDR + 0xbc, 0x060c9122);
//apb_write(PHY_APB_BASE_ADDR + 0xc0, 0x23f00221);
//apb_write(PHY_APB_BASE_ADDR + 0x3d4, 0x00000000);
//apb_write(PHY_APB_BASE_ADDR + 0x3d8, 0x00000000);
//// Initialization done

Temp0 = apb_read(PHY_APB_BASE_ADDR + 0xb0);
apb_write(PHY_APB_BASE_ADDR + 0xb0, Temp0|0x00800000);
Temp1 = (apb_read(PHY_APB_BASE_ADDR + 0xb4))&0x1FF00000;
Temp0 = apb_read(PHY_APB_BASE_ADDR + 0xb0);
apb_write(PHY_APB_BASE_ADDR + 0xb0, ((Temp0&0xE00FFFFF)|Temp1));
Temp0 = apb_read(PHY_APB_BASE_ADDR + 0xb0);
apb_write(PHY_APB_BASE_ADDR + 0xb0, Temp0&0xFFFFFFFDF);


//SYS_OMC_DFS_SET = 0x7;
//while(!SYS_OMC_PDONE);

SYS_PLL2_PWDN	=	0;
SYS_PLL2_P		=	8;
SYS_PLL2_M		=	530;
SYS_PLL2_S		=	2;
SYS_PLL2_PWDN	=	1;

//SYS_PLL2_PWDN	=	0;
//SYS_PLL2_P		=	8;
//SYS_PLL2_M		=	700;
//SYS_PLL2_S		=	2;
//SYS_PLL2_PWDN	=	1;

//SYS_PLL2_PWDN	=	0;
//SYS_PLL2_P		=	8;
//SYS_PLL2_M		=	430;
//SYS_PLL2_S		=	1;
//SYS_PLL2_PWDN	=	1;



//SYS_OMC_DFS_SET = 0x1;

//while(SYS_OMC_DFS_SET==0x6 && SYS_OMC_PACCETP);
// SYS_OMC_DFS_SET = 0x1;



// Drive CKE high
apb_write(OMC_APB_BASE_ADDR + 0x10, 0x0000001c);
apb_write(OMC_APB_BASE_ADDR + 0x14, 0x00000001);
}

