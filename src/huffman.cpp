//
//  huffman.cpp
//  Squishy
//
//  Created by Saruggan Thiruchelvan on 2024-10-30.
//

#include <iostream>
#include <opencv2/opencv.hpp>

#include "huffman.h"

using namespace std;
using namespace cv;

HuffmanNode::HuffmanNode(int value, int count) {
    this->value = value;
    this->count = count;
}

HuffmanNode::HuffmanNode(int count) {
    this->count = count;
}

HuffmanNode::HuffmanNode(HuffmanNode* left, HuffmanNode* right) {
    this->left = left;
    this->right = right;
}

HuffmanNode::~HuffmanNode() {
    delete left;
    delete right;
}

// Build a Huffman tree from frequency list
HuffmanNode* buildTree(const vector<pair<int, int>>& frequencies) {
    // Create HuffmanNodes from frequencies
    vector<HuffmanNode*> nodes; 
    for (const pair<int, int>& frequency : frequencies) {
        int value = frequency.first;
        int count = frequency.second;
        HuffmanNode* node = new HuffmanNode(value, count);
        nodes.push_back(node);
    }

    // Build the tree
    while (nodes.size() > 1) {
        sort(nodes.begin(), nodes.end(),
            [](const HuffmanNode* node1, const HuffmanNode* node2){
                int count1 = node1->count;
                int count2 = node2->count;
                return count1 > count2;
            });

        // Create a new internal HuffmanNode with the two lowest frequencies
        HuffmanNode* left = nodes.back(); 
        nodes.pop_back();
        HuffmanNode* right = nodes.back(); 
        nodes.pop_back();

        HuffmanNode* branch = new HuffmanNode(left->count + right->count);
        branch->left = left;
        branch->right = right;

        // Add the new HuffmanNode back to the list
        nodes.push_back(branch);
    }

    HuffmanNode* root = nodes.front();
    return root;
}

bool HuffmanNode::isLeaf() {
    return left == nullptr && right == nullptr;
}

// Recursively assign binary patterns for unique leaf values
void assignBinaryPatternsRec(unordered_map<int, vector<int>>& patternAssignments, HuffmanNode* node, vector<int> pattern) {
    if (node->isLeaf()) {
        // If is a leaf, store binary pattern for value
        int value = node->value;
        patternAssignments[value] = pattern;
    } else {
        // Append 0 for left branch
        if (node->left != nullptr) {
            vector<int> leftPattern = pattern;
            leftPattern.push_back(0);
            assignBinaryPatternsRec(patternAssignments, node->left, leftPattern);
        }
        // Append 1 for right branch
        if (node->right != nullptr) {
            vector<int> rightPattern = pattern;
            rightPattern.push_back(1);
            assignBinaryPatternsRec(patternAssignments, node->right, rightPattern);
        }
    }
}

unordered_map<int, vector<int>> assignBinaryPatterns(HuffmanNode* root) {
    unordered_map<int, vector<int>> patternAssignments;
    vector<int> pattern;
    assignBinaryPatternsRec(patternAssignments, root, pattern);
    return patternAssignments;
}

// Get frequency counts of intensity values in RGB channels of image
vector<pair<int, int>> getFrequencies(Mat image) {
    int width = image.cols;
    int height = image.rows;

    // Iterate through each pixel to get flattened RGB (intensity) values
    vector<int> intensities;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Vec3b pixel = image.at<Vec3b>(y, x);
            intensities.push_back(pixel[0]); // Blue
            intensities.push_back(pixel[1]); // Green
            intensities.push_back(pixel[2]); // Red
        }
    }

    // Count how frequent each intensity value appears
    unordered_map<int, int> frequencyMap;
    for (int intensity : intensities) {
        frequencyMap[intensity]++;
    }
    vector<pair<int, int>> frequencies(frequencyMap.begin(), frequencyMap.end());
    return frequencies;
}

int binaryToInt(const vector<int> binaryPattern) {
    int result = 0;
    for (int bit : binaryPattern) {
        result = (result << 1) | bit; // Shift result left by 1 bit and add the current bit
    }
    return result;
}

vector<int> intToBinary(const int num) {
    if (num <= 1) {
        return {num}; // Base case: return a vector containing the last bit
    } else {
        vector<int> result = intToBinary(num >> 1); // Recursive call on num shifted right
        result.push_back(num & 1); // Append the least significant bit
        return result;
    }
}

vector<int> padBits(vector<int> binaryPattern, int finalLength) {
    int numAdditionalBits = finalLength - binaryPattern.size();
    binaryPattern.insert(binaryPattern.begin(), numAdditionalBits, 0);
    return binaryPattern;
}
