//
// Created by Göksu Güvendiren on 21/12/2016.
//

#ifndef PATCHMATCH_PATCHMATCH_H
#define PATCHMATCH_PATCHMATCH_H

#include "Bitmap.h"
#include "NNF.h"

void PatchMatch(const Bitmap& source, const Bitmap& dest, NNF& ann);

constexpr int xy_to_int(int x, int y)
{
    return (y << 12) | x;
}

constexpr int vec_to_x(int vec)
{
    return vec & ((1 << 12) - 1);
}

constexpr int vec_to_y(int vec)
{
    return vec >> 12;
}

#endif //PATCHMATCH_PATCHMATCH_H
