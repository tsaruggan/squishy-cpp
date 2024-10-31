//
//  huffman.h
//  Squishy
//
//  Created by Saruggan Thiruchelvan on 2024-10-30.
//

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <opencv2/opencv.hpp>

using namespace std;

class HuffmanNode {
    public:
        int value;
        int count;
        HuffmanNode* left;
        HuffmanNode* right;

        HuffmanNode(int value, int count);
        HuffmanNode(int count);
        ~HuffmanNode();

        bool isLeaf();
};

HuffmanNode* buildTree(const vector<pair<int, int>>& frequencies);
unordered_map<int, vector<int>> assignBinaryPatterns(HuffmanNode* root);
vector<pair<int, int>> getFrequencies(cv::Mat image);

int binaryToInt(const vector<int> binaryPattern);
vector<int> intToBinary(const int num);
vector<int> padBits(vector<int> binaryPattern, int finalLength);

#endif
