#include <iostream>
#include "PageReplacement.h"

int main() {
    PageReplacement firstInFirstOut("FIFO.txt");
    PageReplacement optimal("OPT.txt");

    optimal.runAlgorithm();
    
}