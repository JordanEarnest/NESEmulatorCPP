#include "apu.h"

void APU::power() {
    SQ1_VOL = 0x00;
    SQ1_SWEEP = 0x00;
    SQ1_LO = 0x00;
    SQ1_HI = 0x00;

    SQ2_VOL = 0x00;
    SQ2_SWEEP = 0x00;
    SQ2_LO = 0x00;
    SQ2_HI = 0x00;

    TRI_LINEAR = 0x00;
    TRI_LO = 0x00;
    TRI_HI = 0x00;

    NOISE_VOL = 0x00;
    NOISE_LO = 0x00;
    NOISE_HI = 0x00;

    DMC_FREQ = 0x00;
    DMC_RAW = 0x00;
    DMC_START = 0x00;
    DMC_LEN = 0x00;

    SND_CHN = 0x00;

    JOY2 = 0x00;
}

void APU::reset() {
    SQ1_VOL = 0x00;
    SQ1_SWEEP = 0x00;
    SQ1_LO = 0x00;
    SQ1_HI = 0x00;

    SQ2_VOL = 0x00;
    SQ2_SWEEP = 0x00;
    SQ2_LO = 0x00;
    SQ2_HI = 0x00;

    TRI_LINEAR = 0x00;
    TRI_LO = 0x00;
    TRI_HI = 0x00;

    NOISE_VOL = 0x00;
    NOISE_LO = 0x00;
    NOISE_HI = 0x00;

    DMC_FREQ = 0x00;
    DMC_RAW = 0x00;
    DMC_START = 0x00;
    DMC_LEN = 0x00;

    SND_CHN = 0x00;

    JOY2 = 0x00;
}

uint8_t APU::readRegister(uint16_t addr) {
    switch (addr) {
        case ADDR_SQ1_VOL:    return SQ1_VOL;    break;
        case ADDR_SQ1_SWEEP:  return SQ1_SWEEP;  break;
        case ADDR_SQ1_LO:     return SQ1_LO;     break;
        case ADDR_SQ1_HI:     return SQ1_HI;     break;

        case ADDR_SQ2_VOL:    return SQ2_VOL;    break;
        case ADDR_SQ2_SWEEP:  return SQ2_SWEEP;  break;
        case ADDR_SQ2_LO:     return SQ2_LO;     break;
        case ADDR_SQ2_HI:     return SQ2_HI;     break;

        case ADDR_TRI_LINEAR: return TRI_LINEAR; break;
        case ADDR_TRI_LO:     return TRI_LO;     break;
        case ADDR_TRI_HI:   return TRI_HI;     break;

        case ADDR_NOISE_VOL:  return NOISE_VOL;  break;
        case ADDR_NOISE_LO:   return NOISE_LO;   break;
        case ADDR_NOISE_HI:   return NOISE_HI;   break;

        case ADDR_DMC_FREQ:   return DMC_FREQ;   break;
        case ADDR_DMC_RAW:    return DMC_RAW;    break;
        case ADDR_DMC_START:  return DMC_START;  break;
        case ADDR_DMC_LEN:    return DMC_LEN;    break;

        case ADDR_SND_CHN:    return SND_CHN;    break;

        case ADDR_JOY2:       return JOY2;       break;
        default:              return 0xFF;       break;
    } 
}

void APU::writeRegister(uint16_t addr, uint8_t val) {
    switch (addr) {
        case ADDR_SQ1_VOL:    SQ1_VOL = val;    break;
        case ADDR_SQ1_SWEEP:  SQ1_SWEEP = val;  break;
        case ADDR_SQ1_LO:     SQ1_LO = val;     break;
        case ADDR_SQ1_HI:     SQ1_HI = val;     break;
                                 
        case ADDR_SQ2_VOL:    SQ2_VOL = val;    break;
        case ADDR_SQ2_SWEEP:  SQ2_SWEEP = val;  break;
        case ADDR_SQ2_LO:     SQ2_LO = val;     break;
        case ADDR_SQ2_HI:     SQ2_HI = val;     break;
                               
        case ADDR_TRI_LINEAR: TRI_LINEAR = val; break;
        case ADDR_TRI_LO:     TRI_LO = val;     break;
        case ADDR_TRI_HI:     TRI_HI = val;     break;
                               
        case ADDR_NOISE_VOL:  NOISE_VOL = val;  break;
        case ADDR_NOISE_LO:   NOISE_LO = val;   break;
        case ADDR_NOISE_HI:   NOISE_HI = val;   break;
                                
        case ADDR_DMC_FREQ:   DMC_FREQ = val;   break;
        case ADDR_DMC_RAW:    DMC_RAW = val;    break;
        case ADDR_DMC_START:  DMC_START = val;  break;
        case ADDR_DMC_LEN:    DMC_LEN = val;    break;
                               
        case ADDR_SND_CHN:    SND_CHN = val;    break;
                               
        case ADDR_JOY2:       JOY2 = val;       break;
    } 
}
