#pragma once
#include <string>
#include <fstream>
#include <unordered_map>

const int MAX_FRAMES = 15;

struct FrameState {
    int frames[MAX_FRAMES];
    int count;
};

class PageReplacement {
private:
    std::string m_algorithm;
    std::string m_referenceString;
    int m_numberOfFrames;
    void parseInputFile(std::string filename);
    int runFIFO();
    int runOptimal();
public:
    PageReplacement(std::string filename);
    int runAlgorithm();
    void displayOutput(std::unordered_map<int, FrameState>);
    void printValues();
    std::string getAlgorithm() { return m_algorithm; }
    std::string getReferenceString() { return m_referenceString; }
    int getNumberOfFrames() { return m_numberOfFrames; }
};