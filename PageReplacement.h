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
    std::string inline getAlgorithm() { return m_algorithm; }
    int inline getNumberOfFrames() { return m_numberOfFrames; }
    std::string inline getReferenceString() {
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