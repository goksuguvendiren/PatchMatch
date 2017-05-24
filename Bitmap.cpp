//
// Created by Göksu Güvendiren on 21/12/2016.
//

#include "Bitmap.h"

#include <string>
#include <iostream>

#include <opencv2/imgcodecs.hpp>

Bitmap::Bitmap(string filename)
{
    _data = cv::imread(filename);   // type is 8U

    auto size = _data.size();
    _width = size.width;
    _height = size.height;
    _dimension = 3;
}

void Bitmap::Save(string filename)
{
    imwrite(filename, _data);
}

Bitmap::Bitmap(int w, int h, int dim) : _width(w),  _height(h), _dimension(dim)
{
    _data = cv::Mat(h, w, CV_32SC1, cvScalar(0));
}
