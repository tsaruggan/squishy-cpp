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

    int numBytes = (headerSize + treeSize + pixelsSize) / 8; // convert bits to bytes
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

    // Processing image data
    cout << "Processing image..." << endl;
    // Get frequency counts
    vector<pair<int, int>> frequencies = getFrequencies(image); 
    // Build tree
    HuffmanNode* root = buildTree(frequencies); 
    // Assign binary patterns
    unordered_map<int, vector<int>> patternAssignments = assignBinaryPatterns(root);

    // Calculate compressed size
    int sizeCompressed = compressedSize(frequencies, patternAssignments);
    cout << "Compressed size: " << sizeCompressed << " bytes" << endl;

    // Writing compressed binary
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
    cout << "Compressed " << sizeWrote << " bytes." << endl;
    double spaceSaving = round(100 * (1 - static_cast<double>(sizeWrote) / sizeRaw));
    cout << "Memory reduced by " << spaceSaving << "%." << endl;

    // Clean-up
    delete root;
    delete outputStream;
    cout << "Done." << endl;
}

void decompress(const string inputBinaryFileName, const string outputImageFileName) {
    cout << "Decompressing " << inputBinaryFileName << " -> " << outputImageFileName << endl;
    // Initialize decoding objects
    Input* inputStream = new Input(inputBinaryFileName);
    Decoder decoder = Decoder(inputStream);

    // Reading compressed binary
    cout << "Reading binary..." << endl;
    // Decode header
    pair<int, int> dimensions = decoder.decodeHeader();
    int width = dimensions.first;
    int height = dimensions.second;
    int sizeHeader = inputStream->bytesRead;
    cout << "* Header: " << sizeHeader << " bytes" << endl;

    // Decode Huffman table
    HuffmanNode* root = decoder.decodeTree();
    int sizeTree = inputStream->bytesRead - sizeHeader;
    cout << "* Tree: " << sizeTree << " bytes" << endl;

    // Decode image pixel data
    vector<Vec3b> pixels = decoder.decodePixels(width, height, root);
    int sizePixels = inputStream->bytesRead - sizeTree - sizeHeader;
    cout << "* Pixels: " << sizePixels << " bytes" << endl;

     // Result
    int sizeRead = inputStream->bytesRead;
    cout << "Decompressed " << sizeRead << " bytes." << endl;
    int sizeRaw = rawSize(width, height);
    double spaceExpand = round(100 * (static_cast<double>(sizeRaw) / sizeRead - 1));
    cout << "Memory expanded by " << spaceExpand << "%." << endl;
    cout << "Image dimensions: " << width << " x " << height << "px" << endl;
    cout << "Raw size: " << sizeRaw << " bytes" << endl;
    
    // Save image to file
    cout << "Generating image..." << endl;
    // Create image from pixels
    Mat image = Mat(height, width, CV_8UC3);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            image.at<Vec3b>(y, x) = pixels[y * width + x];
        }
    }
    imwrite(outputImageFileName, image);

    // Clean-up
    delete root;
    delete inputStream;
    cout << "Done." << endl;
}

int main(int argc, const char *argv[]) {
    if (argc < 4) {
        cout << "Error: Insufficient arguments." << endl;
    } else {
        string operation = argv[1];
        if (operation == "compress") {
            compress(argv[2], argv[3]);
            return 0;
        } else if (operation == "decompress") {
            decompress(argv[2], argv[3]);
            return 0;
        } else {
            cout << "Error: Invalid operation '" << operation << "'." << endl;
        }
    }

    // Display usage instructions if the argument count is too low or operation is invalid
    cout << "Usage:" << endl;
    cout << "  To squish:   ./squishy compress <inputImageFile.png> <outputBinaryFile.bin>" << endl;
    cout << "  To unsquish: ./squishy decompress <inputBinaryFile.bin> <outputImageFile.png>" << endl;
    return 1;
}

