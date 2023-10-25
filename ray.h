#pragma once

#include <memory>

#include "common/vectorclass/vector3d.h"

struct material;
struct ray_intersection;

struct ray
{
    Vec3Dd at(double t) const
    {
        return origin + t * direction;
    }

    Vec3Dd origin;
    Vec3Dd direction;
    int remaining_depth;
    double current_refractive_index = 1.0;

    static ray make_scatter_ray(const ray_intersection& ri, Vec3Dd direction);
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

ray ray::make_scatter_ray(const ray_intersection& ri, Vec3Dd direction)
{
    ray result = ri.r;
    result.origin = ri.location + 0.0001 * direction,
    result.direction = direction;
    result.remaining_depth--;
    return result;
}
