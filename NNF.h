//
// Created by Göksu Güvendiren on 22/12/2016.
//

#ifndef PATCHMATCH_NNF_H
#define PATCHMATCH_NNF_H

#include <vector>
#include "Bitmap.h"

struct NN
{
    int x;
    int y;
    int distance;

    NN() : x(0), y(0), distance(std::numeric_limits<int>::max()) {}
    NN(int _x, int _y, int _d) : x(_x), y(_y), distance(_d) {}
};

class NNF
{
    const Size size;

public:
    NNF() : size(0, 0) {}
    NNF(int w, int h) : size(w, h) {
        field.resize(h);
        for (auto i = 0; i < h; i++) field[i].resize(w);
    }
    NNF(const Size& sz) : size(sz) {
        field.resize(size.height);
        for (auto i = 0; i < size.height; i++)
        {
            field[i].resize(size.width);
        }
    }

    std::vector<std::vector<NN>> field;
};

#endif //PATCHMATCH_NNF_H
