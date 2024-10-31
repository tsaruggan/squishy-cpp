//
//  huffman.h
//  Squishy
//
//  Created by Saruggan Thiruchelvan on 2024-10-30.
//

#ifndef HUFFMAN_H
#define HUFFMAN_H

using namespace std;
using namespace cv;

class HuffmanNode {
    public:
        int value;
        int count;
        HuffmanNode* left;
        HuffmanNode* right;

        HuffmanNode(int value, int count);
        HuffmanNode(int count);
        ~HuffmanNode();
};

HuffmanNode* buildTree(const vector<pair<int, int>>& frequencies);
unordered_map<int, vector<int>> assignBinaryPatterns(HuffmanNode* root);
vector<pair<int, int>> getFrequencies(Mat image);

int binaryToInt(const vector<int> binaryPattern);

#endif
