#pragma once
#include <stdint.h>
#include "cpu.h"

class Bus {
    private:
       uint8_t ram[0x800] = {}; // 2048 bytes of ram

       CPU cpu;
         
    public:
        CPU& getCPU();       
        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t val);
};
