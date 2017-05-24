//
// Created by Göksu Güvendiren on 21/12/2016.
//


#ifndef PATCHMATCH_BITMAP_H
#define PATCHMATCH_BITMAP_H

#include <vector>
#include <string>
#include <opencv2/core.hpp>

struct Size
{
    int width;
    int height;

    Size(int w, int h) : width(w), height(h) {}
    Size() : width(0), height(0) {}
};

class Bitmap
{
    Size _size;
    int _dimension;
    cv::Mat _data;

public:
    Bitmap(std::string filename);
    Bitmap(int w, int h, int dim);

    void Save(std::string filename);
//    const cv::Mat& Row(int i) const { return _data.row(i); }

    cv::Vec3b At(int i, int j) const { return _data.at<cv::Vec3b>(i, j); }
    int& At(int i, int j) { return _data.at<int>(i, j); }

    int width() const { return _size.width; }
    int height() const { return _size.height; }

    Size size() const { return _size; }
};

#endif //PATCHMATCH_BITMAP_H
