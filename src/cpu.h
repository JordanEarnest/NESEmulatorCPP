#pragma once
#include <stdint.h>

class CPU {
    private:
        // Registers
        uint8_t A, X, Y;
        uint16_t PC;
        uint16_t S; 
        uint8_t P;
    private:
        int execute(uint8_t opcode);
    public:
};
