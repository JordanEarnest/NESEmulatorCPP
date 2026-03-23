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
        return ppu.readRegister((addr & 0x0007) + 0x2000);
    }
    // APU and I/O registers
    else if (addr <= 0x4017) {
        return apu.readRegister(addr);
    }
    // Disabled
    else if (addr <= 0x401F) {
        return 0xFF;
    }
    // Cartridge ROM/RAM
    else if (addr <= 0xFFFF) {
       return cartridge.read(addr); 
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
        ppu.writeRegister((addr & 0x0007) + 0x2000, val);
    }
    // DMA Transfer 
    else if (addr == 0x4014) {
        doOAMDMA(val); // possible val = 0x00, 0x02, and 0x3F
    }
    // APU and I/O registers
    else if (addr <= 0x4017) {
        apu.writeRegister(addr, val);
    }
    // Disabled
    else if (addr <= 0x401F) {
        return;
    }
    // Cartridge ROM/RAM
    else if (addr <= 0xFFFF) {
        cartridge.write(addr, val);
        return;
    }
}

void Bus::doOAMDMA(uint8_t prefix) {
    // Stall CPU, even cycle = 513, odd cycle = 514
    if (cpu.totalCycles % 2 == 0)
        cpu.stall(513);
    else
        cpu.stall(514);
    // If CPU writes 0x00 → DMA copies from $0000–00FF
    // If CPU writes 0x02 → DMA copies from $0200–02FF
    // If CPU writes 0x3F → DMA copies from $3F00–3FFF
    uint16_t highByte = prefix << 8;
    for (int i = 0; i < 256; i++) {
        ppu.oam[i] = read(highByte + i);
    }
}

void Bus::power() {
    cpu.power();
    ppu.power();
    apu.power();
}

void Bus::reset() {
    cpu.reset();
    ppu.reset();
    apu.reset();
}

void Bus::clock() {
    cpu.clock();

    // PPU runs 3x more than CPU
    ppu.clock();
    ppu.clock();
    ppu.clock();
    
    apu.clock();
}

Cartridge& Bus::getCartridge() {
    return cartridge;
}

