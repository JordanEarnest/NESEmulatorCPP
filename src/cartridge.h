#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

class Cartridge {
    private:
        // Sizes depend on specific cartridge variety so dynamic arrays are used
        std::vector<uint8_t> prgROM; // game code
        std::vector<uint8_t> chrROM; // used by ppu, stores graphics
        std::vector<uint8_t> chrRAM; // used by ppu, stores graphics, rewritable by CPU
        std::vector<uint8_t> prgRAM; // save ram

        // Data from iNES header
        uint8_t numPrgROMBanks;
        uint8_t numPrgRAMUnits;
        uint8_t numChrROMBanks;
        bool hasChrROM;

        bool verticalMirroring;
        bool batteryPackedPrgRAM;
        bool trainerPresent;
        bool fourScreenVRAM;

        bool vsUnisystem;
        bool playchoice10;
        bool iNES2Indicator;

        bool NTSC; // alternative is PAL

        uint8_t mapperID;

    private:
        bool interpretiNESHeader(std::ifstream& file);
        bool loadROM(std::ifstream& file);
    public:
        bool loadNESFile(const std::string& path);

        uint8_t read(uint16_t addr);
        void write(uint16_t addr, uint8_t val);

        void print();
};
