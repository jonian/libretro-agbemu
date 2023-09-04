#ifndef GBA_H
#define GBA_H

#include "arm7tdmi.h"
#include "cartridge.h"
#include "types.h"

enum {
    BIOS_SIZE = 0x4000,   // 16kb
    EWRAM_SIZE = 0x40000, // 256kb
    IWRAM_SIZE = 0x8000,  // 32kb
    CRAM_SIZE = 0x400, // 1kb
    VRAM_SIZE = 0x18000, // 96kb
    OAM_SIZE = 0x400, // 1kb
};

enum { 
    R_BIOS,
    R_UNUSED,
    R_EWRAM,
    R_IWRAM,
    R_IO,
    R_CRAM,
    R_VRAM,
    R_OAM,
    R_ROM0,
    R_ROM0EX,
    R_ROM1,
    R_ROM1EX,
    R_ROM2,
    R_ROM2EX,
    R_SRAM
};

typedef struct _GBA {
    Arm7TDMI cpu;

    Cartridge* cart;

    union {
        byte b[BIOS_SIZE];
        hword h[BIOS_SIZE >> 1];
        word w[BIOS_SIZE >> 2];
    } bios;

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

    union {
        byte b[CRAM_SIZE];
        hword h[CRAM_SIZE >> 1];
        word w[CRAM_SIZE >> 2];
    } cram;

    union {
        byte b[VRAM_SIZE];
        hword h[VRAM_SIZE >> 1];
        word w[VRAM_SIZE >> 2];
    } vram;

    union {
        byte b[OAM_SIZE];
        hword h[OAM_SIZE >> 1];
        word w[OAM_SIZE >> 2];
    } oam;

} GBA;

void init_gba(GBA* gba, Cartridge* cart);

void gba_load_bios(GBA* gba, char* filename);

byte gba_readb(GBA* gba, word addr, int* cycles);
hword gba_readh(GBA* gba, word addr, int* cycles);
word gba_read(GBA* gba, word addr, int* cycles);
void gba_writeb(GBA* gba, word addr, byte b, int* cycles);
void gba_writeh(GBA* gba, word addr, hword h, int* cycles);
void gba_write(GBA* gba, word addr, word w, int* cycles);

void tick_gba(GBA* gba);

#endif