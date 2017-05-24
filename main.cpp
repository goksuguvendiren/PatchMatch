#include <iostream>
#include <opencv2/core.hpp>
#include <opencv/cv.hpp>
#include <cassert>

#include "Bitmap.h"
#include "PatchMatch.h"
#include "NNF.h"

int main()
{
    std::string input1 = "/Users/goksu/Documents/PatchMatch/a.png";
    std::string input2 = "/Users/goksu/Documents/PatchMatch/b.png";

    const Bitmap bm1(input1);
    const Bitmap bm2(input2);

    std::cerr << "Beginning PatchMatch\n";
    NNF ann = PatchMatch(bm1, bm2);

    std::cerr << "Ended\n";
    cv::Mat image = cv::Mat(bm2.height(), bm2.width(), CV_8UC3);

    for (int i = 0; i < bm1.height(); i++){  // height
        for (int j = 0; j < bm1.width(); j++){  // width
            auto nn = ann.field[i][j];
//            std::cerr << nn.x << " " << nn.y << " " << nn.distance << " --- ";
            assert(nn.x >= 0);
            assert(nn.y >= 0);
            assert(nn.distance >= 0);

            image.at<cv::Vec3b>(nn.y, nn.x) = bm1.At(i, j);
        }
//        std::cerr << '\n';
    }

    cv::imshow("final", image);
    cv::waitKey(0);

    cv::imwrite("/Users/goksu/Documents/PatchMatch/image_computed.png", image);

    return 0;
}