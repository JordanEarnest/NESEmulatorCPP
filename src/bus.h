#pragma once
#include <stdint.h>
#include "cpu.h"
#include "ppu.h"
#include "apu.h"
#include "cartridge.h"

class Bus {
    private:
        uint8_t ram[0x800] = {}; // 2048 bytes of ram

        Cartridge cartridge;

        CPU cpu;
        PPU ppu;
        APU apu;
    private:
        void doOAMDMA(uint8_t prefix);
    public:
        
    public:
        Bus();
        
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t val);

        void power();
        void reset();

        void clock();

        Cartridge& getCartridge();
};
