#include <iostream>
#include <numbers>

#include "renderer.h"
#include "scene.h"
#include "sphere.h"
#include "texture.h"
#include "material.h"

std::shared_ptr sky_material = std::make_shared<basic_sky_texture_material>(std::make_shared<texture2d<fRGBA>>("probe_10-00_latlongmap.hdr", false, true));

auto material_ground = std::make_shared<basic_colour_material>(fRGBA(0.8f, 0.8f, 0.0f));
auto material_center = std::make_shared<basic_colour_material>(fRGBA(0.1f, 0.2f, 0.5f));
auto material_left   = std::make_shared<basic_dialectric_material>(1.5);
auto material_right  = std::make_shared<basic_metal_material>(fRGBA(0.8f, 0.6f, 0.2f));

std::shared_ptr ground = std::make_shared<sphere>(Vec3Dd{ 0.0, -100, 1.0}, 100, material_ground);
std::shared_ptr center = std::make_shared<sphere>(Vec3Dd{ 0.0,  0.5, 1.0}, 0.5, material_center);
std::shared_ptr left   = std::make_shared<sphere>(Vec3Dd{-1.0,  0.5, 1.0}, 0.5, material_left);
std::shared_ptr left2  = std::make_shared<sphere>(Vec3Dd{-1.0,  0.5, 1.0}, -0.4, material_left);
std::shared_ptr right  = std::make_shared<sphere>(Vec3Dd{ 1.0,  0.5, 1.0}, 0.5, material_right);

scene sc = {.objects = {ground, center, left, left2, right}, .sky_material = sky_material };

int main()
{
	camera cam = { Vec3Dd(0, 0.5, 0), 1.0 };

	renderer render;
	render.image_width = 1280;
	render.image_height = 720;
#if NDEBUG
	render.num_samples = 10000;
#else
	render.num_samples = 1;
#endif
	render.recursion_depth = 100;

	render.render(cam, sc);

	std::cerr << "\nDone.\n";
}
