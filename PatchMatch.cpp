//
// Created by Göksu Güvendiren on 21/12/2016.
//

#include "PatchMatch.h"
#include <iostream>
#include <cassert>
#include <opencv/cv.hpp>

constexpr int patch_w  = 7;
constexpr int pm_iters = 10;
constexpr int rs_max   = INT_MAX;

/* Measure distance between 2 patches with upper left corners (ax, ay) and (bx, by), terminating early if we exceed a cutoff distance.
   You could implement your own descriptor here. */
int Distance(const Bitmap& source, const Bitmap& dest, int ax, int ay, int bx, int by, int cutoff = std::numeric_limits<int>::max())
{
    int ans = 0;
    for (int dy = 0; dy < patch_w; dy++) {
        for (int dx = 0; dx < patch_w; dx++) {
            auto ac = source.At(ay + dy, ax + dx);          // At(row, col)
            auto bc = dest.At(by + dy, bx + dx);
            int dred = ac[2] - bc[2];                         // Color space in OpenCV is BGR, not RGB!
            int dgre = ac[1] - bc[1];
            int dblu = ac[0] - bc[0];

            ans += dred * dred + dgre * dgre + dblu * dblu;  // set distance as the square distance of pixel colors
        }
        if (ans >= cutoff) { return cutoff; }
    }
    return ans;
}

void improve_guess(const Bitmap& source, const Bitmap& dest, int ax, int ay, NN& nnBest, int bx, int by) 
{
    int d = Distance(source, dest, ax, ay, bx, by, nnBest.distance);

    assert(d >= 0);

    if (d < nnBest.distance) {
        nnBest.distance = d;
        nnBest.x = bx;
        nnBest.y = by;
    }
}

NNF Random_NNF(const Bitmap& source, const Bitmap& dest)
{
    NNF ann(source.size());

    int aew = source.size().width  - patch_w + 1;
    int aeh = source.size().height - patch_w + 1;       /* Effective width and height (possible upper left corners of patches). */
    int bew = dest.size().width    - patch_w + 1;
    int beh = dest.size().height   - patch_w + 1;

    for (int i = 0; i < aeh; i++) {
        for (int j = 0; j < aew; j++) {                 // Randomize the corresponding patches
            int bx = rand() % bew;
            int by = rand() % beh;

            ann.field[i][j].x = bx;
            ann.field[i][j].y = by;
            ann.field[i][j].distance = Distance(source, dest, j, i, bx, by, ann.field[i][j].distance);  // fast return if distance is greater than the already calculated distance.
        }
    }

    return ann;
}

NNF PatchMatch(const Bitmap& source, const Bitmap& dest)
{
    /* Initialize with random nearest neighbor field (NNF). */

    int aew = source.width()  - patch_w + 1;
    int aeh = source.height() - patch_w + 1;       /* Effective width and height (possible upper left corners of patches). */
    int bew = dest.width()    - patch_w + 1;
    int beh = dest.height()   - patch_w + 1;

    NNF ann = Random_NNF(source, dest);

    for (int iters = 0; iters < pm_iters; iters++)
    {
        std::cerr << iters << '\n';
        /* In each iteration, improve the NNF, by looping in scanline or reverse-scanline order. */
        int ystart = 0, yend = aeh, ychange = 1;
        int xstart = 0, xend = aew, xchange = 1;

        if (iters % 2 == 1) {           // reverse-scanline
            xstart = xend - 1;
            xend = -1;
            xchange = -1;
            ystart = yend - 1;
            yend = -1;
            ychange = -1;
        }

        for (int ay = ystart; ay != yend; ay += ychange) {
            for (int ax = xstart; ax != xend; ax += xchange) {
                /* Current (best) guess. */
                auto& nn = ann.field[ay][ax];

                /* Propagation: Improve current guess by trying instead correspondences from left and above (below and right on odd iterations). */
                if ((unsigned) (ax - xchange) < (unsigned) aew) 
                {
                    auto nnp = ann.field[ay][ax - xchange];
                    auto xp = nnp.x;
                    auto yp = nnp.y;

                    if ((unsigned) xp < (unsigned) bew) {
                        improve_guess(source, dest, ax, ay, nn, xp, yp);   // updates xbest, ybest and dbest
                    }
                }

                if ((unsigned) (ay - ychange) < (unsigned) aeh) {
                    auto nnp = ann.field[ay - ychange][ax];
                    auto xp = nnp.x;
                    auto yp = nnp.y;

                    if ((unsigned) yp < (unsigned) beh) {
                        improve_guess(source, dest, ax, ay, nn, xp, yp);
                    }
                }

                /* Random search: Improve current guess by searching in boxes of exponentially decreasing size around the current best guess. */
                int rs_start = rs_max;
                if (rs_start > std::max(dest.width(), dest.height())) {
                    rs_start = std::max(dest.width(), dest.height());
                }

                for (int mag = rs_start; mag >= 1; mag /= 2) {
                    /* Sampling window */
                    int xmin = std::max(nn.x - mag, 0);
                    int xmax = std::min(nn.x + mag + 1,bew);
                    int ymin = std::max(nn.y - mag, 0);
                    int ymax = std::min(nn.y + mag + 1,beh);

                    int xp = xmin + rand() % (xmax - xmin);
                    int yp = ymin + rand() % (ymax - ymin);
                    improve_guess(source, dest, ax, ay, nn, xp, yp);
                }

                ann.field[ay][ax] = nn;
            }
        }

        const auto& s = ann;

        cv::Mat image = cv::Mat(source.height(), source.width(), CV_8UC3);
        for (int i = 0; i < source.height(); i++){  // height
            for (int j = 0; j < source.width(); j++){  // width
                auto nn = s.field[i][j];
                assert(nn.x >= 0);
                assert(nn.y >= 0);
                assert(nn.distance >= 0);

                image.at<cv::Vec3b>(nn.y, nn.x) = source.At(i, j);
            }
        }

        cv::imshow("asd", image);
        cv::waitKey(0);
    }

    return ann;
}