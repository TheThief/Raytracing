#pragma once

#include <optional>
#include "ray.h"

class traceable
{
public:
    virtual ~traceable() {}
    virtual std::optional<ray_intersection> ray_intersect(const ray& r) = 0;
};
