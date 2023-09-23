#pragma once

#include <random>
#include "common/pcg/pcg_random.hpp"
#include "common/vectorclass/vector3d.h"

inline auto& rand_generator()
{
    static thread_local pcg64_fast generator;
    return generator;
}

inline double random_double(double min, double max)
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(rand_generator(), std::uniform_real_distribution<double>::param_type(min, max));
}

inline double random_double()
{
    return random_double(0.0, 1.0);
}

static Vec3Dd random3d(double min, double max)
{
    return Vec3Dd(random_double(min, max), random_double(min, max), random_double(min, max));
}

static Vec3Dd random3d()
{
    return random3d(0.0, 1.0);
}

inline double gaussian_double()
{
    static std::normal_distribution<double> distribution;
    return distribution(rand_generator());
}

static Vec3Dd gaussian_3d()
{
    return Vec3Dd(gaussian_double(), gaussian_double(), gaussian_double());
}

inline Vec3Dd random_unit_vector()
{
    do
    {
        auto p = gaussian_3d();
        [[likely]]
        if (p != Vec3Dd(0, 0, 0))
        {
            return normalize_vector(p);
        }
    } while (true);
}

inline Vec3Dd random_on_hemisphere(const Vec3Dd& normal)
{
    Vec3Dd on_unit_sphere = random_unit_vector();
    if (dot_product(on_unit_sphere, normal) > 0.0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}
