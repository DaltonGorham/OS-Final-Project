#include <iostream>
#include "PageReplacement.h"

int main() {
    // Create PageReplacement instances for FIFO and Optimal algorithms
    PageReplacement firstInFirstOut("FIFO.txt");
    PageReplacement optimal("OPT.txt");

    // Run the FIFO algorithm and display results
    firstInFirstOut.runAlgorithm();
    optimal.runAlgorithm();
    
}