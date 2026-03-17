#pragma once
#include <stdint.h>

class Bus;

class CPU {
    private:
        // Registers
        uint8_t A, X, Y;
        uint16_t PC;
        uint8_t SP; 
        uint8_t P;

        Bus* bus;
    private:
        uint8_t fetch(uint16_t addr);
        uint32_t execute(uint8_t opcode);
    public:
        void power();
        void reset();

        void connect(Bus* bus);

        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t val);

        uint32_t clock();
};
