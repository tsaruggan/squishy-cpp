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

class HuffmanNode {
    public:
        int value;
        int count;
        HuffmanNode* left;
        HuffmanNode* right;

        HuffmanNode(int value, int count) {
            this->value = value;
            this->count = count;
        }

        HuffmanNode(int count) {
            this->count = count;
        }
}; 

// Build a Huffman tree from frequency list
HuffmanNode* buildTree(const vector<pair<int, int>>& frequencies) {
    // Create HuffmanNodes from frequencies
    vector<HuffmanNode*> nodes; 
    for (const pair<int, int>& frequency : frequencies) {
        int value = frequency.first;
        int count = frequency.second;
        HuffmanNode* node = new HuffmanNode(value, count);
        nodes.push_back(node);
    }

    // Build the tree
    while (nodes.size() > 1) {
        sort(nodes.begin(), nodes.end(),
            [](const HuffmanNode* node1, const HuffmanNode* node2){
                int count1 = node1->count;
                int count2 = node2->count;
                return count1 > count2;
            });

        // Create a new internal HuffmanNode with the two lowest frequencies
        HuffmanNode* left = nodes.back(); 
        nodes.pop_back();
        HuffmanNode* right = nodes.back(); 
        nodes.pop_back();

        HuffmanNode* branch = new HuffmanNode(left->count + right->count);
        branch->left = left;
        branch->right = right;

        // Add the new HuffmanNode back to the list
        nodes.push_back(branch);
    }

    HuffmanNode* root = nodes.front();
    return root;
}

// Clean-up tree
void deleteTree(HuffmanNode* root) {
    if (root) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
}

void printTree(HuffmanNode* root, const string& prefix = "") {
    if (root == nullptr) {
        return; // Base case: if the node is null, do nothing
    }

    // Check if the node is a leaf (both children are null)
    if (root->left == nullptr && root->right == nullptr) {
        // Print the current node's count (no value)
        cout << prefix << "[" << root->count << "]" << "{" << root->value << "}" << endl;
    } else {
        // Print the current node's count (for branches)
        cout << prefix << "(" << root->count << ")" << endl;
    }

    // Traverse the left and right children with an updated prefix
    printTree(root->left, prefix + "  ");
    printTree(root->right, prefix + "  ");
}

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
    // cout << "Frequencies: ";
    // for (const pair<int, int>& frequency : frequencies) {
    //     int intensity = frequency.first;
    //     int count = frequency.second;
    //     cout << intensity << " (" << count << ")" << ", ";
    // }
    // cout << endl;

    // Build tree
    HuffmanNode* root = buildTree(frequencies);
    // printTree(root);
    deleteTree(root);

    return 0;
}
