//
//  main.cpp
//  Squishy
//
//  Created by Saruggan Thiruchelvan on 2024-10-29.
//

#include <iostream>
#include <opencv2/opencv.hpp>

#include "huffman.h"

using namespace std;
using namespace cv;

int main(int argc, const char *argv[]) {
    // Load image from file
    string imagePath = "image.jpg";
    Mat image = imread(imagePath, IMREAD_COLOR);

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

    deleteTree(root);
    return 0;
}
