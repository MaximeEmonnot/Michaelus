#pragma once
#include "Rotator.h"
#include "Vec3D.h"

template <typename T>
struct Transform
{
public:
	Transform operator+(const Transform& rhs) const
	{
		Transform out = *this;
		out.location += rhs.location;
		out.rotation += rhs.rotation;

		return out;
	}

	Vec3D<T> location;
	Rotator<T> rotation;
	Vec3D<T> Scale = { 1.f, 1.f, 1.f };
};

typedef Transform<float> FTransform;
typedef Transform<int> ITransform;