#include "PageReplacement.h"
#include <iostream>
#include <fstream>


PageReplacement::PageReplacement(std::string filename) {
    parseInputFile(filename);
}

void PageReplacement::firstInFirstOut() {
    // need to write this here...
}

void PageReplacement::optimal() {
    // need to make this....
}

void PageReplacement::displayOutput() {
    // placeholder for now
    // will eventually need to look similar to this output
    /*
        1 2 3 4 1 2 5 1 2 3 4 5
--------------------------------------------------------------------
        1 1 1 1     1       4
          2 2 2     2       2
            3 3     3       3
              4     5       5
    */
    std::cout << "You are using the " << m_algorithm << " Page Replacement Algorithm!" << std::endl;
}

void PageReplacement::parseInputFile(std::string filename) {
    std::ifstream file(filename);
    std::string line;
    
    if (file.is_open()) {
        std::getline(file, line);

          // we already know this is the format she has in the files
          m_algorithm = line[0];
          if (m_algorithm == "F") m_algorithm = "FIFO";
          if (m_algorithm == "O") m_algorithm = "OPTIMAL";
          m_numberOfFrames = line[2] - '0'; // quick hack to make it an int and not a ascii char

          for (int i = 3; i < line.length(); i++) {
                if (line[i] != ',') {
                    m_referenceString += line[i];
                }
          }
          
        file.close();
    }
}

// sanity check for debugging stuff. prob wont need this later
void PageReplacement::printValues() {
    std::cout << "Algorithm: " << m_algorithm << std::endl;
    std::cout << "Number of Frames: " << m_numberOfFrames << std::endl;
    std::cout << "Reference String: " << m_referenceString << std::endl;
}
