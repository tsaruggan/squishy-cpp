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
void Encoder::encodeHeader(int width, int height) {
    vector<int> widthBits = padBits(intToBinary(width), 16);
    outputStream->writeBits(widthBits);

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
                int value = pixel[channel];
                vector<int> pattern = binaryPatternAssignments[value];
                outputStream->writeBits(pattern);
            }
        }
    }
}

Decoder::Decoder(Input* inputStream) {
    this->inputStream = inputStream;
}

pair<int, int> Decoder::decodeHeader() {
    vector<int> widthBits = inputStream->readBits(16);
    int width = binaryToInt(widthBits);

    vector<int> heightBits = inputStream->readBits(16);
    int height = binaryToInt(heightBits);

    inputStream->flush();

    return {width, height};
}

HuffmanNode* Decoder::decodeTreeRec() {
    int flag = inputStream->readBit();
    if (flag == 1) {
        vector<int> pattern = inputStream->readBits(8);
        int value = binaryToInt(pattern);
        HuffmanNode* node = new HuffmanNode(value, -1);
        return node;
    } else {
        HuffmanNode* left = decodeTreeRec();
        HuffmanNode* right = decodeTreeRec();
        HuffmanNode* node = new HuffmanNode(left, right);
        return node;
    }
}

HuffmanNode* Decoder::decodeTree() {
    HuffmanNode* root = decodeTreeRec();
    inputStream->flush();
    return root;
}

Mat Decoder::decodePixels(int width, int height, HuffmanNode* root) {
    // Construct pixels from decoded channel intensity values
    vector<Vec3b> pixels;
    for (int i = 0; i < width * height; i++) {
        Vec3b pixel;
        for (int channel = 0; channel < 3; channel++) {
            pixel[channel] = decodeValue(root);
        }
        pixels.push_back(pixel);
    }
    inputStream->flush();

    // Create image from pixels
    Mat image = Mat(height, width, CV_8UC3);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            image.at<Vec3b>(y, x) = pixels[y * width + x];
        }
    }
    return image.clone();
}

int Decoder::decodeValue(HuffmanNode* root) {
    int bit = inputStream->readBit();

    HuffmanNode* node;
    if (bit == 0) {
        node = root->left;
    } else if (bit == 1) {
        node = root->right;
    }

    if (node->isLeaf()) {
        return node->value;
    } else {
        return decodeValue(node);
    }
}