#pragma once

#include "common/vectorclass/vector3d.h"

struct camera
{
    Vec3Dd origin = Vec3Dd(0, 0, 0);
    double focal_length = 1.0;
};
