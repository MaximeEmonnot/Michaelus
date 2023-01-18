#pragma once

#include <functional>
#include <string>

#include "Math.h"

template <typename T>
class Vec3D {
public:
	class Hash
	{
	public:
		size_t operator()(const Vec3D& v) const {
			std::hash<T> hasher;
			return (hasher(v.x) << 42 | hasher(v.y) << 21 | hasher(v.z));
		}
	};

public:
	Vec3D() = default;
	constexpr Vec3D(T x, T	y, T z)
		:
		x(x),
		y(y),
		z(z)
	{}
	constexpr Vec3D(Vec3D pos0, Vec3D pos1)
		:
		x(pos1.x - pos0.x),
		y(pos1.y - pos0.y),
		z(pos1.z - pos0.z)
	{
	}
	Vec3D& operator= (const Vec3D& rhs) {
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	template <typename T2>
	Vec3D(Vec3D<T2> v)
		:
		Vec3D(static_cast<T>(v.x), static_cast<T>(v.y), static_cast<T>(v.z))
	{}


	Vec3D operator-() {
		x *= -1;
		y *= -1;
		z *= -1;
		return *this;
	}

	Vec3D operator+(const Vec3D& rhs) const {
		Vec3D out = *this;
		out.x += rhs.x;
		out.y += rhs.y;
		out.z += rhs.z;
		return out;
	}
	void operator+=(const Vec3D& rhs) {
		*this = *this + rhs;
	}
	Vec3D operator-(const Vec3D& rhs) const {
		Vec3D out = *this;
		out.x -= rhs.x;
		out.y -= rhs.y;
		out.z -= rhs.z;
		return out;
	}
	void operator-=(const Vec3D& rhs) {
		*this = *this - rhs;
	}
	Vec3D operator*(T scale) const {
		Vec3D out = *this;
		out.x *= scale;
		out.y *= scale;
		out.z *= scale;
		return out;
	}
	void operator*=(T scale) {
		*this = *this * scale;
	}
	Vec3D operator/(T scale) const {
		T invScale = 1 / scale;
		return *this * invScale;
	}
	void operator/=(T scale) {
		*this = *this / scale;
	}

	bool operator==(const Vec3D& rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	bool operator!=(const Vec3D& rhs) const {
		return !(*this == rhs);
	}

	float GetLength() const {
		return MMath::Sqrt(x * x + y * y + z * z);
	}

	Vec3D GetNormalized() const {
		Vec3D v = *this;
		if (GetLength() > 0) {
			float magnitude = 1 / GetLength();
			v.x *= magnitude;
			v.y *= magnitude;
			v.z *= magnitude;
			return v;
		}
		return Vec3D();
	}

	void Normalize() {
		*this = GetNormalized();
	}

	float Dot(const Vec3D& rhs) {
		Vec3D v0 = GetNormalized();
		Vec3D v1 = rhs.GetNormalized();

		return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
	}

	Vec3D Cross(const Vec3D& rhs)
	{
		Vec3D a = GetNormalized();
		Vec3D b = rhs.GetNormalized();

		Vec3D out;
		out.x = a.y * b.z - a.z * b.y;
		out.y = a.x * b.z - a.z * b.x;
		out.z = a.x * b.y - a.y * b.x;

		return out;
	}

	std::string ToString() const
	{
		return "X = " + std::to_string(x) + " Y = " + std::to_string(y) + " Z = " + std::to_string(z);
	}

	static float Distance(const Vec3D<T>& lhs, const Vec3D<T>& rhs)
	{
		Vec3D<T> v(lhs, rhs);
		return v.GetLength();
	}

public:

	T x = static_cast<T>(0);
	T y = static_cast<T>(0);
	T z = static_cast<T>(0);
};

typedef Vec3D<int> IVec3D;
typedef Vec3D<long long> LLVec3D;
typedef Vec3D<float> FVec3D;