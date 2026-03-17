#include "cpu.h"
#include "bus.h"

uint8_t CPU::fetch(uint16_t addr) {
    return read(addr); 
}

uint32_t CPU::execute(uint8_t opcode) {
   return 4; 
}

void CPU::connect(Bus* bus) {
    this->bus = bus; 
}

uint8_t CPU::read(uint16_t addr) {
    return bus->read(addr);
}

void CPU::write(uint16_t addr, uint8_t val) {
    bus->write(addr, val);
}

void CPU::power() {
    A = 0x00;
    X = 0x00;
    Y = 0x00;
    PC = 0xFFFC;
    SP = 0xFD; 
    P = 0x04;
}

void CPU::reset() {
    // A, X, Y = unchanged
    PC = 0xFFFC;
    SP -= 0x003;
    // P = unchanged except bit 2 = 1
    P |= 0x04;
}

uint32_t CPU::clock() {
    uint8_t opcode = fetch(PC++);
    uint32_t cycles = execute(opcode);
    return cycles;
}


