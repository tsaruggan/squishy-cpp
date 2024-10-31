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

        void encodeHeader(Mat image);
        void encodeTree(HuffmanNode* root);
        void encodePixels(Mat image, unordered_map<int, vector<int>> binaryPatternAssignments);

    private:
        Output* outputStream;
        void encodeTreeRec(HuffmanNode* node);
};

#endif