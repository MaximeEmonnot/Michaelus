#pragma once
#include "Vec2D.h"
#include "Vec3D.h"

template <typename T>
class Vec2DAdapter<T> : public Vec3D<T>
{
public:
	Vec2DAdapter(const Vec2D<T>& vec2d)
		:
		vec2d(vec2d)
	{}

	Vec3D<T> operator-() override
	{
		Vec3D<T> out = Vec3D<T>(vec2d.GetX(), vec2d.GetY(), static_cast<T>(0));
		out.x *= -1;
		out.y *= -1;
		out.z *= -1;
		return *this;
	}
	Vec3D<T> operator+(const Vec3D<T>& rhs) const override {
		Vec3D out = Vec3D(vec2d.GetX(), vec2d.GetY(), static_cast<T>(0));
		out.x += rhs.x;
		out.y += rhs.y;
		out.z += rhs.z;
		return out;
	}
	Vec3D<T> operator-(const Vec3D<T>& rhs) const override {
		Vec3D out = Vec3D(vec2d.GetX(), vec2d.GetY(), static_cast<T>(0));
		out.x -= rhs.x;
		out.y -= rhs.y;
		out.z -= rhs.z;
		return out;
	}
	Vec3D<T> operator*(T scale) const override {
		Vec3D out = Vec3D(vec2d.GetX(), vec2d.GetY(), static_cast<T>(0));
		out.x *= scale;
		out.y *= scale;
		out.z *= scale;
		return out;
	}
	bool operator==(const Vec3D<T>& rhs) const override {
		return vec2d.GetX() == rhs.x && vec2d.GetY() == rhs.y && rhs.z == static_cast<T>(0);
	}
	float GetLength() const override {
		return vec2d.GetLength();
	}
	Vec3D<T> GetNormalized() const override {
		Vec3D v = Vec3D(vec2d.GetX(), vec2d.GetY(), static_cast<T>(0));
		if (GetLength() > 0) {
			float magnitude = 1 / GetLength();
			v.x *= magnitude;
			v.y *= magnitude;
			v.z *= magnitude;
			return v;
		}
		return Vec3D();
	}
	float Dot(const Vec3D<T>& rhs) const override {
		return vec2d.GetX() * rhs.x + vec2d.GetY() * rhs.y;
	}
	Vec3D<T> Cross(const Vec3D<T>& rhs) const override {
		return Vec3D(
			vec2d.GetY() * rhs.z,
			 -vec2d.GetX() * rhs.z,
			vec2d.GetX() * rhs.y - vec2d.GetY() * rhs.x
		);
	}
	std::string ToString() const override {
		return "X = " + std::to_string(vec2d.GetX()) + " Y = " + std::to_string(vec2d.GetY()) + " Z = 0";
	}
	T GetX() const override {
		return vec2d.GetX();
	}
	T GetY() const override {
		return vec2d.GetY();
	}
	T GetZ() const override {
		return static_cast<T>(0);
	}

private:
	Vec2D<T> vec2d;
};