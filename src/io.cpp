//
//  io.cpp
//  Squishy
//
//  Created by Saruggan Thiruchelvan on 2024-10-30.
//

#include <fstream>

#include "io.h"
#include "huffman.h"

using namespace std;

Output::Output(const string fileName) {
    file = new ofstream(fileName, ios::binary);
    bytesWritten = 0;
}

Output::~Output() {
    flush();
    file->close();
    delete file;
}

// Add bits to buffer; when buffer contains 8 bits, save byte
void Output::writeBits(const vector<int> bits) {
    for (const int bit : bits) {
        buffer[currentIndex++] = bit;

        if (currentIndex == 8) {
            saveByte();
            currentIndex = 0;
        }
    }
}

// Write a single bit
void Output::writeBit(const int bit) {
    vector<int> bits = {bit};
    writeBits(bits);
}

// Add trailing zeros to complete a byte and write it
void Output::flush() {
    if (currentIndex > 0) {
        for (int i = currentIndex; i < 8; i++) {
            buffer[i] = 0;
        }
        saveByte();
        currentIndex = 0;
    }
}

// Save byte from buffer to output file
void Output::saveByte() {
    vector<int> bufferVector(buffer, buffer + 8);
    int byte = binaryToInt(bufferVector);
    char byteChar = static_cast<char>(byte); // Interpret integer as byte
    file->put(byteChar);  // Write the byte to the file
    bytesWritten++;
}