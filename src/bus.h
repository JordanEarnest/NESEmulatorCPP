#pragma once
#include <stdint.h>
#include "cpu.h"
#include "ppu.h"
#include "apu.h"

class Bus {
    private:
       uint8_t ram[0x800] = {}; // 2048 bytes of ram

       CPU cpu;
       PPU ppu;
       APU apu;

    public:
        Bus();
        
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t val);

        void power();
        void reset();

        uint32_t clock();
};
