#include "timer.h"

#include <stdio.h>

#include "gba.h"
#include "io.h"

const int RATES[4] = {0, 6, 8, 10};

void update_timer_count(TimerController* tmc, int i) {
    if (!tmc->master->io.tm[i].cnt.enable || tmc->master->io.tm[i].cnt.countup) {
        tmc->set_time[i] = tmc->master->sched.now;
        return;
    }

    int rate = RATES[tmc->master->io.tm[i].cnt.rate];
    tmc->counter[i] += (tmc->master->sched.now >> rate) - (tmc->set_time[i] >> rate);
    tmc->set_time[i] = tmc->master->sched.now;
}

void update_timer_reload(TimerController* tmc, int i) {
    remove_event(&tmc->master->sched, i);

    if (!tmc->master->io.tm[i].cnt.enable || tmc->master->io.tm[i].cnt.countup) return;

    int rate = RATES[tmc->master->io.tm[i].cnt.rate];
    dword rel_time =
        (tmc->set_time[i] + ((0x10000 - tmc->counter[i]) << rate)) & ~((1 << rate) - 1);
    add_event(&tmc->master->sched, &(Event){rel_time, i});
}

void enable_timer(TimerController* tmc, int i) {
    tmc->counter[i] = tmc->master->io.tm[i].reload;
    tmc->set_time[i] = tmc->master->sched.now;
    update_timer_reload(tmc, i);
}

void reload_timer(TimerController* tmc, int i) {
    tmc->counter[i] = tmc->master->io.tm[i].reload;
    tmc->set_time[i] = tmc->master->sched.now;
    update_timer_reload(tmc, i);

    if (tmc->master->io.tm[i].cnt.irq) tmc->master->io.ifl.timer |= 1 << i;

    if (i + 1 < 4 && tmc->master->io.tm[i + 1].cnt.enable &&
        tmc->master->io.tm[i + 1].cnt.countup) {
        tmc->counter[i + 1]++;
        if (tmc->counter[i + 1] == 0) reload_timer(tmc, i + 1);
    }

    if(tmc->master->io.soundcnth.cha_timer == i) {
        fifo_a_pop(&tmc->master->apu);
        if(tmc->master->apu.fifo_a_size <= 16 && tmc->master->io.dma[1].cnt.start == DMA_ST_SPEC) {
            tmc->master->dmac.dma[1].sound = true;
            dma_activate(&tmc->master->dmac, 1);
        }
    }
    if (tmc->master->io.soundcnth.chb_timer == i) {
        fifo_b_pop(&tmc->master->apu);
        if (tmc->master->apu.fifo_b_size <= 16 && tmc->master->io.dma[2].cnt.start == DMA_ST_SPEC) {
            tmc->master->dmac.dma[2].sound = true;
            dma_activate(&tmc->master->dmac, 2);
        }
    }
}