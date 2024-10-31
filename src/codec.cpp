//
//  codec.cpp
//  Squishy
//
//  Created by Saruggan Thiruchelvan on 2024-10-31.
//

#include <iostream>
#include <opencv2/opencv.hpp>

#include "codec.h"
#include "io.h"
#include "huffman.h"

using namespace std;
using namespace cv;

Encoder::Encoder(Output* outputStream) {
    this->outputStream = outputStream;
}

// Encode image width and height
void Encoder::encodeHeader(Mat image) {
    int width = image.cols;
    vector<int> widthBits = padBits(intToBinary(width), 16);
    outputStream->writeBits(widthBits);

    int height = image.rows;
    vector<int> heightBits = padBits(intToBinary(height), 16);
    outputStream->writeBits(heightBits);

    outputStream->flush();
}

// (Recursively) Encode binary to reconstruct tree
void Encoder::encodeTreeRec(HuffmanNode* node) {
    if (node->isLeaf()) {
        outputStream->writeBit(1); // If leaf write 1 followed by value pattern
        vector<int> pattern = padBits(intToBinary(node->value), 8);
        outputStream->writeBits(pattern);
    } else {
        outputStream->writeBit(0); // If branch write 0 and recurse
        if (node->left != nullptr) {
            encodeTreeRec(node->left);
        }
        if (node->right != nullptr) {
            encodeTreeRec(node->right);
        }
    }
}

void Encoder::encodeTree(HuffmanNode* root) {
    encodeTreeRec(root);
    outputStream->flush();
}

// Convert and store pixel channel intensity values 
void Encoder::encodePixels(Mat image, unordered_map<int, vector<int>> binaryPatternAssignments) {
    int width = image.cols;
    int height = image.rows;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Vec3b pixel = image.at<Vec3b>(y, x);
            for (int channel = 0; channel < 3; channel++) {
                vector<int> pattern = binaryPatternAssignments[channel];
                outputStream->writeBits(pattern);
            }
        }
    }
}