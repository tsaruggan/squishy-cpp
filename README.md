# Squishy
Squishy uses a [Huffman coding](https://en.wikipedia.org/wiki/Huffman_coding) algorithm to compress (and decompress) image files into a binary format. The technique works by creating a binary tree of nodes based on the colour information of pixels and how frequently they appear in the image. A binary encoding is efficiently generated for each pixel so the image can be expressed using fewer bits. Information about how to restore the tree is also included in the header of the binary file so the image can be reconstructed without losing any detail.
* equivalent Python implementation: [tsaruggan/squishy](https://github.com/tsaruggan/squishy)
* Squishy speed comparison test: [tsaruggan/squishy-speed-comparison-test](https://github.com/tsaruggan/squishy-speed-comparison-test/blob/master/squishy_speed_comparison_test.png)


### Setup & Installation
Install C++ and [OpenCV](https://opencv.org), then run `make`

### Compress
```bash
./squishy compress <inputImageFile.png> <outputBinaryFile.bin>
```

### Decompress
```bash
./squishy decompress <inputBinaryFile.bin> <outputImageFile.png>
```
### Python Speed Comparison
![Graph comparison speeds of Squishy compression/decompression for both Python & C++ implementations.](https://raw.githubusercontent.com/tsaruggan/squishy-speed-comparison-test/refs/heads/master/squishy_speed_comparison_test.png)
