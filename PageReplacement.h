#pragma once
#include <string>
#include <fstream>

class PageReplacement {
private:
    std::string m_algorithm;
    std::string m_referenceString;
    int m_numberOfFrames;
    void parseInputFile(std::string filename);
public:
    PageReplacement(std::string filename);
    void firstInFirstOut();
    void optimal();
    std::string getAlgorithm() { return m_algorithm; }
    std::string getReferenceString() { return m_referenceString; }
    int getNumberOfFrames() { return m_numberOfFrames; }
    void displayOutput();
    void printValues();
};