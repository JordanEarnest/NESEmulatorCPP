#include "bus.h"

CPU& Bus::getCPU() {
    return cpu;
}

uint8_t Bus::read(uint16_t addr) {
    // RAM (with mirroring)
    if (addr <= 0x1FFF) {
        return ram[addr & 0x07FF]
    }
}

void Bus::write(uint16_t addr, uint8_t val) {
    // RAM (with mirroring)
    if (addr <= 0x1FFF) {
        ram[addr & 0x07FF] = val;
    }
}


