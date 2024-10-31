//
//  io.h
//  Squishy
//
//  Created by Saruggan Thiruchelvan on 2024-10-30.
//

#ifndef IO_H
#define IO_H

#include <fstream>

using namespace std;

class Output {
    public:
        Output(const string filename);
        ~Output();

        int bytesWritten;

        void writeBits(const vector<int> bits);
        void writeBit(const int bit);
        void flush();
        
    private:
        ofstream* file;
        int buffer[8];
        int currentIndex = 0;
        void saveByte();
};

#endif
