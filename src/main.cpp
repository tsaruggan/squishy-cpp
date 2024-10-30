//
//  main.cpp
//  Squishy
//
//  Created by Saruggan Thiruchelvan on 2024-10-29.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, const char *argv[])
{
    string imagePath = "image.jpg";
    Mat image = imread(imagePath, IMREAD_COLOR);

    // Get dimensions
    int width = image.cols;
    int height = image.rows;
    cout << "Dimensions: " << width << "x" << height << "px" << endl;

    // Iterate through each pixel to get flattened RGB (intensity) values
    vector<int> intensities;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Vec3b pixel = image.at<Vec3b>(y, x);
            intensities.push_back(pixel[0]); // Blue
            intensities.push_back(pixel[1]); // Green
            intensities.push_back(pixel[2]); // Red
        }
    }

    // Count how frequent each intensity value appears
    unordered_map<int, int> frequencyMap;
    for (int intensity : intensities)
    {
        frequencyMap[intensity]++;
    }
    vector<pair<int, int>> frequencies(frequencyMap.begin(), frequencyMap.end());
    sort(frequencies.begin(), frequencies.end(),
         [](const pair<int, int> &a, const pair<int, int> &b){
             return a.second > b.second;
         });

    // Display count
    cout << "Frequencies: ";
    for (const pair<int, int> frequency : frequencies) {
        int intensity = frequency.first;
        int count = frequency.second;
        cout << intensity << " (" << count << ")" << endl;
    }

    return 0;
}
