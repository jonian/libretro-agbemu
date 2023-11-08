#include "debugger.h"

#include <stdio.h>
#include <string.h>

#include "emulator.h"
#include "gba.h"

const char* help = "Debugger commands:\n"
                   "c -- continue emulation\n"
                   "n -- next instruction\n"
                   "i -- cpu state info\n"
                   "rb <addr> -- read byte\n"
                   "rh <addr> -- read halfword \n"
                   "rw <addr> -- read word\n"
                   "r -- reset\n"
                   "q -- quit debugger\n"
                   "h -- help";

int read_num(char* str, word* res) {
    if (sscanf(str, "0x%x", res) < 1) {
        if (sscanf(str, "%d", res) < 1) return -1;
    }
    return 0;
}

void debugger_run() {
    char buf[100];

    printf("agbemu Debugger\n");
    print_cpu_state(&agbemu.gba->cpu);
    print_cur_instr(&agbemu.gba->cpu);

    while (true) {
        printf("> ");
        fgets(buf, 100, stdin);

        char* com = strtok(buf, " \t\n");
        if (!(com && *com)) {
            continue;
        }

        switch (com[0]) {
            case 'q':
                agbemu.debugger = false;
                return;
            case 'c':
                agbemu.running = true;
                return;
            case 'h':
                printf("%s",help);
                break;
            case 'n':
                gba_step(agbemu.gba);
                print_cur_instr(&agbemu.gba->cpu);
                break;
            case 'i':
                print_cpu_state(&agbemu.gba->cpu);
                break;
            case 'r':
                switch (com[1]) {
                    case 'b': {
                        word addr;
                        if (read_num(strtok(NULL, " \t\n"), &addr) < 0) {
                            printf("Invalid address\n");
                        } else {
                            printf("[%08x] = %02x\n", addr, bus_readb(agbemu.gba, addr));
                        }
                        break;
                    }
                    case 'h': {
                        word addr;
                        if (read_num(strtok(NULL, " \t\n"), &addr) < 0) {
                            printf("Invalid address\n");
                        } else {
                            printf("[%08x] = %04x\n", addr, bus_readh(agbemu.gba, addr));
                        }
                        break;
                    }
                    case 'w': {
                        word addr;
                        if (read_num(strtok(NULL, " \t\n"), &addr) < 0) {
                            printf("Invalid address\n");
                        } else {
                            printf("[%08x] = %08x\n", addr, bus_readw(agbemu.gba, addr));
                        }
                        break;
                    }
                    default:
                        printf("Reset emulation? ");
                        char ans[5];
                        scanf("%4s\n", ans);
                        if (ans[0] == 'y') {
                            init_gba(agbemu.gba, agbemu.cart, agbemu.bios, agbemu.bootbios);
                            return;
                        }
                        break;
                }
                break;
            default:
                printf("Invalid command\n");
        }
    }
}