#include <iostream>
#include <numbers>

#include "renderer.h"
#include "scene.h"
#include "sphere.h"
#include "texture.h"
#include "material.h"

std::shared_ptr debug_normal_mat = std::make_shared<debug_normal_material>();
std::shared_ptr grey_mat = std::make_shared<basic_colour_material>(fRGBA(0.9f, 0.9f, 0.9f));
std::shared_ptr sky_material = std::make_shared<basic_sky_texture_material>(std::make_shared<texture2d<fRGBA>>("probe_10-00_latlongmap.hdr", false, true));
std::shared_ptr s = std::make_shared<sphere>(sphere({ 0,0.5,1 }, 0.5, grey_mat));
std::shared_ptr g = std::make_shared<sphere>(sphere({0, -100, 1}, 100, grey_mat));
scene sc = {.objects = {s, g}, .sky_material = sky_material };

int main()
{
	camera cam = { Vec3Dd(0, 0.5, 0), 1.0 };

	renderer render;
	render.image_width = 1280;
	render.image_height = 720;
	render.num_samples = 10000;

	render.render(cam, sc);

	std::cerr << "\nDone.\n";
}
