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
    uint16_t mirroredAddr = (addr & 0x0007) + 0x2000;        
    switch (mirroredAddr) {
        case ADDR_PPUCTRL:   return PPUCTRL;   break;
        case ADDR_PPUMASK:   return PPUMASK;   break;
        case ADDR_PPUSTATUS: return PPUSTATUS; break;
        case ADDR_OAMADDR:   return OAMADDR;   break;
        case ADDR_PPUSCROLL: return PPUSCROLL; break;
        case ADDR_PPUADDR:   return PPUADDR;   break;
        case ADDR_PPUDATA:   return PPUDATA;   break;
        case ADDR_OAMDMA:    return OAMDMA;    break;
        default:             return 0xFF;      break;
    }
}

void PPU::writeRegister(uint16_t addr, uint8_t val) {
    uint16_t mirroredAddr = (addr & 0x007) + 0x2000;
    switch (mirroredAddr) {
        case ADDR_PPUCTRL:   PPUCTRL   = val; break;
        case ADDR_PPUMASK:   PPUMASK   = val; break;
        case ADDR_PPUSTATUS: PPUSTATUS = val; break;
        case ADDR_OAMADDR:   OAMADDR   = val; break;
        case ADDR_OAMDATA:   OAMDATA   = val; break;
        case ADDR_PPUSCROLL: PPUSCROLL = val; break;
        case ADDR_PPUADDR:   PPUADDR   = val; break;
        case ADDR_PPUDATA:   PPUDATA   = val; break;
        case ADDR_OAMDMA:    OAMDMA    = val; break;
   }
}





