//
// Created by Göksu Güvendiren on 22/12/2016.
//

#ifndef PATCHMATCH_NNF_H
#define PATCHMATCH_NNF_H

#include <vector>

struct NN
{
    int x;
    int y;
    int distance;
};

class NNF
{
    int _width;
    int _height;

public:
    NNF() : _width(0), _height(0) {}
    NNF(int w, int h) : _width(w), _height(h) {
        field.resize(h);
        for (auto i = 0; i < h; i++) field[i].resize(w);
    }

    std::vector<std::vector<NN>> field;
};

#endif //PATCHMATCH_NNF_H
