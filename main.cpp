#include <iostream>
#include "PageReplacement.h"

int main() {
    PageReplacement firstInFirstOut("FIFO.txt");
    PageReplacement optimal("OPT.txt");

    int numberOfPageFaults = optimal.runAlgorithm();
    std::cout << numberOfPageFaults << std::endl;
    
    numberOfPageFaults = firstInFirstOut.runAlgorithm();
    std::cout << numberOfPageFaults << std::endl;
}