//
//  codec.h
//  Squishy
//
//  Created by Saruggan Thiruchelvan on 2024-10-31.
//

#ifndef CODEC_H
#define CODEC_H

#include "io.h"
#include "huffman.h"

using namespace std;
using namespace cv;

class Encoder {
    public:
        Encoder(Output* outputStream);

        void encodeHeader(int width, int height);
        void encodeTree(HuffmanNode* root);
        void encodePixels(Mat image, unordered_map<int, vector<int>> binaryPatternAssignments);

    private:
        Output* outputStream;
        void encodeTreeRec(HuffmanNode* node);
};

class Decoder {
    public:
        Decoder(Input* inputStream);

        pair<int, int> decodeHeader();
        HuffmanNode* decodeTree();
        vector<Vec3b> decodePixels(int width, int height, HuffmanNode* root);

    private:
        Input* inputStream;
        HuffmanNode* decodeTreeRec();
        int decodeValue(HuffmanNode* node);
};

#endif