#pragma once

#include <vector>
#include <memory>
#include "traceable.h"

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

	fRGBA ray_colour(const ray& r) const;

public:
	std::vector<std::shared_ptr<traceable>> objects;
	std::shared_ptr<material> sky_material;
};

#include "material.h"

fRGBA scene::ray_colour(const ray& r) const
{
	if (r.remaining_depth == 0)
		return fRGBA(0, 0, 0);

	auto t = ray_intersect(r);
	return *t.and_then([&, &sc=*this](const ray_intersection& ri) -> std::optional<fRGBA>
		{
			Vec3Dd N = ri.normal;
			fRGBA C = ri.mat->sample(sc, ri);
			return C;
		}).or_else([&, &sc = *this]() -> std::optional<fRGBA>
		{
			auto sample = sky_material->sample(sc, { .r = r });
			return sample;
		});
}
