//
// Created by Göksu Güvendiren on 21/12/2016.
//

#include "Bitmap.h"

#include <string>
#include <iostream>

#include <opencv2/imgcodecs.hpp>


Bitmap::Bitmap(std::string filename)
{
    _data = cv::imread(filename);   // type is 8U

    auto size = _data.size();
    _size = Size(size.width, size.height);
    _dimension = 3;
}

void Bitmap::Save(std::string filename)
{
    imwrite(filename, _data);
}

Bitmap::Bitmap(int w, int h, int dim) : _size(w, h), _dimension(dim)
{
    _data = cv::Mat(h, w, CV_32SC1, cvScalar(0));
}
