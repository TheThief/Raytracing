#pragma once

#include "colour.h"
#include "scalarmath.h"

constexpr uint8_t sRGB_limit_u8 = (uint8_t)10; // (0.04045 * 255) = 10.31475

fRGBA sRGB_to_linear(RGBA sRGB_colour)
{
	fRGBA colour(sRGB_colour.R/255.f, sRGB_colour.G/255.f, sRGB_colour.B/255.f, sRGB_colour.A/255.f);

	colour.R = sRGB_colour.R <= sRGB_limit_u8 ? colour.R / 12.92f : std::pow((colour.R + 0.055f) / 1.055f, 2.4f);
	colour.G = sRGB_colour.G <= sRGB_limit_u8 ? colour.G / 12.92f : std::pow((colour.G + 0.055f) / 1.055f, 2.4f);
	colour.B = sRGB_colour.B <= sRGB_limit_u8 ? colour.B / 12.92f : std::pow((colour.B + 0.055f) / 1.055f, 2.4f);

	return colour;
}

fRGBA sRGB_to_linear(BGRA sRGB_colour)
{
	fRGBA colour = (fRGBA)sRGB_colour;

	colour.R = sRGB_colour.R <= sRGB_limit_u8 ? colour.R / 12.92f : std::pow((colour.R + 0.055f) / 1.055f, 2.4f);
	colour.G = sRGB_colour.G <= sRGB_limit_u8 ? colour.G / 12.92f : std::pow((colour.G + 0.055f) / 1.055f, 2.4f);
	colour.B = sRGB_colour.B <= sRGB_limit_u8 ? colour.B / 12.92f : std::pow((colour.B + 0.055f) / 1.055f, 2.4f);

	return colour;
}

fRGBA sRGB_to_linear(fRGBA sRGB_colour)
{
	fRGBA colour = sRGB_colour;

	colour.R = sRGB_colour.R <= 0.04045f ? colour.R / 12.92f : std::pow((colour.R + 0.055f) / 1.055f, 2.4f);
	colour.G = sRGB_colour.G <= 0.04045f ? colour.G / 12.92f : std::pow((colour.G + 0.055f) / 1.055f, 2.4f);
	colour.B = sRGB_colour.B <= 0.04045f ? colour.B / 12.92f : std::pow((colour.B + 0.055f) / 1.055f, 2.4f);

	return colour;
}

fRGBA linear_to_sRGB(fRGBA linear_colour)
{
	fRGBA colour = Saturate(linear_colour);

	colour.R = colour.R <= 0.0031308f ? colour.R * 12.92f : std::pow(colour.R, 1.0f / 2.4f) * 1.055f - 0.055f;
	colour.G = colour.G <= 0.0031308f ? colour.G * 12.92f : std::pow(colour.G, 1.0f / 2.4f) * 1.055f - 0.055f;
	colour.B = colour.B <= 0.0031308f ? colour.B * 12.92f : std::pow(colour.B, 1.0f / 2.4f) * 1.055f - 0.055f;

	return colour;
}

template<typename colour_type_dest, typename colour_type_source>
colour_type_dest convert(colour_type_source source)
{
	if constexpr (std::is_same_v<colour_type_source, colour_type_dest>)
		return source;
	else if constexpr (std::is_same_v<colour_type_source, fRGBA>)
		return linear_to_sRGB(source);
	else if constexpr (std::is_same_v<colour_type_dest, fRGBA>)
		return sRGB_to_linear(source);
}
