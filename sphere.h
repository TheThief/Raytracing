#pragma once

#include "traceable.h"

class sphere : public traceable
{
public:
    std::optional<ray_intersection> ray_intersect(const ray& r)
    {
        Vec3Dd oc = r.origin - center;
        auto a = dot_product(r.direction, r.direction); // 1 if normalised ray
        auto b_2 = dot_product(oc, r.direction); // half of b
        auto c = dot_product(oc, oc) - radius * radius;
        auto d_4 = b_2 * b_2 - a * c; // 1/4 of the discriminant

        if (d_4 < 0)
        {
            return std::nullopt;
        }

        // (-b - sqrt(d)) / 2a
        // but we cancel the 2 by using half b and sqrt(quarter d) aka half sqrt(d)
        double t = (-b_2 - sqrt(d_4)) / a;

        // Find the nearest root that lies in the acceptable range.
        double t_min = 0, t_max = std::numeric_limits<double>::infinity();
        if (t < t_min || t > t_max)
        {
            t = (-b_2 + sqrt(d_4)) / a;
            if (t < t_min || t > t_max)
            {
                return std::nullopt;
            }
        }

        Vec3Dd location = r.at(t);
        Vec3Dd normal = (location - center) / radius;
        return ray_intersection{
            .location = location,
            .normal = normal,
            .texcoord = Vec2d(normal[0] + 1, normal[1] + 1) * 0.5,
            .mat = mat,
            .r = r,
            .t = t,
        };
    }

public:
    Vec3Dd center;
    double radius;
    std::shared_ptr<material> mat;

    sphere(const Vec3Dd& center, double radius, const std::shared_ptr<material>& mat)
        : center(center), radius(radius), mat(mat)
    {
    }
};
