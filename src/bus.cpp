#include "bus.h"

Bus::Bus() {
    cpu.connect(this);
}

uint8_t Bus::read(uint16_t addr) {
    // 2KB RAM (with mirroring)
    if (addr <= 0x1FFF) {
        return ram[addr & 0x07FF];
    }
    // PPU registers (with mirroring)
    else if (addr <= 0x3FFF) {
        return ppu.readRegister(addr);
    }
    // APU registers
    else if (addr <= 0x4017) {
        return apu.readRegister(addr);
    }
    return 0xFF;
}

void Bus::write(uint16_t addr, uint8_t val) {
    // RAM (with mirroring)
    if (addr <= 0x1FFF) {
        ram[addr & 0x07FF] = val;
    }
    // PPU registers (with mirroring)
    else if (addr <= 0x3FFF) {
        ppu.writeRegister(addr, val);
    }
    // APU registers
    else if (addr <= 0x4017) {
        return apu.writeRegister(addr, val);
    }
}

void Bus::power() {
    cpu.power();
    ppu.power();
    apu.power();
}

void Bus::reset() {
    cpu.reset();
    ppu.power();
    apu.power();
}

uint32_t Bus::clock() {
    return cpu.clock();
}







