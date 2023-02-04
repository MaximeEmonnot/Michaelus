#pragma once
#include "Vec3D.h"

template <typename T>
class Quaternion
{
public:
	constexpr Quaternion()
		:
		Quaternion(1, 0, 0, 0)
	{}

	constexpr Quaternion(T w, T x, T y, T z)
		:
		w(w),
		x(x),
		y(y),
		z(z)
	{}

	constexpr Quaternion(T roll, T pitch, T yaw)
	{
		const float cr = MMath::Cos(roll * 0.5f);
		const float sr = MMath::Sin(roll * 0.5f);
		const float cp = MMath::Cos(pitch * 0.5f);
		const float sp = MMath::Sin(pitch * 0.5f);
		const float cy = MMath::Cos(yaw * 0.5f);
		const float sy = MMath::Sin(yaw * 0.5f);

		w = cr * cp * cy + sr * sp * sy;
		x = sr * cp * cy - cr * sp * sy;
		y = cr * sp * cy + sr * cp * sy;
		z = cr * cp * sy - sr * sp * cy;
	}

	constexpr Quaternion(const Vec3D<T>& axis, T angle)
	{
		const float halfAngle = 0.5f * angle;
		const float sin = MMath::Sin(halfAngle);
		const float cos = MMath::Cos(halfAngle);
		const Vec3D<T> normalizedAxis = axis.GetNormalized();

		w = cos;
		x = sin * normalizedAxis.x;
		y = sin * normalizedAxis.y;
		z = sin * normalizedAxis.z;
	}

	Quaternion& operator=(const Quaternion& rhs)
	{
		w = rhs.w;
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	template<typename T2>
	Quaternion(Quaternion<T2> q)
		:
		Quaternion(static_cast<T>(q.w), static_cast<T>(q.x), static_cast<T>(q.y), static_cast<T>(q.z))
	{}

	Quaternion operator+(const Quaternion& rhs) const
	{
		Quaternion out = *this;
		out.w += rhs.w;
		out.x += rhs.x;
		out.y += rhs.y;
		out.z += rhs.z;

		return out;
	}
	void operator+=(const Quaternion& rhs)
	{
		*this = *this + rhs;
	}

	Quaternion operator*(const Quaternion& rhs) const
	{
		Quaternion out = *this;

		out.w = out.w * rhs.w - out.x * rhs.x - out.y * rhs.y - out.z * rhs.z;
		out.x = out.w * rhs.x + out.x * rhs.w + out.y * rhs.z - out.z * rhs.y;
		out.y = out.w * rhs.y - out.x * rhs.z + out.y * rhs.w + out.z * rhs.x;
		out.z = out.w * rhs.z + out.x * rhs.y - out.y * rhs.x + out.z * rhs.w;

		return out;
	}
	void operator *= (const Quaternion& rhs)
	{
		*this = *this * rhs;
	}

	Quaternion operator*(T scale) const
	{
		Quaternion out = *this;
		out.w *= scale;
		out.x *= scale;
		out.y *= scale;
		out.z *= scale;

		return out;
	}
	void operator*=(T scale)
	{
		*this = *this * scale;
	}

	FVec3D RotateVector(const FVec3D& v) const
	{
		const FVec3D q(x, y, z);
		
		return q * 2.f * FVec3D::DotProduct(q, v)
			+ q * (w * w - FVec3D::DotProduct(q, q))
			+ FVec3D::CrossProduct(q, v) * w * 2.f;
	}

	Quaternion GetConjugate() const
	{
		Quaternion out = *this;

		out.x *= -1;
		out.y *= -1;
		out.z *= -1;

		return out;
	}

	float GetNorm() const
	{
		return MMath::Sqrt(MMath::Square(w) + MMath::Square(x) + MMath::Square(y) + MMath::Square(z));
	}

	Quaternion GetUnit() const
	{
		float norm = GetNorm();

		return Quaternion(w / norm, x / norm, y / norm, z / norm);
	}

	std::string ToString() const
	{
		return "W = " + std::to_string(w) + " X = " + std::to_string(x) + " Y = " + std::to_string(y) + " Z = " + std::to_string(z);
	}

public:
	T w = static_cast<T>(1);
	T x = static_cast<T>(0);
	T y = static_cast<T>(0);
	T z = static_cast<T>(0);
};

typedef Quaternion<float> FQuaternion;
typedef Quaternion<int> IQuaternion;