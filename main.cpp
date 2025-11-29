/*
CSCI 4300 Operating Systems Final Project
Names: Dalton Gorham, Aiden Cary
*/
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cctype>

// current frame max we expect from input file
const int MAX_FRAMES = 15;

// store metadata for displaying
struct FrameState {
    int frames[MAX_FRAMES];
    int count;
};

// validate the input file
class InputValidator {
public:
    static bool validateFile(std::string filename, std::string line);
private:
    static bool checkLineEmpty(std::string line, std::string filename);
    static bool checkAlgorithm(char algoChar, std::string filename);
    static bool checkFirstComma(std::string line, std::string filename);
    static bool checkReferenceString(std::string line, std::string filename);
};

// main validation method that calls all the other checks
bool InputValidator::validateFile(std::string filename, std::string line) {
    if (!checkLineEmpty(line, filename)) return false;
    if (!checkAlgorithm(line[0], filename)) return false;
    if (!checkFirstComma(line, filename)) return false;
    if (!checkReferenceString(line, filename)) return false;
    return true;
}

// check if the line from the file is empty
bool InputValidator::checkLineEmpty(std::string line, std::string filename) {
    if (line.empty()) {
        std::cerr << "Error: File is empty: " << filename << std::endl;
        std::cerr << "Expected format: Algorithm,NumFrames,Page1,Page2,..." << std::endl;
        return false;
    }
    return true;
}

// check if the algorithm character is valid (F or O)
bool InputValidator::checkAlgorithm(char algoChar, std::string filename) {
    if (algoChar != 'F' && algoChar != 'O') {
        std::cerr << "Error: Invalid algorithm '" << algoChar << "' in file: " << filename << std::endl;
        std::cerr << "Expected 'F' (FIFO) or 'O' (OPTIMAL)" << std::endl;
        return false;
    }
    return true;
}

// check if there's a comma after the algorithm character
bool InputValidator::checkFirstComma(std::string line, std::string filename) {
    if (line[1] != ',') {
        std::cerr << "Error: Missing comma after algorithm in file: " << filename << std::endl;
        std::cerr << "Expected format: Algorithm,NumFrames,..." << std::endl;
        return false;
    }
    return true;
}

// validate the reference string contains only digits and commas
bool InputValidator::checkReferenceString(std::string line, std::string filename) {
    // check minimum length for valid format
    if (line.length() < 5) {
        std::cerr << "Error: No reference string found in file: " << filename << std::endl;
        std::cerr << "Expected format: Algorithm,NumFrames,Page1,Page2,..." << std::endl;
        return false;
    }

    // verify reference string has at least one digit and only valid characters
    bool hasAtLeastOneDigit = false;
    for (int i = 4; i < line.length(); i++) {
        char c = line[i];
        if (isdigit(c)) {
            hasAtLeastOneDigit = true;
        } else if (c == ',') {
            continue;
        } else {
            std::cerr << "Error: Invalid character `" << c << "` in reference string in file: " << filename << std::endl;
            std::cerr << "Reference string should only contain digits (0-9) and commas" << std::endl;
            return false;
        }
    }
    
    if (!hasAtLeastOneDigit) {
        std::cerr << "Error: Reference string contains no page numbers in file: " << filename << std::endl;
        return false;
    }
    return true;
}

// page replacement class to handle core algorithm logic
class PageReplacement {
private:
    std::string m_algorithm;
    std::string m_referenceString;
    int m_numberOfFrames;
    
    void parseInputFile(std::string filename);
    void runFIFO();
    void runOptimal();
    void displayOutput(std::unordered_map<int, FrameState>);
    
public:
    PageReplacement(std::string filename);
    void runAlgorithm();
    void printValues();
    
    std::string getAlgorithm() { return m_algorithm; }
    int getNumberOfFrames() { return m_numberOfFrames; }
    std::string getReferenceString() {
        std::string formattedString = "";
        for (int i = 0; i < m_referenceString.length(); i++) {
            formattedString += m_referenceString[i];
            if (i < m_referenceString.length() - 1) {
                formattedString += ",";
            }
        }
        return formattedString;
    }
};

// constructor that takes in a filename and parses it
PageReplacement::PageReplacement(std::string filename) {
    parseInputFile(filename);
}

// run the appropriate algorithm based on what was in the file
void PageReplacement::runAlgorithm() {
    if (m_algorithm == "FIFO") return runFIFO();
    else if (m_algorithm == "OPTIMAL") return runOptimal();
    else std::cout << "Unknown Algorithm" << std::endl;
}

void PageReplacement::runFIFO() {
    int numOfPageFaults = 0;
    std::string referenceString = m_referenceString;
    int frames[m_numberOfFrames]; // array of our frames currently in memory
    int count = 0; // the number of frames currently in memory
    std::unordered_map<int, FrameState> frameHistroy; // map to store metadata for displaying
    int k = 0; // pointer to track which frame to replace next
    
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
        // otherwise, we need to replace the oldest one (FIFO)
        } 
        else {
            // Replace the oldest frame
            frames[k] = current;
            // Move the pointer to the next frame
            k += 1;
            // Wrap around if we reach the end
            if (k >= m_numberOfFrames) {
                k = 0;
            }
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
    std::cout << "Running Page Replacement Algorithm for the following:\n\n";
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
    
    // Check if file opened successfully
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        std::cerr << "Please ensure the file exists and is readable." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // Read the line
    std::getline(file, line);
    
    // Validate the file format
    if (!InputValidator::validateFile(filename, line)) {
        file.close();
        exit(EXIT_FAILURE);
    }

    // now thats its in a format we can handle, parse it and set member vars
    m_algorithm = line[0];
    if (m_algorithm == "F") m_algorithm = "FIFO";
    if (m_algorithm == "O") m_algorithm = "OPTIMAL";
    m_numberOfFrames = line[2] - '0';

    // create our reference string
    for (int i = 3; i < line.length(); i++) {
        if (line[i] != ',') {
            m_referenceString += line[i];
        }
    }
    
    file.close();
}

// print out the parsed values from the input file
void PageReplacement::printValues() {
    std::cout << "Algorithm: " << m_algorithm << std::endl;
    std::cout << "Number of Frames: " << m_numberOfFrames << std::endl;
    std::cout << "Reference String: " << getReferenceString() << std::endl;
}

int main() {
    // Create PageReplacement instances for FIFO and Optimal algorithms
    PageReplacement firstInFirstOut("FIFO.txt");
    PageReplacement optimal("OPT.txt");

    // Run the FIFO & OPTIMAL algorithm and display results
    firstInFirstOut.runAlgorithm();
    optimal.runAlgorithm();
    
}