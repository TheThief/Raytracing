#pragma once

#include "common/math/colour.h"
#include "common/math/colour_transforms.h"
#include "common/math/random.h"
#include "common/mdspan/mdarray"

#define STBI_ONLY_PNG 1
#include "common/stb/stb_image_write.h"

#include "camera.h"
#include "scene.h"

#include <iostream>

struct renderer
{
public:
	int image_width = 100;  // Rendered image width in pixel count
	int image_height = 100; // Rendered image height
	int num_samples = 1;

	void render(const camera& cam, const scene& sc)
	{
		const double aspect_ratio = static_cast<double>(image_width) / image_height;

		// Determine viewport dimensions.
		constexpr auto viewport_height = 2.0;
		const auto viewport_width = viewport_height * aspect_ratio;

		// Calculate the vectors across the horizontal and down the vertical viewport edges.
		const auto viewport_u = Vec3Dd(viewport_width, 0, 0);
		const auto viewport_v = Vec3Dd(0, -viewport_height, 0);

		// Calculate the horizontal and vertical delta vectors from pixel to pixel.
		const auto pixel_delta_u = viewport_u / image_width;
		const auto pixel_delta_v = viewport_v / image_height;

		// Calculate the location of the upper left pixel.
		const auto viewport_upper_left =
			cam.origin + Vec3Dd(0, 0, cam.focal_length) - viewport_u / 2 - viewport_v / 2;

		std::experimental::mdarray<RGBA, std::experimental::dextents<int, 2>> image(image_height, image_width);

		// Render

		for (int y = image_height - 1; y >= 0; --y)
		{
			std::cout << "\rScanlines remaining: " << y << ' ' << std::flush;
			for (int x = 0; x < image_width; ++x)
			{
				fRGBA pixel_colour(0,0,0,0);
				for (int i = 0; i < num_samples; ++i)
				{
					auto pixel_center = viewport_upper_left + ((x + random_double()) * pixel_delta_u) + ((y + random_double()) * pixel_delta_v);
					auto ray_direction = pixel_center - cam.origin;
					ray r(cam.origin, ray_direction);
					pixel_colour += ray_colour(sc, r);
				}
				pixel_colour /= (float)num_samples;

				image(y, x) = RGBA(linear_to_sRGB(pixel_colour));
			}
		}

		stbi_write_png("output.png", image_width, image_height, 4, image.data(), image.stride(0) * 4);
	}

private:

	fRGBA ray_colour(const scene& sc, const ray& r)
	{
		auto t = sc.ray_intersect(r);
		return *t.and_then([&](const ray_intersection& ri) -> std::optional<fRGBA>
			{
				Vec3Dd N = ri.normal;
				fRGBA C = ri.mat->sample(ri);
				Vec3Dd R = r.direction - 2 * dot_product(r.direction, N) * N;
				return C * ray_colour(sc, { ri.location + 0.001 * R, R });
			}).or_else([&]() -> std::optional<fRGBA>
			{
				Vec3Dd unit_direction = normalize_vector(r.direction);

				//float y = 0.5f * ((float)unit_direction.get_y() + 1.0f);
				//return Lerp(fRGBA(1.0f, 1.0f, 1.0f), fRGBA(0.5f, 0.7f, 1.0f), y);

				double yaw = atan2(unit_direction.get_z(), unit_direction.get_x()) * (std::numbers::inv_pi / 2) + 0.5;
				double pitch = asin(unit_direction.get_y()) * std::numbers::inv_pi * 2;

				auto sample = sc.sky_material->sample({ .texcoord = {yaw, 0.5 - pitch * 0.5} });
				return sample;
			});
	}
};
