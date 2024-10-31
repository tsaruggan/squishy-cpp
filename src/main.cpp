//
//  main.cpp
//  Squishy
//
//  Created by Saruggan Thiruchelvan on 2024-10-29.
//

#include <iostream>
#include <opencv2/opencv.hpp>

#include "huffman.h"
#include "io.h"
#include "codec.h"

using namespace std;
using namespace cv;

int main(int argc, const char *argv[]) {
    const string INPUT_FILENAME =  "curry.jpg";
    const string OUTPUT_FILENAME = "output.bin";

    // Load image from file
    Mat image = imread(INPUT_FILENAME, IMREAD_COLOR);

    // Get frequency counts
    vector<pair<int, int>> frequencies = getFrequencies(image);

    // Build tree
    HuffmanNode* root = buildTree(frequencies);

    // Assign binary patterns
    unordered_map<int, vector<int>> patternAssignments = assignBinaryPatterns(root);

    // Display binary patterns
    for (const pair<int, vector<int>>& assignment: patternAssignments) {
        int value = assignment.first;
        vector<int> pattern = assignment.second;
        cout << value << ": {";
        for (const int bit: pattern) {
            cout << bit;
        }
        cout << "}" << endl;
    }

    Output* outputStream = new Output(OUTPUT_FILENAME);
    Encoder encoder = Encoder(outputStream);

    encoder.encodeHeader(image);
    encoder.encodeTree(root);
    encoder.encodePixels(image, patternAssignments);

    // Clean-up & exit
    delete root;
    delete outputStream;
    return 0;
}