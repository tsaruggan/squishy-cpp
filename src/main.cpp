//
//  main.cpp
//  Squishy
//
//  Created by Saruggan Thiruchelvan on 2024-10-29.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, const char * argv[]) {
    std::string image_path = "image.jpg";
    Mat img = imread(image_path, IMREAD_COLOR);

    imshow("Display window", img);
    int k = waitKey(0); // Wait for a keystroke in the window
    
    return 0;
}
