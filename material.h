#pragma once

#include "common/mdspan/mdarray"
#include "common/vectorclass/vector3d.h"
#include "common/math/colour.h"

#include "texture.h"

struct material
{
	virtual ~material() {}
	virtual fRGBA sample(const ray_intersection& ri) const = 0;
};

struct debug_normal_material : material
{
	debug_normal_material()
	{
	}

	virtual fRGBA sample(const ray_intersection& ri) const
	{
		return fRGBA((float)(ri.normal[0] * 0.5 + 0.5), (float)(ri.normal[1] * 0.5 + 0.5), (float)(ri.normal[2] * 0.5 + 0.5));
	}
};

struct basic_texture_material : material
{
	std::shared_ptr<texture> tex;

	basic_texture_material(std::shared_ptr<texture> tex)
		: tex(std::move(tex))
	{
	}

	virtual fRGBA sample(const ray_intersection& ri) const
	{
		return tex->sample(ri.texcoord);
	}
};
