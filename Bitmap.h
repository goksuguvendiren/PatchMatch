//
// Created by Göksu Güvendiren on 21/12/2016.
//


#ifndef PATCHMATCH_BITMAP_H
#define PATCHMATCH_BITMAP_H

#include <vector>
#include <string>
#include <opencv2/core.hpp>

using namespace std;

class Bitmap
{
    int _width, _height;
    int _dimension;
    cv::Mat _data;
public:
    Bitmap(string filename);   //_data.resize(_width * _height); }// = new int[_width*_height];
    Bitmap(int w, int h, int dim);

    //_data.resize(_width * _height); }// = new int[_width*_height];
    void Save(string filename);
    const cv::Mat& Row(int i) const { return _data.row(i); }

    cv::Vec3b At(int i, int j) const { return _data.at<cv::Vec3b>(i, j); }
    int& At(int i, int j) { return _data.at<int>(i, j); }

    int width() const { return _width; }
    int height() const { return _height; }

//    vector<int> operator[](int y) { return _data[y*_width]; }
};

#endif //PATCHMATCH_BITMAP_H
