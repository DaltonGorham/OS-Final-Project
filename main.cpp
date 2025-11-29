/*
CSCI 4300 Operating Systems Final Project
Names: Dalton Gorham, Aiden Cary
*/
#include <iostream>
#include "PageReplacement.h"

int main() {
    // Create PageReplacement instances for FIFO and Optimal algorithms
    PageReplacement firstInFirstOut("FIFO.txt");
    PageReplacement optimal("OPT.txt");

    // Run the FIFO & OPTIMAL algorithm and display results
    firstInFirstOut.runAlgorithm();
    optimal.runAlgorithm();
    
}