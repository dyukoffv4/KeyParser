#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

namespace Paint {
    using std::pow;

#pragma pack(push, 1)

    struct BitmapFileHeader {
        unsigned short signature;
        unsigned int filesize;
        unsigned short reserved1;
        unsigned short reserved2;
        unsigned int pixelArrOffset;
    };

    struct BitmapInfoHeader {
        unsigned int headerSize;
        unsigned int width;
        unsigned int height;
        unsigned short planes;
        unsigned short bitsPerPixel;
        unsigned int compression;
        unsigned int imageSize;
        unsigned int xPixelsPerMeter;
        unsigned int yPixelsPerMeter;
        unsigned int colorsInColorTable;
        unsigned int importantColorCount;
    };

    struct BGR {
        unsigned char b;
        unsigned char g;
        unsigned char r;
    };

#pragma pack(pop)

    class Image {
    public:
        BitmapFileHeader BFH;
        BitmapInfoHeader BIH;
        BGR **BM;

        Image();
        Image(const Image& image);
        ~Image();

        Image& operator=(const Image& image);

        bool load(std::string path);
        bool save(std::string path);
    };

    std::vector<Image> slice_image(const Image& image, int x_lines, int y_lines);

    Image set_component(const Image& image, char index, unsigned char value);

    Image put_circle(const Image& image, int x, int y, int radius, BGR f_color, int border, BGR b_color);

    Image put_square(const Image& image, int x1, int y1, int x2, int y2, int x_d, int y_d);

    std::ostream &operator<<(std::ostream &out, const BitmapFileHeader &bfh);

    std::ostream &operator<<(std::ostream &out, const BitmapInfoHeader &bih);

    std::ostream &operator<<(std::ostream &out, const BGR &bgr);
}