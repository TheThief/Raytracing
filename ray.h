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
};

struct ray_intersection
{
    Vec3Dd location;
    Vec3Dd direction;
    Vec3Dd normal;
    Vec2d texcoord;
    double t;
    std::shared_ptr<material> mat;
};
