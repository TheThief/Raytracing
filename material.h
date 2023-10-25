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

struct basic_metal_material : material
{
	fRGBA diffuse_colour;

	basic_metal_material(fRGBA diffuse_colour)
		: diffuse_colour(std::move(diffuse_colour))
	{
	}

	virtual fRGBA sample(const scene& sc, const ray_intersection& ri) const;
};

struct basic_dialectric_material : material
{
	double ir = 1.5; // Index of Refraction

	basic_dialectric_material(double ir)
		: ir(ir)
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

inline Vec3Dd reflect(const Vec3Dd& v, const Vec3Dd& n)
{
	return v - 2 * dot_product(v, n) * n;
}

inline Vec3Dd refract(const Vec3Dd& v, const Vec3Dd& n, double ref_idx)
{
	Vec3Dd uv = normalize_vector(v);
	auto cos_theta = fmin(dot_product(-uv, n), 1.0);
	Vec3Dd r_out_perp = ref_idx * (uv + cos_theta * n);
	Vec3Dd r_out_parallel = -sqrt(fabs(1.0 - dot_product(r_out_perp, r_out_perp))) * n;
	return r_out_perp + r_out_parallel;
}

fRGBA basic_colour_material::sample(const scene& sc, const ray_intersection& ri) const
{
	//Vec3Dd R = random_on_hemisphere(ri.normal);
	Vec3Dd R = ri.normal + random_unit_vector();
	if (horizontal_and(is_zero_or_subnormal(R.to_vector())))
		R = ri.normal;

	ray r2 = ray::make_scatter_ray(ri, R);
	return diffuse_colour * sc.ray_colour(r2);
}

fRGBA basic_metal_material::sample(const scene& sc, const ray_intersection& ri) const
{
	Vec3Dd R = reflect(ri.r.direction, ri.normal);
	ray r2 = ray::make_scatter_ray(ri, R);
	return diffuse_colour * sc.ray_colour(r2);
}

inline double schlick_reflectance(double cosine, double ref_idx_1, double ref_idx_2)
{
	// Use Schlick's approximation for reflectance.
	auto r0 = (ref_idx_1 - ref_idx_2) / (ref_idx_1 + ref_idx_2);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

fRGBA basic_dialectric_material::sample(const scene& sc, const ray_intersection& ri) const
{
	const fRGBA diffuse_colour = { 1.0, 1.0, 1.0 };
	bool is_front_face = dot_product(ri.r.direction, ri.normal) < 0;
	double new_ref_idx = is_front_face ? ir : 1.0; // todo: ir stack?
	double refraction_ratio = ri.r.current_refractive_index / new_ref_idx;
	auto normal = is_front_face ? ri.normal : -ri.normal;

	Vec3Dd unit_direction = normalize_vector(ri.r.direction);
	double cos_theta = fmin(dot_product(-unit_direction, normal), 1.0);
	double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

	bool cannot_refract = refraction_ratio * sin_theta > 1.0;
	Vec3Dd R;

	if (cannot_refract || schlick_reflectance(cos_theta, ri.r.current_refractive_index, new_ref_idx) > random_double())
	{
		R = reflect(ri.r.direction, normal);
		new_ref_idx = ri.r.current_refractive_index;
	}
	else
	{
		R = refract(ri.r.direction, normal, refraction_ratio);
	}

	ray r2 = ray::make_scatter_ray(ri, R);
	r2.current_refractive_index = new_ref_idx;
	return diffuse_colour * sc.ray_colour(r2);
}

fRGBA basic_texture_material::sample(const scene& sc, const ray_intersection& ri) const
{
	auto C = tex->sample(ri.texcoord);
	Vec3Dd R = ri.normal + random_unit_vector();
	if (horizontal_and(is_zero_or_subnormal(R.to_vector())))
		R = ri.normal;

	ray r2 = ray::make_scatter_ray(ri, R);
	return C * sc.ray_colour(r2);
}

fRGBA basic_sky_texture_material::sample(const scene& sc, const ray_intersection& ri) const
{
	Vec3Dd unit_direction = normalize_vector(ri.r.direction);

	//float y = 0.5f * ((float)unit_direction.get_y() + 1.0f);
	//return Lerp(fRGBA(1.0f, 1.0f, 1.0f), fRGBA(0.5f, 0.7f, 1.0f), y);

	double yaw = atan2(unit_direction.get_x(), unit_direction.get_z()) * (std::numbers::inv_pi / 2) + 0.5;
	double pitch = asin(unit_direction.get_y()) * std::numbers::inv_pi * 2;
	Vec2d texcoord = { yaw, fmin(0.5 + pitch * 0.5, 0.5 - pitch * 0.5) };

	auto C = tex->sample(texcoord);
	return C;
}
