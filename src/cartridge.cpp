#include "cartridge.h"

bool Cartridge::loadNESFile(const std::string& path) {
    // Open up file
    std::ifstream file(path, std::ios::in | std::ios::binary);

    if (!file) {
        std::cout << "[ERROR] Failed to open ROM at " << path << std::endl;
        return false;
    }

    // Read iNES header and adjusts object data for spescific model of cartridge
    if (!interpretiNESHeader(file))
        return false;

    // Get ROM data from .nes file and place in cartridge vectors
    if (!loadROM(file))
        return false;

    return true;
}

bool Cartridge::interpretiNESHeader(std::ifstream& file) {
    uint8_t header[16]; // typical header is 16 bytes
    file.read(reinterpret_cast<char*>(header), 16); // put header info in file into array

    // Byte 0-3: File Signature ("NES.")
    // Note: 0x1A is a legacy end-of-file marker, often used to represent Ctrl+Z in systems
    if (header[0] != 'N' || header[1] != 'E' || header [2] != 'S' || header[3] != 0x1A) {
        std::cout << "[ERROR] Incorrect File Signature when interpreting ROM header... should be iNES" << std::endl;
        return false;
    }
    // Byte 4: PRG ROM size
    numPrgROMBanks = header[4]; // Number of 16KB PRG-ROM banks
    prgROM.resize(numPrgROMBanks * 0x4000); // 0x4000 = 16KB
    // Byte 5: CHR ROM size
    numChrROMBanks = header[5]; // Number of 8KB CHR-ROM banks
    if (numChrROMBanks != 0) { 
        chrROM.resize(numChrROMBanks * 0x2000); // 0x2000 = 8KB
        hasChrROM = true;
    } else { // use ChrRAM instead
        chrRAM.resize(0x2000); // 0x2000 = 8KB
        hasChrROM = false;
    }
    // Byte 6: Flag 6
    uint8_t flag6 = header[6];
    verticalMirroring = flag6 & 0x01;
    batteryPackedPrgRAM = flag6 & 0x02;
    trainerPresent = flag6 & 0x04;
    fourScreenVRAM = flag6 & 0x08;
    uint8_t mapperLowerNibble = (flag6 >> 4) & 0x0F;
    // Byte 7: Flag 7
    uint8_t flag7 = header[7];
    vsUnisystem = flag7 & 0x01;
    playchoice10 = (flag7 & 0x02);
    iNES2Indicator = (flag7 & 0x0C) == 0x08; 
    if (iNES2Indicator) {
        std::cout << "[ERROR] Found cartridge using iNES 2.0 which is unsupported" << std::endl;
        return false;
    }
    uint8_t mapperHigherNibble = flag7 & 0xF0;
    mapperID = mapperHigherNibble | mapperLowerNibble;
    // Byte 8: PRG RAM size
    numPrgRAMUnits = header[8]; // each unit = 8KB
    if (numPrgRAMUnits == 0) // default 0 is just 1 or 8KB
        numPrgRAMUnits = 1;
    prgRAM.resize(numPrgRAMUnits * 0x2000);
    // Byte 9: TV System
    uint8_t byte9 = header[9];
    NTSC = byte9 & 0x01;
    // Byte 10: Misc Flags IGNORED
    // Byte 11-15 Zero Padding:
    // iNES 1.0: ignore padding (header[11..15] can be garbage)

    return true;
}
bool Cartridge::loadROM(std::ifstream& file) {
    // If trainer exists in ROM, skip ahead to program code
    // Trainer = 512 bytes, stored between header and program code
    // Read trainer and put it into prgRAM
    if (trainerPresent) 
        file.read(reinterpret_cast<char*>(prgRAM.data() + 0x1000), 512);

    if (!file) {
        std::cout << "[ERROR] Failed to read trainer data in ROM" << std::endl;
        return false;
    }
    
    // Read Program Code and put it into vector
    file.read(reinterpret_cast<char*>(prgROM.data()), numPrgROMBanks * 0x4000);

    if (!file) {
        std::cout << "[ERROR] Failed to read program code in ROM" << std::endl;
        return false;
    }
    
    // Read ChrROM if exists, else cartridge uses ChrRAM written to by CPU
    if (hasChrROM) 
        file.read(reinterpret_cast<char*>(chrROM.data()), numChrROMBanks * 0x2000);

    if (!file) {
        std::cout << "[ERROR] Failed to read ChrROM data in ROM" << std::endl;
        return false;
    }
    return true;
}

    uint8_t Cartridge::read(uint16_t addr) {
        // TODO: use mapper logic, some games have swappable banks
        // RAM
        if (addr >= 0x6000 && addr <= 0x7FFF) {
            return prgRAM[addr - 0x6000];
        }
        // ROM
        else if (addr >= 0x8000) {
            // printf("0x%X\n", addr);
            // printf("0x%X\n", addr-0x8000);
            // printf("0x%X\n", prgROM[addr-0x8000]);
            return prgROM[addr - 0x8000];
        }
        return 0xFF;
    }

void Cartridge::write(uint16_t addr, uint8_t val) {
    // RAM 
    if (addr <= 0x7FFF) {
        prgRAM[addr - 0x6000] = val;
    }
    else if (addr <= 0xFFFF) {
        return; // prgROM is read only!
    }
}

void Cartridge::print() {
    // print program code for debugging
    for (int i = 0; i < chrROM.size(); i++) {
        std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)chrROM[i] << " ";
        if ((i+1) % 16 == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;            // error 0x400D should be 9F not 04
    //std::cout << std::hex << "Ended print with size of " << (int)prgROM[0x400D - 0x0010] << std::endl;
}