#pragma once

#include <vector>
#include <memory>
#include "traceable.h"
#include "material.h"

class scene
{
public:
    std::optional<ray_intersection> ray_intersect(const ray& r) const
    {
        std::optional<ray_intersection> result;
        for (auto&& object : objects)
        {
            auto temp = object->ray_intersect(r);
            if (temp.has_value() && (!result.has_value() || temp->t < result->t))
                result = temp;
        }
        return result;
    }

public:
    std::vector<std::shared_ptr<traceable>> objects;
    std::shared_ptr<material> sky_material;
};
