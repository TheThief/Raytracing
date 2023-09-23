#pragma once

#include "common/mdspan/mdarray"
#include "common/vectorclass/vector3d.h"
#include "common/math/colour.h"
#include "common/math/colour_transforms.h"

#define STBI_ONLY_PNG 1
#define STBI_ONLY_HDR 1
#include "common/stb/stb_image.h"

struct texture
{
	virtual ~texture() {}
	virtual fRGBA sample(Vec2d coords) const = 0;
};

template<typename colour_t>
class texture2d : public texture
{
	using view_t = std::experimental::mdspan<colour_t, std::experimental::extents<int, std::experimental::dynamic_extent, std::experimental::dynamic_extent>>;
	using channel_t = decltype(colour_t::R);

	void* data = nullptr;
	int size_x = 0;
	int size_y = 0;
	Vec2db wrap_xy = Vec2db(true, true);

public:
	texture2d() = default;

	texture2d(const char* filename, bool wrap_x, bool wrap_y)
		: wrap_xy(wrap_x, wrap_y)
	{
		int channels;

		if constexpr (std::is_same_v<channel_t, float>)
		{
			data = stbi_loadf(filename, &size_x, &size_y, &channels, colour_t::num_channels);
		}
		else if constexpr (std::is_same_v<channel_t, uint8_t>)
		{
			data = stbi_load(filename, &size_x, &size_y, &channels, colour_t::num_channels);
		}
	}

	texture2d(const texture2d&) = delete;
	texture2d(texture2d&& rhs)
		: data(rhs.data)
		, size_x(rhs.size_x)
		, size_y(rhs.size_y)
		, wrap_xy(rhs.wrap_xy)
	{
		rhs.data = nullptr;
		rhs.size_x = 0;
		rhs.size_y = 0;
		rhs.wrap_xy = Vec2db(true, true);
	}

	~texture2d()
	{
		stbi_image_free(data);
	}

	auto as_view() const
	{
		return view_t((colour_t*)data, size_y, size_x);
	}

	fRGBA sample(Vec2d coords) const
	{
		const Vec2d scaled_coords = select(wrap_xy,
			minimum(maximum(coords * Vec2d(size_x, size_y), Vec2d(0.0)), Vec2d(size_x - 1, size_y - 1)),
			fmodulo(coords, 1.0) * Vec2d(size_x, size_y));
		const Vec4i ic = truncate_to_int32(scaled_coords);
		const Vec4i ic2 = truncate_to_int32(ceil(scaled_coords));
		const Vec2d fc = fraction(scaled_coords);

		const auto view = as_view();
		return Lerp(
			Lerp(convert<fRGBA>(view(ic[1] , ic[0])), convert<fRGBA>(view(ic[1] , ic2[0])), (float)fc[0]),
			Lerp(convert<fRGBA>(view(ic2[1], ic[0])), convert<fRGBA>(view(ic2[1], ic2[0])), (float)fc[0]),
			(float)fc[1]);
	}
};
