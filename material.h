#pragma once

#include "common/mdspan/mdarray"
#include "common/vectorclass/vector3d.h"
#include "common/math/colour.h"

#include "texture.h"

struct material
{
	virtual ~material() {}
	virtual fRGBA sample(const scene& sc, const ray_intersection& ri) const = 0;
};

struct debug_normal_material : material
{
	debug_normal_material()
	{
	}

	virtual fRGBA sample(const scene& sc, const ray_intersection& ri) const
	{
		return fRGBA((float)(ri.normal[0] * 0.5 + 0.5), (float)(ri.normal[1] * 0.5 + 0.5), (float)(ri.normal[2] * 0.5 + 0.5));
	}
};

struct basic_colour_material : material
{
	fRGBA diffuse_colour;

	basic_colour_material(fRGBA diffuse_colour)
		: diffuse_colour(std::move(diffuse_colour))
	{
	}

	virtual fRGBA sample(const scene& sc, const ray_intersection& ri) const;
};

struct basic_texture_material : material
{
	std::shared_ptr<texture> tex;

	basic_texture_material(std::shared_ptr<texture> tex)
		: tex(std::move(tex))
	{
	}

	virtual fRGBA sample(const scene& sc, const ray_intersection& ri) const;
};

struct basic_sky_texture_material : material
{
	std::shared_ptr<texture> tex;

	basic_sky_texture_material(std::shared_ptr<texture> tex)
		: tex(std::move(tex))
	{
	}

	virtual fRGBA sample(const scene& sc, const ray_intersection& ri) const;
};

#include "scene.h"

fRGBA basic_colour_material::sample(const scene& sc, const ray_intersection& ri) const
{
	//Vec3Dd R = ri.direction - 2 * dot_product(ri.direction, ri.normal) * ri.normal;
	//Vec3Dd R = random_on_hemisphere(ri.normal);
	Vec3Dd R = ri.normal + random_unit_vector();
	ray r2 =
	{
		.origin = ri.location + 0.001 * R,
		.direction = R,
		.remaining_depth = ri.r.remaining_depth - 1
	};
	return diffuse_colour * sc.ray_colour(r2);
}

fRGBA basic_texture_material::sample(const scene& sc, const ray_intersection& ri) const
{
	auto C = tex->sample(ri.texcoord);
	//Vec3Dd R = ri.direction - 2 * dot_product(ri.direction, ri.normal) * ri.normal;
	//Vec3Dd R = random_on_hemisphere(ri.normal);
	Vec3Dd R = ri.normal + random_unit_vector();
	ray r2 =
	{
		.origin = ri.location + 0.001 * R,
		.direction = R,
		.remaining_depth = ri.r.remaining_depth - 1
	};
	return C * sc.ray_colour(r2);
}

fRGBA basic_sky_texture_material::sample(const scene& sc, const ray_intersection& ri) const
{
	Vec3Dd unit_direction = normalize_vector(ri.r.direction);

	//float y = 0.5f * ((float)unit_direction.get_y() + 1.0f);
	//return Lerp(fRGBA(1.0f, 1.0f, 1.0f), fRGBA(0.5f, 0.7f, 1.0f), y);

	double yaw = atan2(unit_direction.get_z(), unit_direction.get_x()) * (std::numbers::inv_pi / 2) + 0.5;
	double pitch = asin(unit_direction.get_y()) * std::numbers::inv_pi * 2;
	Vec2d texcoord = { yaw, 0.5 - pitch * 0.5 };

	auto C = tex->sample(texcoord);
	return C;
}
