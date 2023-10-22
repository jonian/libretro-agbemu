#ifndef GBA_H
#define GBA_H

#include "arm7tdmi.h"
#include "apu.h"
#include "cartridge.h"
#include "dma.h"
#include "io.h"
#include "ppu.h"
#include "scheduler.h"
#include "timer.h"
#include "types.h"

typedef enum { D_BYTE, D_HWORD, D_WORD } DataWidth;

enum {
    BIOS_SIZE = 0x4000,   // 16kb
    EWRAM_SIZE = 0x40000, // 256kb
    IWRAM_SIZE = 0x8000,  // 32kb
    PRAM_SIZE = 0x400,    // 1kb
    VRAM_SIZE = 0x18000,  // 96kb
    OAM_SIZE = 0x400,     // 1kb
};

enum {
    R_BIOS,
    R_UNUSED,
    R_EWRAM,
    R_IWRAM,
    R_IO,
    R_PRAM,
    R_VRAM,
    R_OAM,
    R_ROM0,
    R_ROM0EX,
    R_ROM1,
    R_ROM1EX,
    R_ROM2,
    R_ROM2EX,
    R_SRAM,
    R_SRAMEX
};

typedef struct _GBA {
    Arm7TDMI cpu;
    PPU ppu;
    APU apu;

    DMAController dmac;
    TimerController tmc;

    Scheduler sched;

    Cartridge* cart;
    word next_rom_addr;

    union {
        byte* b;
        hword* h;
        word* w;
    } bios;
    word last_bios_val;

    union {
        byte b[EWRAM_SIZE];
        hword h[EWRAM_SIZE >> 1];
        word w[EWRAM_SIZE >> 2];
    } ewram;

    union {
        byte b[IWRAM_SIZE];
        hword h[IWRAM_SIZE >> 1];
        word w[IWRAM_SIZE >> 2];
    } iwram;

    IO io;

    union {
        byte b[PRAM_SIZE];
        hword h[PRAM_SIZE >> 1];
        word w[PRAM_SIZE >> 2];
    } pram;

    union {
        byte b[VRAM_SIZE];
        hword h[VRAM_SIZE >> 1];
        word w[VRAM_SIZE >> 2];
    } vram;

    union {
        byte b[OAM_SIZE];
        hword h[OAM_SIZE >> 1];
        word w[OAM_SIZE >> 2];
        ObjAttr objs[128];
    } oam;

    bool halt;
    bool stop;

    bool openbus;

} GBA;

void init_gba(GBA* gba, Cartridge* cart, byte* bios);

byte* load_bios(char* filename);

int get_waitstates(GBA* gba, word addr, DataWidth d);

byte bus_readb(GBA* gba, word addr);
hword bus_readh(GBA* gba, word addr);
word bus_readw(GBA* gba, word addr);
void bus_writeb(GBA* gba, word addr, byte b);
void bus_writeh(GBA* gba, word addr, hword h);
void bus_writew(GBA* gba, word addr, word w);

void tick_components(GBA* gba, int cycles);

void gba_step(GBA* gba);

void update_keypad_irq(GBA* gba);

void log_error(GBA* gba, char* mess, word addr);

#endif