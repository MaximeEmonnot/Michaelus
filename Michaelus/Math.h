#pragma once
#include <cmath>

#define PI (3.1415926535897932f)

struct MMath
{
	// Powers
	static inline float Square(float value) { return value * value; }
	static inline float Pow(float value, float power) { return powf(value, power); }
	static inline float Sqrt(float value) { return sqrtf(value); }

	// Trigonometry
	static inline float Cos(float value) { return cosf(value); }
	static inline float Sin(float value) { return sinf(value); }
	static inline float Tan(float value) { return tanf(value); }

	static inline float Rad(float degree) { return degree * PI / 180.f; }
	static inline float Deg(float radian) { return radian * 180.f / PI; }

	// Algebra
	static inline float Abs(float value) { return abs(value); }
	static inline float Sign(float value) { return value / Abs(value); }
};
