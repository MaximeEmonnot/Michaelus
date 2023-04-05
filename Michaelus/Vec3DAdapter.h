#pragma once
#include "Vec2D.h"
#include "Vec3D.h"

template <typename T>
class Vec3DAdapter<T> : public Vec2D<T>
{
public:
	Vec3DAdapter(const Vec3D<T>& vec3d)
		:
		vec3d(vec3d)
	{}

	Vec2D<T> operator-() override
	{
		Vec2D<T> out = Vec2D<T>(vec3d.GetX(), vec3d.GetY());
		out.x *= -1;
		out.y *= -1;
		return out;
	}
	Vec2D<T> operator+(const Vec2D<T>& rhs) const override
	{
		Vec2D out = Vec2D(vec3d.GetX(), vec3d.GetY());
		out.x += rhs.x;
		out.y += rhs.y;
		return out;
	}
	Vec2D<T> operator-(const Vec2D<T>& rhs) const override
	{
		Vec2D out = Vec2D(vec3d.GetX(), vec3d.GetY());
		out.x -= rhs.x;
		out.y -= rhs.y;
		return out;
	}
	Vec2D<T> operator*(T scale) const override
	{
		Vec2D out = Vec2D(vec3d.GetX(), vec3d.GetY());
		out.x *= scale;
		out.y *= scale;
		return out;
	}
	bool operator==(const Vec2D<T>& rhs) const override
	{
		return vec3d.GetX() == rhs.x && vec3d.GetY() == rhs.y;
	}
	float GetLength() const override
	{
		return sqrtf(vec3d.GetX() * vec3d.GetX() + vec3d.GetY() * vec3d.GetY());
	}
	Vec2D<T> GetNormalized() const override
	{
		Vec2D v = Vec2D(vec3d.GetX(), vec3d.GetY());
		if (GetLength() > 0) {
			float magnitude = 1 / GetLength();
			v.x *= magnitude;
			v.y *= magnitude;
			return v;
		}
		return Vec2D();
	}
	std::string ToString() const override
	{
		return "X = " + std::to_string(vec3d.GetX()) + " Y = " + std::to_string(vec3d.GetY());
	}
	T GetX() const override
	{
		return vec3d.GetX();
	}
	T GetY() const override
	{
		return vec3d.GetY();
	}

private:
	Vec3D<T> vec3d;
};