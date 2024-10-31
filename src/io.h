//
//  io.h
//  Squishy
//
//  Created by Saruggan Thiruchelvan on 2024-10-30.
//

#ifndef IO_H
#define IO_H

using namespace std;

class Output {
    public:
        ofstream file;
        int buffer[8];

        Output(const string filename);
        ~Output();

        void writeBits(const vector<int> bits);
        void writeBit(const int bit);
        void flush();
    private:
        int currentIndex = 0;
        void saveByte();
};

#endif
