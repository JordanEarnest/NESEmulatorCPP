#include "cpu.h"
#include "bus.h"

CPU::CPU() {
    totalCycles = 0;
    remainingStallCycles = 0;
    remainingCycles = 0;
}

uint8_t CPU::fetch(uint16_t addr) {
    return read(addr); 
}


uint8_t CPU::execute(uint8_t opcode) {
    // OFFICIAL OPCODES
    switch (opcode) {
        // ADC - Add with Carry
        case 0x69: {
            uint8_t n = read(PC++);
            ADC(n);
            return 2;
        }
        case 0x65: {
            uint8_t addr = read(PC++);
            uint8_t value = read(addr);
            ADC(value);
            return 3;
        }
        case 0x75: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + X) & 0xFF; // wrap in zero page

            uint8_t value = read(addr);
            ADC(value);

            return 4;
        }
        case 0x6D: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t value = read(addr);
            ADC(value);
            return 4;
        }
        case 0x7D: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + X;

            uint8_t value = read(addr);
            ADC(value);

            return 4 + (pageCrossed(base, addr) ? 1 : 0);
        }
        case 0x79: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + Y;

            uint8_t value = read(addr);
            ADC(value);

            return 4 + (pageCrossed(base, addr) ? 1 : 0);
        }
        case 0x61: {
            uint8_t zp = read(PC++);
            uint8_t ptr = (zp + X) & 0xFF;

            uint16_t addr = read(ptr) | (read((ptr + 1) & 0xFF) << 8);

            uint8_t value = read(addr);
            ADC(value);

            return 6;
        }
        case 0x71: {
            uint8_t zp = read(PC++);

            uint16_t base = read(zp) | (read((zp + 1) & 0xFF) << 8);
            uint16_t addr = base + Y;

            uint8_t value = read(addr);
            ADC(value);

            return 5 + (pageCrossed(base, addr) ? 1 : 0);
        }
        // AND - Bitwise AND
        case 0x29: {
            uint8_t n = read(PC++);
            AND(n);
            return 2;
        }
        case 0x25: {
            uint8_t addr = read(PC++);
            uint8_t value = read(addr);
            AND(value);
            return 3;
        }
        case 0x35: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + X) & 0xFF;

            uint8_t value = read(addr);
            AND(value);

            return 4;
        }
        case 0x2D: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t value = read(addr);
            AND(value);

            return 4;
        }
        case 0x3D: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + X;

            uint8_t value = read(addr);
            AND(value);

            return 4 + (pageCrossed(base, addr) ? 1 : 0);
        }
        case 0x39: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + Y;

            uint8_t value = read(addr);
            AND(value);

            return 4 + (pageCrossed(base, addr) ? 1 : 0);
        }
        case 0x21: {
            uint8_t zp = read(PC++);
            uint8_t ptr = (zp + X) & 0xFF;

            uint16_t addr = read(ptr) | (read((ptr + 1) & 0xFF) << 8);

            uint8_t value = read(addr);
            AND(value);

            return 6;
        }
        case 0x31: {
            uint8_t zp = read(PC++);

            uint16_t base = read(zp) | (read((zp + 1) & 0xFF) << 8);
            uint16_t addr = base + Y;

            uint8_t value = read(addr);
            AND(value);

            return 5 + (pageCrossed(base, addr) ? 1 : 0);
        }
        // ASL_A - Arithmetic Shift Left with Accumulator
        case 0x0A: {
            ASL_A();
            return 2;
        }
        // ASL - Arithmetic Shift Left
        case 0x06: {
            uint8_t addr = read(PC++);
            uint8_t val = read(addr);

            val = ASL(val);
            write(addr, val);

            return 5;
        }
        case 0x16: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + X) & 0xFF;

            uint8_t val = read(addr);
            val = ASL(val);
            write(addr, val);

            return 6;
        }
        case 0x0E: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t val = read(addr);
            val = ASL(val);
            write(addr, val);

            return 6;
        }
        case 0x1E: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + X;

            uint8_t val = read(addr);
            val = ASL(val);
            write(addr, val);

            return 7;
        }
        // BCC - Branch if Carry Clear
        case 0x90: {
            
            return 2 + BCC();
        }
        // BCS - Branch if Carry Set
        case 0xB0: {
            return 2 + BCS();
        }
        // BEQ - Branch if equal
        case 0xF0: {
            return 2 + BEQ();
        }
        // BIT - Bit Test
        case 0x24: {
            uint8_t addr = read(PC++);
            uint8_t value = read(addr);

            BIT(value);

            return 3;
        }
        case 0x2C: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t value = read(addr);

            BIT(value);

            return 4;
        }
        // BMI - Branch if minus
        case 0x30: {
            return 2 + BMI();
        }
        // BNE
        case 0xD0: { 
            return 2 + BNE();
        }
        // BPL
        case 0x10: { 
            return 2 + BPL();
        }
        // BVC
        case 0x50: { 
            return 2 + BVC();
        }
        // BVS
        case 0x70: { 
            return 2 + BVS();
        }
        // CLC
        case 0x18: { 
            CLC();
            return 2;
        }
        // BRK
        case 0x00: { 
            return 7 + BRK();
        }
        // CLD
        case 0xD8: {
            CLD();
            return 2;
        }

        // CLI
        case 0x58: {
            CLI();
            return 2;
        }

        // CLV
        case 0xB8: {
            CLV();
            return 2;
        }

        // CMP Immediate
        case 0xC9: {
            uint8_t val = read(PC++);
            CMP(val);
            return 2;
        }

        // CMP Zero Page
        case 0xC5: {
            uint8_t addr = read(PC++);
            uint8_t val = read(addr);
            CMP(val);
            return 3;
        }

        // CMP Zero Page,X
        case 0xD5: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + X) & 0xFF;

            uint8_t val = read(addr);
            CMP(val);

            return 4;
        }

        // CMP Absolute
        case 0xCD: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t val = read(addr);
            CMP(val);

            return 4;
        }

        // CMP Absolute,X
        case 0xDD: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + X;

            uint8_t val = read(addr);
            CMP(val);

            return 4 + (pageCrossed(base, addr) ? 1 : 0);
        }

        // CMP Absolute,Y
        case 0xD9: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + Y;

            uint8_t val = read(addr);
            CMP(val);

            return 4 + (pageCrossed(base, addr) ? 1 : 0);
        }

        // CMP (Indirect,X)
        case 0xC1: {
            uint8_t zp = read(PC++);
            uint8_t ptr = (zp + X) & 0xFF;

            uint16_t addr = read(ptr) | (read((ptr + 1) & 0xFF) << 8);

            uint8_t val = read(addr);
            CMP(val);

            return 6;
        }

        // CMP (Indirect),Y
        case 0xD1: {
            uint8_t zp = read(PC++);

            uint16_t base = read(zp) | (read((zp + 1) & 0xFF) << 8);
            uint16_t addr = base + Y;

            uint8_t val = read(addr);
            CMP(val);

            return 5 + (pageCrossed(base, addr) ? 1 : 0);
        }

        // CPX Immediate
        case 0xE0: {
            uint8_t val = read(PC++);
            CPX(val);
            return 2;
        }

        // CPX Zero Page
        case 0xE4: {
            uint8_t addr = read(PC++);
            uint8_t val = read(addr);
            CPX(val);
            return 3;
        }

        // CPX Absolute
        case 0xEC: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t val = read(addr);
            CPX(val);

            return 4;
        }

        // CPY Immediate
        case 0xC0: {
            uint8_t val = read(PC++);
            CPY(val);
            return 2;
        }

        // CPY Zero Page
        case 0xC4: {
            uint8_t addr = read(PC++);
            uint8_t val = read(addr);
            CPY(val);
            return 3;
        }

        // CPY Absolute
        case 0xCC: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t val = read(addr);
            CPY(val);

            return 4;
        }

        // DEC Zero Page
        case 0xC6: {
            uint8_t addr = read(PC++);
            uint8_t val = read(addr);

            val = DEC(val);
            write(addr, val);

            return 5;
        }

        // DEC Zero Page,X
        case 0xD6: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + X) & 0xFF;

            uint8_t val = read(addr);
            val = DEC(val);
            write(addr, val);

            return 6;
        }

        // DEC Absolute
        case 0xCE: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t val = read(addr);
            val = DEC(val);
            write(addr, val);

            return 6;
        }

        // DEC Absolute,X
        case 0xDE: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + X;

            uint8_t val = read(addr);
            val = DEC(val);
            write(addr, val);

            return 7;
        }
        // DEX
        case 0xCA: {
            DEX();
            return 2;
        }

        // DEY
        case 0x88: {
            DEY();
            return 2;
        }

        // EOR Immediate
        case 0x49: {
            uint8_t val = read(PC++);
            EOR(val);
            return 2;
        }

        // EOR Zero Page
        case 0x45: {
            uint8_t addr = read(PC++);
            uint8_t val = read(addr);
            EOR(val);
            return 3;
        }

        // EOR Zero Page,X
        case 0x55: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + X) & 0xFF;

            uint8_t val = read(addr);
            EOR(val);

            return 4;
        }

        // EOR Absolute
        case 0x4D: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t val = read(addr);
            EOR(val);

            return 4;
        }

        // EOR Absolute,X
        case 0x5D: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + X;

            uint8_t val = read(addr);
            EOR(val);

            return 4 + (pageCrossed(base, addr) ? 1 : 0);
        }

        // EOR Absolute,Y
        case 0x59: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + Y;

            uint8_t val = read(addr);
            EOR(val);

            return 4 + (pageCrossed(base, addr) ? 1 : 0);
        }

        // EOR (Indirect,X)
        case 0x41: {
            uint8_t zp = read(PC++);
            uint8_t ptr = (zp + X) & 0xFF;

            uint16_t addr = read(ptr) | (read((ptr + 1) & 0xFF) << 8);

            uint8_t val = read(addr);
            EOR(val);

            return 6;
        }

        // EOR (Indirect),Y
        case 0x51: {
            uint8_t zp = read(PC++);

            uint16_t base = read(zp) | (read((zp + 1) & 0xFF) << 8);
            uint16_t addr = base + Y;

            uint8_t val = read(addr);
            EOR(val);

            return 5 + (pageCrossed(base, addr) ? 1 : 0);
        }

        // INC Zero Page
        case 0xE6: {
            uint8_t addr = read(PC++);
            uint8_t val = read(addr);

            val = INC(val);
            write(addr, val);

            return 5;
        }

        // INC Zero Page,X
        case 0xF6: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + X) & 0xFF;

            uint8_t val = read(addr);
            val = INC(val);
            write(addr, val);

            return 6;
        }

        // INC Absolute
        case 0xEE: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t val = read(addr);
            val = INC(val);
            write(addr, val);

            return 6;
        }

        // INC Absolute,X
        case 0xFE: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + X;

            uint8_t val = read(addr);
            val = INC(val);
            write(addr, val);

            return 7;
        }

        // INX
        case 0xE8: {
            INX();
            return 2;
        }

        // INY
        case 0xC8: {
            INY();
            return 2;
        }

        // JMP Absolute
        case 0x4C: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            JMP(addr);
            return 3;
        }

        // JMP Indirect (6502 bug emulated)
        case 0x6C: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t ptr = (hi << 8) | lo;

            uint16_t addr;
            if ((ptr & 0x00FF) == 0x00FF) {
                // hardware bug: page wrap
                addr = read(ptr) | (read(ptr & 0xFF00) << 8);
            } else {
                addr = read(ptr) | (read(ptr + 1) << 8);
            }

            JMP(addr);
            return 5;
        }
        // JSR Absolute
        case 0x20: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            return 6 + JSR(addr);
        }

        // LDA Immediate
        case 0xA9: {
            uint8_t val = read(PC++);
            LDA(val);
            return 2;
        }

        // LDA Zero Page
        case 0xA5: {
            uint8_t addr = read(PC++);
            uint8_t val = read(addr);
            LDA(val);
            return 3;
        }

        // LDA Zero Page,X
        case 0xB5: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + X) & 0xFF;

            uint8_t val = read(addr);
            LDA(val);

            return 4;
        }

        // LDA Absolute
        case 0xAD: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t val = read(addr);
            LDA(val);

            return 4;
        }

        // LDA Absolute,X
        case 0xBD: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + X;

            uint8_t val = read(addr);
            LDA(val);

            return 4 + (pageCrossed(base, addr) ? 1 : 0);
        }

        // LDA Absolute,Y
        case 0xB9: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + Y;

            uint8_t val = read(addr);
            LDA(val);

            return 4 + (pageCrossed(base, addr) ? 1 : 0);
        }

        // LDA (Indirect,X)
        case 0xA1: {
            uint8_t zp = read(PC++);
            uint8_t ptr = (zp + X) & 0xFF;

            uint16_t addr = read(ptr) | (read((ptr + 1) & 0xFF) << 8);

            uint8_t val = read(addr);
            LDA(val);

            return 6;
        }

        // LDA (Indirect),Y
        case 0xB1: {
            uint8_t zp = read(PC++);

            uint16_t base = read(zp) | (read((zp + 1) & 0xFF) << 8);
            uint16_t addr = base + Y;

            uint8_t val = read(addr);
            LDA(val);

            return 5 + (pageCrossed(base, addr) ? 1 : 0);
        }

        // LDX Immediate
        case 0xA2: {
            uint8_t val = read(PC++);
            LDX(val);
            return 2;
        }

        // LDX Zero Page
        case 0xA6: {
            uint8_t addr = read(PC++);
            uint8_t val = read(addr);
            LDX(val);
            return 3;
        }

        // LDX Zero Page,Y
        case 0xB6: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + Y) & 0xFF;

            uint8_t val = read(addr);
            LDX(val);

            return 4;
        }

        // LDX Absolute
        case 0xAE: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t val = read(addr);
            LDX(val);

            return 4;
        }

        // LDX Absolute,Y
        case 0xBE: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + Y;

            uint8_t val = read(addr);
            LDX(val);

            return 4 + (pageCrossed(base, addr) ? 1 : 0);
        }

        // LDY Immediate
        case 0xA0: {
            uint8_t val = read(PC++);
            LDY(val);
            return 2;
        }

        // LDY Zero Page
        case 0xA4: {
            uint8_t addr = read(PC++);
            uint8_t val = read(addr);
            LDY(val);
            return 3;
        }

        // LDY Zero Page,X
        case 0xB4: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + X) & 0xFF;

            uint8_t val = read(addr);
            LDY(val);

            return 4;
        }

        // LDY Absolute
        case 0xAC: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t val = read(addr);
            LDY(val);

            return 4;
        }

        // LDY Absolute,X
        case 0xBC: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + X;

            uint8_t val = read(addr);
            LDY(val);

            return 4 + (pageCrossed(base, addr) ? 1 : 0);
        }

        // LSR Accumulator
        case 0x4A: {
            LSR_A();
            return 2;
        }

        // LSR Zero Page
        case 0x46: {
            uint8_t addr = read(PC++);
            uint8_t val = read(addr);

            val = LSR(val);
            write(addr, val);

            return 5;
        }

        // LSR Zero Page,X
        case 0x56: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + X) & 0xFF;

            uint8_t val = read(addr);
            val = LSR(val);
            write(addr, val);

            return 6;
        }

        // LSR Absolute
        case 0x4E: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t val = read(addr);
            val = LSR(val);
            write(addr, val);

            return 6;
        }

        // LSR Absolute,X
        case 0x5E: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + X;

            uint8_t val = read(addr);
            val = LSR(val);
            write(addr, val);

            return 7;
        }

        // NOP
        case 0xEA: {
            NOP();
            return 2;
        }

        // ORA Immediate
        case 0x09: {
            uint8_t val = read(PC++);
            ORA(val);
            return 2;
        }

        // ORA Zero Page
        case 0x05: {
            uint8_t addr = read(PC++);
            uint8_t val = read(addr);
            ORA(val);
            return 3;
        }

        // ORA Zero Page,X
        case 0x15: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + X) & 0xFF;

            uint8_t val = read(addr);
            ORA(val);

            return 4;
        }

        // ORA Absolute
        case 0x0D: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t val = read(addr);
            ORA(val);

            return 4;
        }

        // ORA Absolute,X
        case 0x1D: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + X;

            uint8_t val = read(addr);
            ORA(val);

            return 4 + (pageCrossed(base, addr) ? 1 : 0);
        }

        // ORA Absolute,Y
        case 0x19: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + Y;

            uint8_t val = read(addr);
            ORA(val);

            return 4 + (pageCrossed(base, addr) ? 1 : 0);
        }

        // ORA (Indirect,X)
        case 0x01: {
            uint8_t zp = read(PC++);
            uint8_t ptr = (zp + X) & 0xFF;

            uint16_t addr = read(ptr) | (read((ptr + 1) & 0xFF) << 8);

            uint8_t val = read(addr);
            ORA(val);

            return 6;
        }

        // ORA (Indirect),Y
        case 0x11: {
            uint8_t zp = read(PC++);

            uint16_t base = read(zp) | (read((zp + 1) & 0xFF) << 8);
            uint16_t addr = base + Y;

            uint8_t val = read(addr);
            ORA(val);

            return 5 + (pageCrossed(base, addr) ? 1 : 0);
        }
        // PHA
        case 0x48: {
            PHA();
            return 3;
        }

        // PHP
        case 0x08: {
            PHP();
            return 3;
        }

        // PLA
        case 0x68: {
            PLA();
            return 4;
        }

        // PLP
        case 0x28: {
            PLP();
            return 4;
        }

        // ROL Accumulator
        case 0x2A: {
            ROL_A();
            return 2;
        }

        // ROL Zero Page
        case 0x26: {
            uint8_t addr = read(PC++);
            uint8_t val = read(addr);

            val = ROL(val);
            write(addr, val);

            return 5;
        }

        // ROL Zero Page,X
        case 0x36: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + X) & 0xFF;

            uint8_t val = read(addr);
            val = ROL(val);
            write(addr, val);

            return 6;
        }

        // ROL Absolute
        case 0x2E: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t val = read(addr);
            val = ROL(val);
            write(addr, val);

            return 6;
        }

        // ROL Absolute,X
        case 0x3E: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + X;

            uint8_t val = read(addr);
            val = ROL(val);
            write(addr, val);

            return 7;
        }

        // ROR Accumulator
        case 0x6A: {
            ROR_A();
            return 2;
        }

        // ROR Zero Page
        case 0x66: {
            uint8_t addr = read(PC++);
            uint8_t val = read(addr);

            val = ROR(val);
            write(addr, val);

            return 5;
        }

        // ROR Zero Page,X
        case 0x76: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + X) & 0xFF;

            uint8_t val = read(addr);
            val = ROR(val);
            write(addr, val);

            return 6;
        }

        // ROR Absolute
        case 0x6E: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t val = read(addr);
            val = ROR(val);
            write(addr, val);

            return 6;
        }

        // ROR Absolute,X
        case 0x7E: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + X;

            uint8_t val = read(addr);
            val = ROR(val);
            write(addr, val);

            return 7;
        }
        // RTI
        case 0x40: {
            return 6 + RTI();
        }
        // RTS
        case 0x60: {
            return 6 + RTS();
        }

        // SBC Immediate
        case 0xE9: {
            uint8_t val = read(PC++);
            SBC(val);
            return 2;
        }

        // SBC Zero Page
        case 0xE5: {
            uint8_t addr = read(PC++);
            uint8_t val = read(addr);
            SBC(val);
            return 3;
        }

        // SBC Zero Page,X
        case 0xF5: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + X) & 0xFF;

            uint8_t val = read(addr);
            SBC(val);

            return 4;
        }

        // SBC Absolute
        case 0xED: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            uint8_t val = read(addr);
            SBC(val);

            return 4;
        }

        // SBC Absolute,X
        case 0xFD: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + X;

            uint8_t val = read(addr);
            SBC(val);

            return 4 + (pageCrossed(base, addr) ? 1 : 0);
        }

        // SBC Absolute,Y
        case 0xF9: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t base = (hi << 8) | lo;

            uint16_t addr = base + Y;

            uint8_t val = read(addr);
            SBC(val);

            return 4 + (pageCrossed(base, addr) ? 1 : 0);
        }

        // SBC (Indirect,X)
        case 0xE1: {
            uint8_t zp = read(PC++);
            uint8_t ptr = (zp + X) & 0xFF;

            uint16_t addr = read(ptr) | (read((ptr + 1) & 0xFF) << 8);

            uint8_t val = read(addr);
            SBC(val);

            return 6;
        }

        // SBC (Indirect),Y
        case 0xF1: {
            uint8_t zp = read(PC++);

            uint16_t base = read(zp) | (read((zp + 1) & 0xFF) << 8);
            uint16_t addr = base + Y;

            uint8_t val = read(addr);
            SBC(val);

            return 5 + (pageCrossed(base, addr) ? 1 : 0);
        }

        // SEC
        case 0x38: {
            SEC();
            return 2;
        }

        // SED
        case 0xF8: {
            SED();
            return 2;
        }

        // SEI
        case 0x78: {
            SEI();
            return 2;
        }

        // STA Zero Page
        case 0x85: {
            uint8_t addr = read(PC++);
            STA(addr);
            return 3;
        }

        // STA Zero Page,X
        case 0x95: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + X) & 0xFF;

            STA(addr);
            return 4;
        }

        // STA Absolute
        case 0x8D: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            STA(addr);
            return 4;
        }

        // STA Absolute,X
        case 0x9D: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = ((hi << 8) | lo) + X;

            STA(addr);
            return 5;
        }

        // STA Absolute,Y
        case 0x99: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = ((hi << 8) | lo) + Y;

            STA(addr);
            return 5;
        }

        // STA (Indirect,X)
        case 0x81: {
            uint8_t zp = read(PC++);
            uint8_t ptr = (zp + X) & 0xFF;

            uint16_t addr = read(ptr) | (read((ptr + 1) & 0xFF) << 8);

            STA(addr);
            return 6;
        }

        // STA (Indirect),Y
        case 0x91: {
            uint8_t zp = read(PC++);

            uint16_t base = read(zp) | (read((zp + 1) & 0xFF) << 8);
            uint16_t addr = base + Y;

            STA(addr);
            return 6;
        }

        // STX Zero Page
        case 0x86: {
            uint8_t addr = read(PC++);
            STX(addr);
            return 3;
        }

        // STX Zero Page,Y
        case 0x96: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + Y) & 0xFF;

            STX(addr);
            return 4;
        }

        // STX Absolute
        case 0x8E: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            STX(addr);
            return 4;
        }
        // STY Zero Page
        case 0x84: {
            uint8_t addr = read(PC++);
            STY(addr);
            return 3;
        }

        // STY Zero Page,X
        case 0x94: {
            uint8_t zp = read(PC++);
            uint8_t addr = (zp + X) & 0xFF;

            STY(addr);
            return 4;
        }

        // STY Absolute
        case 0x8C: {
            uint16_t lo = read(PC++);
            uint16_t hi = read(PC++);
            uint16_t addr = (hi << 8) | lo;

            STY(addr);
            return 4;
        }

        // TAX
        case 0xAA: {
            TAX();
            return 2;
        }

        // TAY
        case 0xA8: {
            TAY();
            return 2;
        }

        // TSX
        case 0xBA: {
            TSX();
            return 2;
        }

        // TXA
        case 0x8A: {
            TXA();
            return 2;
        }

        // TXS
        case 0x9A: {
            TXS();
            return 2;
        }

        // TYA
        case 0x98: {
            TYA();
            return 2;
        }
        default:
            return 2;
   }
}

bool CPU::pageCrossed(uint16_t base, uint16_t addr) {
    return (base & 0xFF00) != (addr & 0xFF00);
}

// Instruction Set
void CPU::ADC(uint8_t val) {
    uint16_t sum = A + val + getPCarry();

    uint8_t result = sum & 0xFF;

    setPCarry(sum > 0xFF);
    setPZero(result == 0);
    setPNegative(result & 0x80);
    setPOverflow(((result ^ A) & (result ^ val) & 0x80) != 0);

    A = result;
}

void CPU::AND(uint8_t val) {
    uint8_t result = A & val;

    setPZero(result == 0);
    setPNegative(result & 0x80);

    A = result;
}

uint8_t CPU::ASL(uint8_t val) {
    setPCarry(val & 0x80); // old bit 7

    uint8_t result = val << 1;

    setPZero(result == 0);
    setPNegative(result & 0x80);

    return result;
}

void CPU::ASL_A() {
    setPCarry(A & 0x80);

    A <<= 1;

    setPZero(A == 0);
    setPNegative(A & 0x80);
}

uint8_t CPU::BCC() {
    int8_t offset = (int8_t)read(PC++);

    if (!getPCarry()) {
        uint16_t oldPC = PC;
        PC += offset;

        // base extra cycle for branch taken
        uint8_t cycles = 1;

        // additional cycle if page crossed
        if (pageCrossed(oldPC, PC))
            cycles++;

        return cycles;
    }

    return 0;
}

uint8_t CPU::BCS() {
    int8_t offset = (int8_t)read(PC++);

    if (getPCarry()) {
        uint16_t oldPC = PC;
        PC += offset;

        uint8_t cycles = 1;

        if (pageCrossed(oldPC, PC))
            cycles++;

        return cycles;
    }
    return 0;
}

uint8_t CPU::BEQ() {
    int8_t offset = (int8_t)read(PC++);

    if (getPZero()) {
        uint16_t oldPC = PC;
        PC += offset;

        uint8_t cycles = 1;

        if (pageCrossed(oldPC, PC))
            cycles++;

        return cycles;
    }

    return 0;
}

void CPU::BIT(uint8_t val) {
    setPZero((A & val) == 0);
    setPNegative(val & 0x80);
    setPOverflow(val & 0x40);
}

uint8_t CPU::BMI() {
    int8_t offset = (int8_t)read(PC++);

    if (getPNegative()) {
        uint16_t oldPC = PC;
        PC += offset;

        uint8_t cycles = 1;

        if (pageCrossed(oldPC, PC))
            cycles++;

        return cycles;
    }
    return 0;
}

uint8_t CPU::BNE() {
    int8_t offset = (int8_t)read(PC++);

    if (!getPZero()) {
        uint16_t oldPC = PC;
        PC += offset;

        uint8_t cycles = 1;

        if (pageCrossed(oldPC, PC))
            cycles++;

        return cycles;
    }

    return 0;
}

uint8_t CPU::BPL() {
    int8_t offset = (int8_t)read(PC++);

    if (!getPNegative()) {
        uint16_t oldPC = PC;
        PC += offset;

        uint8_t cycles = 1;

        if (pageCrossed(oldPC, PC))
            cycles++;

        return cycles;
    }

    return 0;
}

uint8_t CPU::BVC() {
    int8_t offset = (int8_t)read(PC++);

    if (!getPOverflow()) {
        uint16_t oldPC = PC;
        PC += offset;

        uint8_t cycles = 1;

        if (pageCrossed(oldPC, PC))
            cycles++;

        return cycles;
    }

    return 0;
}

uint8_t CPU::BVS() {
    int8_t offset = (int8_t)read(PC++);

    if (getPOverflow()) {
        uint16_t oldPC = PC;
        PC += offset;

        uint8_t cycles = 1;

        if (pageCrossed(oldPC, PC))
            cycles++;

        return cycles;
    }

    return 0;
}

void CPU::CLC() {
    setPCarry(0);
}

uint8_t CPU::BRK() {
    PC++; // skip next byte (padding)

    // push PC high then low
    write(getSPAddr(), (PC >> 8) & 0xFF);
    SP--;
    write(getSPAddr(), PC & 0xFF);
    SP--;

    // set break flag before pushing
    uint8_t p = (P | 0x10) | 0x20; // set B only in pushed value, enforce bit 5 = 1
    write(getSPAddr(), p);
    SP--;

    // set interrupt disable
    setPInterruptDisable(1);

    // load interrupt vector
    uint16_t lo = read(0xFFFE);
    uint16_t hi = read(0xFFFF);
    PC = (hi << 8) | lo;

    return 0; // no extra cycles
}

void CPU::CLD() {
    setPDecimal(0);
}

void CPU::CLI() {
    setPInterruptDisable(0);
}

void CPU::CLV() {
    setPOverflow(0);
}

void CPU::CMP(uint8_t val) {
    uint16_t temp = A - val;

    setPCarry(A >= val);
    setPZero((temp & 0xFF) == 0);
    setPNegative(temp & 0x80);
}

void CPU::CPX(uint8_t val) {
    uint16_t temp = X - val;

    setPCarry(X >= val);
    setPZero((temp & 0xFF) == 0);
    setPNegative(temp & 0x80);
}

void CPU::CPY(uint8_t val) {
    uint16_t temp = Y - val;

    setPCarry(Y >= val);
    setPZero((temp & 0xFF) == 0);
    setPNegative(temp & 0x80);
}

uint8_t CPU::DEC(uint8_t val) {
    val--;

    setPZero(val == 0);
    setPNegative(val & 0x80);

    return val;
}

void CPU::DEX() {
    X--;
    setPZero(X == 0);
    setPNegative(X & 0x80);
}

void CPU::DEY() {
    Y--;
    setPZero(Y == 0);
    setPNegative(Y & 0x80);
}

void CPU::EOR(uint8_t val) {
    A ^= val;
    setPZero(A == 0);
    setPNegative(A & 0x80);
}

uint8_t CPU::INC(uint8_t val) {
    val++;
    setPZero(val == 0);
    setPNegative(val & 0x80);
    return val;
}

void CPU::INX() {
    X++;
    setPZero(X == 0);
    setPNegative(X & 0x80);
}

void CPU::INY() {
    Y++;
    setPZero(Y == 0);
    setPNegative(Y & 0x80);
}

void CPU::JMP(uint16_t addr) {
    PC = addr;
}

uint8_t CPU::JSR(uint16_t addr) {
    uint16_t ret = PC - 1;

    write(getSPAddr(), (ret >> 8) & 0xFF);
    SP--;

    write(getSPAddr(), ret & 0xFF);
    SP--;

    PC = addr;
    return 0;
}

void CPU::LDA(uint8_t val) {
    A = val;
    setPZero(A == 0);
    setPNegative(A & 0x80);
}

void CPU::LDX(uint8_t val) {
    X = val;
    setPZero(X == 0);
    setPNegative(X & 0x80);
}

void CPU::LDY(uint8_t val) {
    Y = val;
    setPZero(Y == 0);
    setPNegative(Y & 0x80);
}

uint8_t CPU::LSR(uint8_t val) {
    setPCarry(val & 0x01);

    uint8_t result = val >> 1;

    setPZero(result == 0);
    setPNegative(0);

    return result;
}

void CPU::LSR_A() {
    setPCarry(A & 0x01);

    A >>= 1;

    setPZero(A == 0);
    setPNegative(0);
}

void CPU::NOP() {
}

void CPU::ORA(uint8_t val) {
    A |= val;
    setPZero(A == 0);
    setPNegative(A & 0x80);
}

void CPU::PHA() {
    write(getSPAddr(), A);
    SP--;
}

void CPU::PHP() {
    uint8_t p = P | 0x10; // B flag set when pushed
    write(getSPAddr(), p);
    SP--;
}

void CPU::PLA() {
    SP++;
    A = read(getSPAddr());

    setPZero(A == 0);
    setPNegative(A & 0x80);
}

void CPU::PLP() {
    SP++;
    P = read(getSPAddr());
    P &= ~0x10;  // clear B
    P |= 0x20;   // enforce bit 5
}

uint8_t CPU::ROL(uint8_t val) {
    uint8_t carryIn = getPCarry() ? 1 : 0;

    setPCarry(val & 0x80);

    uint8_t result = (val << 1) | carryIn;

    setPZero(result == 0);
    setPNegative(result & 0x80);

    return result;
}

void CPU::ROL_A() {
    uint8_t carryIn = getPCarry() ? 1 : 0;

    setPCarry(A & 0x80);

    A = (A << 1) | carryIn;

    setPZero(A == 0);
    setPNegative(A & 0x80);
}

uint8_t CPU::ROR(uint8_t val) {
    uint8_t carryIn = getPCarry() ? 1 : 0;

    setPCarry(val & 0x01);

    uint8_t result = (val >> 1) | (carryIn << 7);

    setPZero(result == 0);
    setPNegative(result & 0x80);

    return result;
}

void CPU::ROR_A() {
    uint8_t carryIn = getPCarry() ? 1 : 0;

    setPCarry(A & 0x01);

    A = (A >> 1) | (carryIn << 7);

    setPZero(A == 0);
    setPNegative(A & 0x80);
}

uint8_t CPU::RTI() {
    SP++;
    P = read(getSPAddr());
    P |= 0x20; // bit 5 always set
    P &= ~0x10; // clear B flag after pull

    SP++;
    uint8_t lo = read(getSPAddr());

    SP++;
    uint8_t hi = read(getSPAddr());

    PC = (hi << 8) | lo;

    return 0;
}

uint8_t CPU::RTS() {
    SP++;
    uint8_t lo = read(getSPAddr());

    SP++;
    uint8_t hi = read(getSPAddr());

    PC = ((hi << 8) | lo) + 1;

    return 0;
}

void CPU::SBC(uint8_t val) {
    uint16_t value = val ^ 0xFF;
    uint16_t sum = A + value + (getPCarry() ? 1 : 0);

    setPCarry(sum > 0xFF);
    setPZero((sum & 0xFF) == 0);
    setPOverflow(((A ^ sum) & (A ^ val) & 0x80) != 0);
    setPNegative(sum & 0x80);

    A = sum & 0xFF;
}

void CPU::SEC() {
    setPCarry(1);
}

void CPU::SED() {
    setPDecimal(1);
}

void CPU::SEI() {
    setPInterruptDisable(1);
}

void CPU::STA(uint16_t addr) {
    write(addr, A);
}

void CPU::STX(uint16_t addr) {
    write(addr, X);
}

void CPU::STY(uint16_t addr) {
    write(addr, Y);
}

void CPU::TAX() {
    X = A;
    setPZero(X == 0);
    setPNegative(X & 0x80);
}

void CPU::TAY() {
    Y = A;
    setPZero(Y == 0);
    setPNegative(Y & 0x80);
}

void CPU::TSX() {
    X = SP;
    setPZero(X == 0);
    setPNegative(X & 0x80);
}

void CPU::TXA() {
    A = X;
    setPZero(A == 0);
    setPNegative(A & 0x80);
}

void CPU::TXS() {
    SP = X;
}

void CPU::TYA() {
    A = Y;
    setPZero(A == 0);
    setPNegative(A & 0x80);
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

void CPU::stall(uint32_t cycles) {
    // accumulate with += because of possible multiple stalls
    remainingStallCycles += cycles;
}

bool CPU::stalled() {
    if (remainingStallCycles > 0) {
        totalCycles++;
        remainingStallCycles--;
        return true;
    }
    return false;
}

uint16_t CPU::getResetVectorPC() {
    return (read(0xFFFD) << 8) | read(0xFFFC);
}

void CPU::power() {
    A = 0x00;
    X = 0x00;
    Y = 0x00;
    PC = getResetVectorPC();
    SP = 0xFD; 
    P = 0x24; // enforce bit 5 with 0x20 and 0x04 (interrupt disable) is on initially
}

void CPU::reset() {
    // A, X, Y = unchanged
    PC = getResetVectorPC();

    SP -= 0x003;
    // P = unchanged except bit 2 = 1
    P |= 0x04; // set interrupt disable
    P |= 0x20; // enforce bit 5 = 1
}

void CPU::clock() {
    if (stalled())
        return;

    // If ready after previous instruction
    if (remainingCycles == 0) {
        uint8_t opcode = fetch(PC++);
        std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << "Executing opcode " << (int)opcode << " at " << (int)PC << std::endl;
        remainingCycles = execute(opcode);
    }

    // Prevents underflow from 0 to 255
    if (remainingCycles > 0)
        remainingCycles--;

    totalCycles++; // one clock = one cycle 
}

// Status (P) Registers Setters
void CPU::setPCarry(bool val) {
    if (val) P |= 0x01;
    else P &= ~0x01;
    P |= 0x20; // enforce bit 5 always 1
}
void CPU::setPZero(bool val) {
    P &= ~(1 << 1);
    P |= (val << 1);
    P |= 0x20; // enforce bit 5 always 1
}
void CPU::setPInterruptDisable(bool val) {
    P &= ~(1 << 2);
    P |= (val << 2);
    P |= 0x20; // enforce bit 5 always 1
}
void CPU::setPDecimal(bool val) {
    P &= ~(1 << 3);
    P |= (val << 3);
    P |= 0x20; // enforce bit 5 always 1
}
void CPU::setPBreak(bool val) {
    P &= ~(1 << 4);
    P |= (val << 4);
    P |= 0x20; // enforce bit 5 always 1
}
void CPU::setPOverflow(bool val) {
    P &= ~(1 << 6);
    P |= (val << 6);
    P |= 0x20; // enforce bit 5 always 1
}
void CPU::setPNegative(bool val) {
    P &= ~(1 << 7);
    P |= (val << 7);
    P |= 0x20; // enforce bit 5 always 1
}
// Status (P) Registers Getters
bool CPU::getPCarry() {
    return (P & 0x01);
}
bool CPU::getPZero() {
    return (P & 0x02);
}
bool CPU::getPInterruptDisable() {
    return (P & 0x04);
}
bool CPU::getPDecimal() {
    return (P & 0x08);
}
bool CPU::getPBreak() {
    return (P & 0x10);
}
bool CPU::getPOverflow() {
    return (P & 0x40);
}
bool CPU::getPNegative() {
    return (P & 0x80);
}
uint16_t CPU::getSPAddr() {
    return 0x0100 | SP;
}







