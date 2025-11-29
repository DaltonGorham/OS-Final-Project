#pragma once
#include <string>

class InputValidator {
public:
    static bool validateFile(std::string filename, std::string line);
private:
    static bool checkLineEmpty(std::string line, std::string filename);
    static bool checkAlgorithm(char algoChar, std::string filename);
    static bool checkFirstComma(std::string line, std::string filename);
    static bool checkReferenceString(std::string line, std::string filename);
};
