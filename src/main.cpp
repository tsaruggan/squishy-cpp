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

int rawSize(int width, int height) {
    int headerSize = 2 * 16; // height and width as 16 bit values
    int pixelsSize = 3 * 8 * width * height;  // 3 channels, 8 bits per channel
    int numBytes = (headerSize + pixelsSize) / 8; // convert bits to bytes
    return numBytes;
}

int compressedSize(vector<pair<int, int>> frequencies, unordered_map<int, vector<int>>  binaryPatternAssignments) {
    int headerSize = 2 * 16; // height and width as 16 bit values

    // Calculate tree size
    int treeSize = frequencies.size() * (1 + 8); // Leafs: 1 bit flag + 8 bit symbol each
    treeSize += frequencies.size() - 1; // Branches: 1 bit flag each
    if (treeSize % 8 > 0) { // padding to next full byte
        treeSize += 8 - (treeSize % 8);
    } 
        
    // Calculate pixels size
    int pixelsSize = 0;
    for (const pair<int, int>& frequency : frequencies) {
        int value = frequency.first;
        int count = frequency.second;
        pixelsSize += count * binaryPatternAssignments[value].size();
    }
    if (pixelsSize % 8 > 0) { // padding to next full byte
        pixelsSize += 8 - (pixelsSize % 8);
    } 

    int numBytes = (headerSize + pixelsSize) / 8; // convert bits to bytes
    return numBytes;
}

void compress(const string inputImageFileName, const string outputBinaryFileName) {
    cout << "Compressing " << inputImageFileName << " -> " << outputBinaryFileName << endl;

    // Load image from file
    Mat image = imread(inputImageFileName, IMREAD_COLOR);

    // Get dimensions
    int width = image.cols;
    int height = image.rows;
    cout << "Image dimensions: " << width << " x " << height << "px" << endl;

    // Calculate raw size
    int sizeRaw = rawSize(width, height);
    cout << "Raw size: " << sizeRaw << " bytes" << endl;

    //// Processing image data
    cout << "Processing image..." << endl;
    // Get frequency counts
    vector<pair<int, int>> frequencies = getFrequencies(image); 
    // Build tree
    HuffmanNode* root = buildTree(frequencies); 
    // Assign binary patterns
    unordered_map<int, vector<int>> patternAssignments = assignBinaryPatterns(root);

    // Calculate compressed size
    int sizeCompressed = compressedSize(frequencies, patternAssignments);
    cout << "Estimated compressed size: " << sizeCompressed << " bytes" << endl;

    //// Writing compressed binary
    cout << "Writing binary..." << endl;
    // Initialize encoding objects
    Output* outputStream = new Output(outputBinaryFileName);
    Encoder encoder = Encoder(outputStream);

    // Encode header
    encoder.encodeHeader(width, height);
    int sizeHeader = outputStream->bytesWritten;
    cout << "* Header: " << sizeHeader << " bytes" << endl;

    // Encode Huffman table
    encoder.encodeTree(root);
    int sizeTree = outputStream->bytesWritten - sizeHeader;
    cout << "* Tree: " << sizeTree << " bytes" << endl;

    // Encode image pixel data
    encoder.encodePixels(image, patternAssignments);
    int sizePixels = outputStream->bytesWritten - sizeTree - sizeHeader;
    cout << "* Pixels: " << sizePixels << " bytes" << endl;

    // Results
    int sizeWrote = outputStream->bytesWritten;
    cout << "Compressed to " << sizeWrote << " bytes." << endl;
    double spaceSaving = round(100 * (1 - static_cast<double>(sizeWrote) / sizeRaw));
    cout << "Memory reduced by " << spaceSaving << "%." << endl;

    // Clean-up
    delete root;
    delete outputStream;
}

void decompress(const string inputBinaryFileName, const string outputImageFileName) {
    cout << "Decompressing " << inputBinaryFileName << " -> " << outputImageFileName << endl;

    Input* inputStream = new Input(inputBinaryFileName);
    Decoder decoder = Decoder(inputStream);

    pair<int, int> dimensions = decoder.decodeHeader();
    int width = dimensions.first;
    int height = dimensions.second;
    int sizeHeader = inputStream->bytesRead;
    cout << "* Header: " << sizeHeader << " bytes" << endl;

    HuffmanNode* root = decoder.decodeTree();
    int sizeTree = inputStream->bytesRead - sizeHeader;
    cout << "* Tree: " << sizeTree << " bytes" << endl;

    Mat image = decoder.decodePixels(width, height, root);
    int sizePixels = inputStream->bytesRead - sizeTree - sizeHeader;
    cout << "* Pixels: " << sizePixels << " bytes" << endl;

    imwrite(outputImageFileName, image);

    delete root;
    delete inputStream;
}

int main(int argc, const char *argv[]) {
    compress("./samples/octopus.png","./samples/output.bin");
    decompress("./samples/output.bin", "./samples/output.png");
    
    return 0;
}