#include "cartridge.h"

#include <stdio.h>
#include <stdlib.h>

Cartridge* create_cartridge(char* filename) {
    FILE* fp = fopen(filename, "rb");
    if (!fp) return NULL;

    Cartridge* cart = malloc(sizeof *cart);

    fseek(fp, 0, SEEK_END);
    int flen = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    cart->rom_size = (flen + 32) & ~0b11;
    cart->rom.b = malloc(cart->rom_size);
    fread(cart->rom.b, 1, flen, fp);
    fclose(fp);

    return cart;
}

void destroy_cartridge(Cartridge* cart) {
    free(cart->rom.b);
    free(cart);
}

byte cart_read_sram(Cartridge* cart, hword addr) {
    return 0;
}

void cart_write_sram(Cartridge* cart, hword addr, byte b) {

}