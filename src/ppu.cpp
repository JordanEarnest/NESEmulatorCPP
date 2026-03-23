#include "ppu.h"

void PPU::power() {
    // Initial register values on power up
    PPUCTRL = 0x00;
    PPUMASK = 0x00;
    PPUSTATUS = 0x00;
    OAMADDR = 0x00;
    OAMDATA = 0x00;
    PPUSCROLL = 0x00;
    PPUADDR = 0x00;
    PPUDATA = 0x00;
}

void PPU::reset() {
    PPUCTRL = 0x00;
    PPUMASK = 0x00;
    PPUSTATUS = 0x00;
    // OAMADDR = unchanged
    PPUSCROLL = 0x00;
    // PPUADDR = unchanged
    PPUDATA = 0x00;
}

uint8_t PPU::readRegister(uint16_t addr) {          
    switch (addr) { // addr already been sanitized in bus with mirroring
        case ADDR_PPUCTRL:   return PPUCTRL;      break;
        case ADDR_PPUMASK:   return PPUMASK;      break;
        case ADDR_PPUSTATUS: return PPUSTATUS;    break;
        case ADDR_OAMADDR:   return OAMADDR;      break;
        case ADDR_OAMDATA:   return oam[OAMADDR]; break;
        case ADDR_PPUSCROLL: return PPUSCROLL;    break;
        case ADDR_PPUADDR:   return PPUADDR;      break;
        case ADDR_PPUDATA:   return PPUDATA;      break;
        default:             return 0xFF;         break;
    }
}

void PPU::writeRegister(uint16_t addr, uint8_t val) {
    switch (addr) { // addr already been sanitized in bus with mirroring
        case ADDR_PPUCTRL:   PPUCTRL   = val; break;
        case ADDR_PPUMASK:   PPUMASK   = val; break;
        case ADDR_PPUSTATUS: /*readonly*/     break;
        case ADDR_OAMADDR: {
            OAMADDR = val;
            break;
        }
        case ADDR_OAMDATA: {
            oam[OAMADDR] = val;
            OAMADDR++; // auto-increment is a hardware behavior
            break;
        }
        case ADDR_PPUSCROLL: PPUSCROLL = val; break;
        case ADDR_PPUADDR:   PPUADDR   = val; break;
        case ADDR_PPUDATA:   PPUDATA   = val; break;
   }
}

void PPU::clock() {
    return;
}