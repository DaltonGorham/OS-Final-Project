#include <iostream>
#include "PageReplacement.h"

int main() {
    PageReplacement firstInFirstOut("FIFO.txt");
    PageReplacement optimal("OPT.txt");

    int numberOfPageFaults = optimal.runAlgorithm();
    std::cout << "Number of Page Faults: " << numberOfPageFaults << std::endl;
    
}