#include "PageReplacement.h"
#include <iostream>
#include <fstream>
#include <unordered_map>


PageReplacement::PageReplacement(std::string filename) {
    parseInputFile(filename);
}


int PageReplacement::runAlgorithm() {
    if (m_algorithm == "FIFO") return runFIFO();
    else if (m_algorithm == "OPTIMAL") return runOptimal();
    else std::cout << "Unknown Algorithm" << std::endl; return -1;
}

int PageReplacement::runFIFO() {
    // add code here
    return 0;
}

int PageReplacement::runOptimal() {
    int numOfPageFaults = 0;
    int frames[m_numberOfFrames];
    int count = 0;
    std::string referenceString = m_referenceString;
    
    for (int i = 0; i < referenceString.length(); i++) {
        int current = referenceString[i] - '0';
        
        bool found = false;
        for (int j = 0; j < count; j++) {
            if (frames[j] == current) {
                found = true;
                break;
            }
        }

        if (found) {
            continue;
        }

        numOfPageFaults++;

        if (count < m_numberOfFrames) {
            frames[count] = current;
            count++;
        } else {
            int farthest = i;
            int replace = 0;

            for (int k = 0; k < count; k++) {
                int nextUse = referenceString.length();

                for (int m = i + 1; m < referenceString.length(); m++) {
                    if ((referenceString[m] - '0') == frames[k]) {
                        nextUse = m;
                        break;
                    }
                }

                if (nextUse > farthest) {
                    farthest = nextUse;
                    replace = k;
                }
            }
            frames[replace] = current;
        }
    }
    return numOfPageFaults;
}

void PageReplacement::displayOutput(std::unordered_map<int, FrameState>){
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
