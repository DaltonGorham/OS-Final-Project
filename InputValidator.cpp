#include "InputValidator.h"
#include <iostream>
#include <fstream>
#include <cctype>

bool InputValidator::validateFile(std::string filename, std::string line) {
    // Check if file is empty
    if (!checkLineEmpty(line, filename)) {
        return false;
    }
    
    // Validate algorithm character
    if (!checkAlgorithm(line[0], filename)) {
        return false;
    }
    
    // Check first comma
    if (!checkFirstComma(line, filename)) {
        return false;
    }

    // check the reference string
    if (!checkReferenceString(line, filename)) {
        return false;
    }
    
    return true;
}

bool InputValidator::checkLineEmpty(std::string line, std::string filename) {
    if (line.empty()) {
        std::cerr << "Error: File is empty: " << filename << std::endl;
        std::cerr << "Expected format: Algorithm,NumFrames,Page1,Page2,..." << std::endl;
        return false;
    }
    return true;
}

bool InputValidator::checkAlgorithm(char algoChar, std::string filename) {
    if (algoChar != 'F' && algoChar != 'O') {
        std::cerr << "Error: Invalid algorithm '" << algoChar << "' in file: " << filename << std::endl;
        std::cerr << "Expected 'F' (FIFO) or 'O' (OPTIMAL)" << std::endl;
        return false;
    }
    return true;
}

bool InputValidator::checkFirstComma(std::string line, std::string filename) {
    if (line[1] != ',') {
        std::cerr << "Error: Missing comma after algorithm in file: " << filename << std::endl;
        std::cerr << "Expected format: Algorithm,NumFrames,..." << std::endl;
        return false;
    }
    return true;
}


bool InputValidator::checkReferenceString(std::string line, std::string filename) {
    if (line.length() < 5) {
        std::cerr << "Error: No reference string found in file: " << filename << std::endl;
        std::cerr << "Expected format: Algorithm,NumFrames,Page1,Page2,..." << std::endl;
        return false;
    }

    // check if there is any reference string
    bool hasAtLeastOneDigit = false;
    
    for (int i = 4; i < line.length(); i++) {
        char c = line[i];
        
        if (isdigit(c)) {
            hasAtLeastOneDigit = true;
        } 
        else if (c == ',') {
            continue;
        } 
        else {
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
