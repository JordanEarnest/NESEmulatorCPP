#pragma once
#include <stdint.h>
#include <iostream>
#include <iomanip>

class Bus;

class CPU {
    private:
        // CPU Registers
        uint8_t A, X, Y;
        uint16_t PC;
        uint8_t SP;
        uint8_t P;

        Bus* bus;

        // Determines how many more cycles before CPU is not stalled anymore
        uint32_t remainingStallCycles;
        // Timing delay for completing an instruction since execute() changes data instantly in emulator
        // We must simulate a waiting period to pretend like the CPU takes time for the instruction
        uint8_t remainingCycles;
    private:
        uint8_t fetch(uint16_t addr);
        uint8_t execute(uint8_t opcode);

        bool stalled();
        bool pageCrossed(uint16_t base, uint16_t addr);
        uint16_t getResetVectorPC();

        // Instruction Set
        void ADC(uint8_t val);
        void AND(uint8_t val);
        uint8_t ASL(uint8_t val);
        void ASL_A();
        uint8_t BCC();
        uint8_t BCS();
        uint8_t BEQ();
        void BIT(uint8_t val);
        uint8_t BMI();
        uint8_t BNE();
        uint8_t BPL();
        uint8_t BVC();
        uint8_t BVS();
        void CLC();
        uint8_t BRK();
        void CLD();
        void CLI();
        void CLV();
        void CMP(uint8_t val);
        void CPX(uint8_t val);
        void CPY(uint8_t val);
        uint8_t DEC(uint8_t val);
        void DEX();
        void DEY();
        void EOR(uint8_t val);
        uint8_t INC(uint8_t val);
        void INX();
        void INY();
        void JMP(uint16_t addr);
        uint8_t JSR(uint16_t addr);
        void LDA(uint8_t val);
        void LDX(uint8_t val);
        void LDY(uint8_t val);
        uint8_t LSR(uint8_t val);
        void LSR_A();
        void NOP();
        void ORA(uint8_t val);
        void PHA();
        void PHP();
        void PLA();
        void PLP();
        uint8_t ROL(uint8_t val);
        void ROL_A();
        uint8_t ROR(uint8_t val);
        void ROR_A();
        uint8_t RTI();
        uint8_t RTS();
        void SBC(uint8_t val);
        void SEC();
        void SED();
        void SEI();
        void STA(uint16_t addr);
        void STX(uint16_t addr);
        void STY(uint16_t addr);
        void TAX();
        void TAY();
        void TSX();
        void TXA();
        void TXS();
        void TYA();
    public:
        // Total number of cycles that have occurred so far
        uint32_t totalCycles;
    public:
        CPU();

        void power();
        void reset();

        void connect(Bus* bus);

        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t val);

        void stall(uint32_t cycles);

        void clock();

        // Register P (Status) Setters
        void setPCarry(bool val);
        void setPZero(bool val);
        void setPInterruptDisable(bool val);
        void setPDecimal(bool val);
        void setPBreak(bool val);
        void setPOverflow(bool val);
        void setPNegative(bool val);
        // Register P (Status) Getters
        bool getPCarry();
        bool getPZero();
        bool getPInterruptDisable();
        bool getPDecimal();
        bool getPBreak();
        bool getPOverflow();
        bool getPNegative(); 
        // Concatenates prefix 0x01 to SP to create address within SP location 0x0100-0x01FF
        uint16_t getSPAddr();
};
