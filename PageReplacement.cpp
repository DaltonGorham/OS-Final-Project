#include "PageReplacement.h"
#include <iostream>
#include <fstream>
#include <unordered_map>

PageReplacement::PageReplacement(std::string filename) {
    parseInputFile(filename);
}

void PageReplacement::runAlgorithm() {
    if (m_algorithm == "FIFO") return runFIFO();
    else if (m_algorithm == "OPTIMAL") return runOptimal();
    else std::cout << "Unknown Algorithm" << std::endl;
}

void PageReplacement::runFIFO() {
    // add code here
}

void PageReplacement::runOptimal() {
    int numOfPageFaults = 0;
    std::string referenceString = m_referenceString;
    int frames[m_numberOfFrames]; // array of our frames currently in memory
    int count = 0; // the number of frames currently in memory
    std::unordered_map<int, FrameState> frameHistroy; // map to store metadata for displaying
    
    // outermost loop that iterates through the reference string
    for (int i = 0; i < referenceString.length(); i++) {
        int current = referenceString[i] - '0'; // convert ascii to int
        
        // if its already in memory, we skip
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

        // its not already in memory so we add to our page faults
        numOfPageFaults++;

        // if we havent reached our frame limit yet, just add to memory
        if (count < m_numberOfFrames) {
            frames[count] = current;
            count++;
        // otherwise, we need to check which to replace based on how far until its used again
        } 
        else {
            int farthest = i; // tracks the farthest so far, first set to the current position in the reference string
            int replace = 0; // variable to track the index of which to replace

            // outer loop to iterate through what we have in memory currently
            for (int k = 0; k < count; k++) {
                int nextUse = referenceString.length(); // the index where its used next. first set to the length incase its not used again

                // inner loop to find the farthest
                for (int m = i + 1; m < referenceString.length(); m++) {
                    if ((referenceString[m] - '0') == frames[k]) {
                        nextUse = m;
                        break;
                    }
                }
                
                // if the next use is greater than the farthest so far
                // then it becomes the farthest and 
                // we know which index we want to replace
                if (nextUse > farthest) {
                    farthest = nextUse;
                    replace = k;
                }
            }
            frames[replace] = current;
        }
        FrameState state; // create a FrameState instance
        state.count = count; // save how many frames are currently filled
        for (int inMemory = 0; inMemory < count; inMemory++) { 
            state.frames[inMemory] = frames[inMemory]; // copy each page number to the state
        }
        frameHistroy[i] = state; // store in map like this key: index, value: {count:3, frames[1,4,3]}
    }
    // display results
    displayOutput(frameHistroy);
    std::cout << "Number of Page Faults: " << numOfPageFaults << std::endl;
}

void PageReplacement::displayOutput(std::unordered_map<int, FrameState> frameHistory){
    std::cout << "Running Page Replacement Algorithm with the following:\n\n";
    printValues();
    std::cout << std::endl;

     // print reference string at top
    for (int i = 0; i < m_referenceString.length(); i++) {
        std::cout << m_referenceString[i] << " ";
    }
    std::cout << std::endl;

    // print line separator. also account for spaces
    for (int i = 0; i < m_referenceString.length() * 2; i++) {
        std::cout << "-";
    }
    std::cout << std::endl;

    // print formatted output
    for (int row = 0; row < m_numberOfFrames; row++) {
        for (int col = 0; col < m_referenceString.length(); col++) {
            // if there was a page fault print it, otherwise print blank
            if (frameHistory.find(col) != frameHistory.end() && row < frameHistory[col].count) {
                std::cout << frameHistory[col].frames[row] << " ";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
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
    } else {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
}

void PageReplacement::printValues() {
    std::cout << "Algorithm: " << m_algorithm << std::endl;
    std::cout << "Number of Frames: " << m_numberOfFrames << std::endl;
    std::cout << "Reference String: " << getReferenceString() << std::endl;
}
