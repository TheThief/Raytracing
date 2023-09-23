#pragma once

#include "mathcommon.h"
#include "scalarmath.h"
#include <cstdint>

#ifdef RGB
#undef RGB
#endif

struct RGB
{
	static const int num_channels = 3;

	uint8_t R;
	uint8_t G;
	uint8_t B;

	RGB() :
		R(255),
		G(0),
		B(255)
	{
	}

	RGB(uint8_t R, uint8_t G, uint8_t B) :
		R(B),
		G(G),
		B(R)
	{
	}

	RGB operator +(const RGB& rhs) const
	{
		return RGB(R + rhs.R, G + rhs.G, B + rhs.B);
	}

	RGB operator -(const RGB& rhs) const
	{
		return RGB(R - rhs.R, G - rhs.G, B - rhs.B);
	}

	template<typename type>
	RGB operator *(const type& rhs) const
	{
		return RGB((uint8_t)(R * rhs), (uint8_t)(G * rhs), (uint8_t)(B * rhs));
	}

	template<typename type>
	const RGB& operator *=(const type& rhs)
	{
		R = (uint8_t)(R * rhs);
		G = (uint8_t)(G * rhs);
		B = (uint8_t)(B * rhs);
		return *this;
	}

	friend RGB Lerp(const RGB& X, const RGB& Y, float Alpha)
	{
		RGB Result;
		Result.R = Lerp(X.R, Y.R, Alpha);
		Result.G = Lerp(X.G, Y.G, Alpha);
		Result.B = Lerp(X.B, Y.B, Alpha);
		return Result;
	}
};

struct RGBA
{
	static const int num_channels = 4;

	union
	{
		struct
		{
			uint8_t R;
			uint8_t G;
			uint8_t B;
			uint8_t A;
		};
		unsigned int dwColour;
	};

	RGBA() :
		R(255),
		G(0),
		B(255),
		A(255)
	{
	}

	RGBA(uint8_t R, uint8_t G, uint8_t B, uint8_t A = 255) :
		R(R),
		G(G),
		B(B),
		A(A)
	{
	}

	RGBA(unsigned int dw) :
		dwColour(dw)
	{
	}

	RGBA(const RGB& RGB, uint8_t A = 255) :
		R(RGB.R),
		G(RGB.G),
		B(RGB.B),
		A(A)
	{
	}

	RGBA& operator=(const RGBA& rhs) = default;

	friend RGBA Lerp(const RGBA& X, const RGBA& Y, float Alpha)
	{
		RGBA Result;
		Result.R = Lerp(X.R, Y.R, Alpha);
		Result.G = Lerp(X.G, Y.G, Alpha);
		Result.B = Lerp(X.B, Y.B, Alpha);
		Result.A = Lerp(X.A, Y.A, Alpha);
		return Result;
	}

	explicit operator unsigned int()
	{
		return dwColour;
	}
};

struct BGR
{
	static const int num_channels = 3;

	uint8_t B;
	uint8_t G;
	uint8_t R;

	BGR() :
		B(255),
		G(0),
		R(255)
	{
	}

	BGR(uint8_t R, uint8_t G, uint8_t B) :
		B(B),
		G(G),
		R(R)
	{
	}

	BGR operator +(const BGR& rhs) const
	{
		return BGR(R + rhs.R, G + rhs.G, B + rhs.B);
	}

	BGR operator -(const BGR& rhs) const
	{
		return BGR(R - rhs.R, G - rhs.G, B - rhs.B);
	}

	template<typename type>
	BGR operator *(const type& rhs) const
	{
		return BGR((uint8_t)(R * rhs), (uint8_t)(G * rhs), (uint8_t)(B * rhs));
	}

	template<typename type>
	const BGR& operator *=(const type& rhs)
	{
		R = (uint8_t)(R * rhs);
		G = (uint8_t)(G * rhs);
		B = (uint8_t)(B * rhs);
		return *this;
	}

	friend BGR Lerp(const BGR& X, const BGR& Y, float Alpha)
	{
		BGR Result;
		Result.R = Lerp(X.R, Y.R, Alpha);
		Result.G = Lerp(X.G, Y.G, Alpha);
		Result.B = Lerp(X.B, Y.B, Alpha);
		return Result;
	}
};

struct BGRA
{
	static const int num_channels = 4;

	union
	{
		struct 
		{
			uint8_t B;
			uint8_t G;
			uint8_t R;
			uint8_t A;
		};
		unsigned int dwColour;
	};

	BGRA() :
		B(255),
		G(0),
		R(255),
		A(255)
	{
	}

	BGRA(uint8_t R, uint8_t G, uint8_t B, uint8_t A = 255) :
		B(B),
		G(G),
		R(R),
		A(A)
	{
	}

	BGRA(unsigned int dw) :
		dwColour(dw)
	{
	}

	BGRA(const RGB& RGB, uint8_t A = 255) :
		B(RGB.B),
		G(RGB.G),
		R(RGB.R),
		A(A)
	{
	}

	BGRA& operator=(const BGRA& rhs) = default;
	//{
	//	dwColour = rhs.dwColour;
	//	return *this;
	//}

	friend BGRA Lerp(const BGRA& X, const BGRA& Y, float Alpha)
	{
		BGRA Result;
		Result.R = Lerp(X.R, Y.R, Alpha);
		Result.G = Lerp(X.G, Y.G, Alpha);
		Result.B = Lerp(X.B, Y.B, Alpha);
		Result.A = Lerp(X.A, Y.A, Alpha);
		return Result;
	}

	explicit operator unsigned int()
	{
		return dwColour;
	}
};

struct alignstruct(16) fRGBA
{
	static const int num_channels = 4;

	union
	{
		struct
		{
			float R;
			float G;
			float B;
			float A;
		};
#if SSE_VERSION >= 2
		__m128 mm;
#endif
	};

	fRGBA() :
		R(1),
		G(0),
		B(1),
		A(1)
	{
	}

	fRGBA(float r, float g, float b, float a = 1) :
		R(r),
		G(g),
		B(b),
		A(a)
	{
	}

	explicit fRGBA(BGRA bgra) :
#if SSE_VERSION >= 2
		mm(_mm_div_ps(_mm_cvtepi32_ps(_mm_unpacklo_epi16(_mm_unpacklo_epi8(_mm_cvtsi32_si128((unsigned int)bgra), _mm_setzero_si128()), _mm_setzero_si128())), _mm_set1_ps(255.0f)))
#else
		R(bgra.R / 255.0f),
		G(bgra.G / 255.0f),
		B(bgra.B / 255.0f),
		A(bgra.A / 255.0f)
#endif
	{
#if SSE_VERSION >= 2
		mm = _mm_shuffle_ps(mm, mm, _MM_SHUFFLE(3,0,1,2));
#endif
	}

#if SSE_VERSION >= 2
	fRGBA(__m128 _mm) :
		mm(_mm)
	{
	}
#endif

	explicit operator RGBA() const
	{
#if SSE_VERSION >= 2
		// using _mm_cvtps_epi32 which implements round-to-nearest-even for exact X.5 - D3D uses trunc(X + 0.5) so will always round X.5 up but this should be faster
		// https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-data-conversion#integer-conversion
		__m128 temp = mm;
		return RGBA(_mm_cvtsi128_si32(_mm_packus_epi16(_mm_packs_epi32(_mm_cvtps_epi32(_mm_mul_ps(temp, _mm_set1_ps(255.0f))), _mm_setzero_si128()), _mm_setzero_si128())));
#else
		const fRGBA Temp = Saturate(*this) * 255;
		return BGRA((uint8_t)(Temp.R + 0.5f), (uint8_t)(Temp.G + 0.5f), (uint8_t)(Temp.B + 0.5f), (uint8_t)(Temp.A + 0.5f));
#endif
	}

	explicit operator RGB() const
	{
		const RGBA Temp = (RGBA)*this;
		return RGB(Temp.R, Temp.G, Temp.B);
	}

	explicit operator BGRA() const
	{
#if SSE_VERSION >= 2
		// using _mm_cvtps_epi32 which implements round-to-nearest-even for exact X.5 - D3D uses trunc(X + 0.5) so will always round X.5 up but this should be faster
		// https://docs.microsoft.com/en-us/windows/win32/direct3d10/d3d10-graphics-programming-guide-resources-data-conversion#integer-conversion
#if SSE_VERSION >= 2
		__m128 temp = _mm_shuffle_ps(mm, mm, _MM_SHUFFLE(3, 0, 1, 2));
#endif
		return BGRA(_mm_cvtsi128_si32(_mm_packus_epi16(_mm_packs_epi32(_mm_cvtps_epi32(_mm_mul_ps(temp,_mm_set1_ps(255.0f))), _mm_setzero_si128()), _mm_setzero_si128())));
#else
		const fRGBA Temp = Saturate(*this) * 255;
		return BGRA((uint8_t)(Temp.R + 0.5f), (uint8_t)(Temp.G + 0.5f), (uint8_t)(Temp.B + 0.5f), (uint8_t)(Temp.A + 0.5f));
#endif
	}

	explicit operator BGR() const
	{
		const BGRA Temp = (BGRA)*this;
		return BGR(Temp.R, Temp.G, Temp.B);
	}

	const fRGBA& operator +=(const fRGBA& rhs)
	{
#if SSE_VERSION >= 2
		mm = _mm_add_ps(mm, rhs.mm);
#else
		R += rhs;
		G += rhs;
		B += rhs;
		A += rhs;
#endif
		return *this;
	}

	fRGBA operator +(const fRGBA& rhs) const
	{
#if SSE_VERSION >= 2
		return fRGBA(_mm_add_ps(mm, rhs.mm));
#else
		return fRGBA(R + rhs.R, G + rhs.G, B + rhs.B, A + rhs.A);
#endif
	}

	const fRGBA& operator -=(const fRGBA& rhs)
	{
#if SSE_VERSION >= 2
		mm = _mm_sub_ps(mm, rhs.mm);
#else
		R -= rhs;
		G -= rhs;
		B -= rhs;
		A -= rhs;
#endif
		return *this;
	}

	fRGBA operator -(const fRGBA& rhs) const
	{
#if SSE_VERSION >= 2
		return fRGBA(_mm_sub_ps(mm, rhs.mm));
#else
		return fRGBA(R - rhs.R, G - rhs.G, B - rhs.B, A - rhs.A);
#endif
	}

	fRGBA operator *(const fRGBA& rhs) const
	{
#if SSE_VERSION >= 2
		return fRGBA(_mm_mul_ps(mm, rhs.mm));
#else
		return fRGBA(R * rhs.R, G * rhs.G, B * rhs.B, A * rhs.A);
#endif
	}

	const fRGBA& operator *=(const fRGBA& rhs)
	{
#if SSE_VERSION >= 2
		mm = _mm_mul_ps(mm, rhs.mm);
#else
		R *= rhs;
		G *= rhs;
		B *= rhs;
		A *= rhs;
#endif
		return *this;
	}

	//template<typename type>
	//fARGB operator *(const type& rhs) const
	//{
	//	return fARGB((float)(R * rhs), (float)(G * rhs), (float)(B * rhs), (float)(A * rhs));
	//}

	//template<typename type>
	//const fARGB& operator *=(const type& rhs)
	//{
	//	R = (float)(R * rhs);
	//	G = (float)(G * rhs);
	//	B = (float)(B * rhs);
	//	A = (float)(A * rhs);
	//	return *this;
	//}

	fRGBA operator *(float rhs) const
	{
#if SSE_VERSION >= 2
		return fRGBA(_mm_mul_ps(mm, _mm_set1_ps(rhs)));
#else
		return fRGBA(R * rhs, G * rhs, B * rhs, A * rhs);
#endif
	}

	const fRGBA& operator *=(float rhs)
	{
#if SSE_VERSION >= 2
		mm = _mm_mul_ps(mm, _mm_set1_ps(rhs));
#else
		R *= rhs;
		G *= rhs;
		B *= rhs;
		A *= rhs;
#endif
		return *this;
	}

	fRGBA operator /(float rhs) const
	{
#if SSE_VERSION >= 2
		return fRGBA(_mm_div_ps(mm, _mm_set1_ps(rhs)));
#else
		return fRGBA(R / rhs, G / rhs, B / rhs, A / rhs);
#endif
	}

	const fRGBA& operator /=(float rhs)
	{
#if SSE_VERSION >= 2
		mm = _mm_div_ps(mm, _mm_set1_ps(rhs));
#else
		R /= rhs;
		G /= rhs;
		B /= rhs;
		A /= rhs;
#endif
		return *this;
	}

	friend fRGBA Lerp(const fRGBA& X, const fRGBA& Y, float Alpha)
	{
		fRGBA Result;
		Result.R = Lerp(X.R, Y.R, Alpha);
		Result.G = Lerp(X.G, Y.G, Alpha);
		Result.B = Lerp(X.B, Y.B, Alpha);
		Result.A = Lerp(X.A, Y.A, Alpha);
		return Result;
	}

	friend fRGBA Saturate(const fRGBA& X)
	{
		fRGBA Result;
#if SSE_VERSION >= 2
		Result.mm = _mm_max_ps(_mm_min_ps(X.mm, _mm_set1_ps(1.0f)), _mm_setzero_ps());
#else
		Result.R = Saturate(X.R);
		Result.G = Saturate(X.G);
		Result.B = Saturate(X.B);
		Result.A = Saturate(X.A);
#endif
		return Result;
	}
};
