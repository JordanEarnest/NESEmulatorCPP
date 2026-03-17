#pragma once
#include <stdint.h>

class APU {
    private:
        // Addresses for APU registers 
        static constexpr uint16_t ADDR_SQ1_VOL = 0x4000;
        static constexpr uint16_t ADDR_SQ1_SWEEP = 0x4001;
        static constexpr uint16_t ADDR_SQ1_LO = 0x4002;
        static constexpr uint16_t ADDR_SQ1_HI = 0x4003;

        static constexpr uint16_t ADDR_SQ2_VOL = 0x4004;
        static constexpr uint16_t ADDR_SQ2_SWEEP = 0x4005;
        static constexpr uint16_t ADDR_SQ2_LO = 0x4006;
        static constexpr uint16_t ADDR_SQ2_HI = 0x4007;

        static constexpr uint16_t ADDR_TRI_LINEAR = 0x4008;
        static constexpr uint16_t ADDR_TRI_LO = 0x400A;
        static constexpr uint16_t ADDR_TRI_HI = 0x400B;

        static constexpr uint16_t ADDR_NOISE_VOL = 0x400C;
        static constexpr uint16_t ADDR_NOISE_LO = 0x400E;
        static constexpr uint16_t ADDR_NOISE_HI = 0x400F;

        static constexpr uint16_t ADDR_DMC_FREQ = 0x4010;
        static constexpr uint16_t ADDR_DMC_RAW = 0x4011;
        static constexpr uint16_t ADDR_DMC_START = 0x4012;
        static constexpr uint16_t ADDR_DMC_LEN = 0x4013;

        static constexpr uint16_t ADDR_SND_CHN = 0x4015;

        static constexpr uint16_t ADDR_JOY2 = 0x4017;

        // Registers
        uint8_t SQ1_VOL;
        uint8_t SQ1_SWEEP;
        uint8_t SQ1_LO;
        uint8_t SQ1_HI;

        uint8_t SQ2_VOL;
        uint8_t SQ2_SWEEP;
        uint8_t SQ2_LO;
        uint8_t SQ2_HI;

        uint8_t TRI_LINEAR;
        uint8_t TRI_LO;
        uint8_t TRI_HI;

        uint8_t NOISE_VOL;
        uint8_t NOISE_LO;
        uint8_t NOISE_HI;

        uint8_t DMC_FREQ;
        uint8_t DMC_RAW;
        uint8_t DMC_START;
        uint8_t DMC_LEN;

        uint8_t SND_CHN;

        uint8_t JOY2; // Frame Counter

    public:
        void power();
        void reset();

        uint8_t readRegister(uint16_t addr);
        void writeRegister(uint16_t addr, uint8_t val);
};

