#include <iostream>
#include "bus.h"

int main() {
    Bus bus;

    // Load ROM
    if (!bus.getCartridge().loadNESFile("supermariobros.nes")) 
        return -1;
    //bus.getCartridge().print();
    bus.power();
    while (true)
        bus.clock();
    return 0;
}
