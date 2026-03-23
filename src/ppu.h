#pragma once
#include <stdint.h>

class PPU {
    private:
        // Addresses for PPU Registers
        static constexpr uint16_t ADDR_PPUCTRL = 0x2000;
        static constexpr uint16_t ADDR_PPUMASK = 0x2001;
        static constexpr uint16_t ADDR_PPUSTATUS = 0x2002;
        static constexpr uint16_t ADDR_OAMADDR = 0x2003;
        static constexpr uint16_t ADDR_OAMDATA = 0x2004;
        static constexpr uint16_t ADDR_PPUSCROLL = 0x2005;
        static constexpr uint16_t ADDR_PPUADDR = 0x2006;
        static constexpr uint16_t ADDR_PPUDATA = 0x2007;

        // PPU Registers
        uint8_t PPUCTRL;    // 0x2000 W
        uint8_t PPUMASK;    // 0x2001 W
        uint8_t PPUSTATUS;  // 0x2002 R
        uint8_t OAMADDR;    // 0x2003 W
        uint8_t OAMDATA;    // 0x2004 RW
        uint8_t PPUSCROLL;  // 0x2005 Wx2
        uint8_t PPUADDR;    // 0x2006 Wx2
        uint8_t PPUDATA;    // 0x2007 RW
    public:
        uint8_t oam[256] = {}; // 256 bytes of OAM memory
    public:
        void reset();
        void power();

        uint8_t readRegister(uint16_t addr);
        void writeRegister(uint16_t addr, uint8_t val);

        void clock();

};

