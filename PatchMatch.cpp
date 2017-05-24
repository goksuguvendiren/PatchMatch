//
// Created by Göksu Güvendiren on 21/12/2016.
//

#include "PatchMatch.h"
#include <opencv2/core.hpp>
#include <iostream>
#include <cassert>

constexpr int patch_w  = 7;
constexpr int pm_iters = 10;
constexpr int rs_max   = INT_MAX;

/* Measure distance between 2 patches with upper left corners (ax, ay) and (bx, by), terminating early if we exceed a cutoff distance.
   You could implement your own descriptor here. */
int Distance(const Bitmap& source, const Bitmap& dest, int ax, int ay, int bx, int by, int cutoff=INT_MAX) {
    int ans = 0;
    for (int dy = 0; dy < patch_w; dy++) {
        for (int dx = 0; dx < patch_w; dx++) {
//            The pixel is (ax + dx, ay + dy)
            auto ac = source.At(ay + dy, ax + dx);          // At(row, col)
            auto bc = dest.At(by + dy, bx + dx);
            int dred = ac[2] - bc[2];                         // Color space in OpenCV is BGR, not RGB!
            int dgre = ac[1] - bc[1];
            int dblu = ac[0] - bc[0];
            ans += dred*dred + dgre*dgre + dblu*dblu;
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


void PatchMatch(const Bitmap& source, const Bitmap& dest, NNF& ann)
{
    /* Initialize with random nearest neighbor field (NNF). */

    int aew = source.width()  - patch_w + 1;
    int aeh = source.height() - patch_w + 1;       /* Effective width and height (possible upper left corners of patches). */
    int bew = dest.width()    - patch_w + 1;
    int beh = dest.height()   - patch_w + 1;

    for (int i = 0; i < aeh; i++) {
        for (int j = 0; j < aew; j++) {                 // Randomize the corresponding patches
            int bx = rand() % bew;
            int by = rand() % beh;

            ann.field[i][j].x = bx;
            ann.field[i][j].y = by;
            ann.field[i][j].distance = Distance(source, dest, j, i, bx, by);

//            ann.At(i, j) = xy_to_int(bx, by);
//            annd.At(i, j) = Distance(source, dest, j, i, bx, by);
//            cout << annd.At(i, j) << endl;
//            cout << Distance(source, dest, j, i, bx, by) << endl;
        }
    }

    for (int iters = 0; iters < pm_iters; iters++) {
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
                auto xbest = nn.x;
                auto ybest = nn.y;
                auto dbest = nn.distance;

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
                    int xmin = std::max(xbest - mag, 0);
                    int xmax = std::min(xbest + mag + 1,bew);
                    int ymin = std::max(ybest - mag, 0);
                    int ymax = std::min(ybest + mag + 1,beh);
                    int xp = xmin + rand() % (xmax - xmin);
                    int yp = ymin + rand() % (ymax - ymin);
                    improve_guess(source, dest, ax, ay, nn, xp, yp);
                }

                ann.field[ay][ax] = nn;
//                ann.At(ay, ax) = xy_to_int(xbest, ybest);
//                annd.At(ay, ax) = dbest;
            }
        }
    }
}