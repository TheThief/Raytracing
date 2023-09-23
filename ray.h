#pragma once

#include <memory>

#include "common/vectorclass/vector3d.h"

struct material;

struct ray
{
    Vec3Dd at(double t) const
    {
        return origin + t * direction;
    }

    Vec3Dd origin;
    Vec3Dd direction;
    int remaining_depth = 100;
};

struct ray_intersection
{
    Vec3Dd location;
    Vec3Dd normal;
    Vec2d texcoord;
    std::shared_ptr<material> mat;
    ray r;
    double t;
};
