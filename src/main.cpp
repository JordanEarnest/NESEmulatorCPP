#include <iostream>
#include "bus.h"

int main() {
    Bus bus;
    bus.power();
    uint32_t cycles = bus.clock();
    std::cout << cycles << " cycles" << std::endl;
    return 0;

}
