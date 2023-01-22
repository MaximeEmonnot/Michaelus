#pragma once
#include "Rotator.h"
#include "Vec3D.h"

template <typename T>
struct Transform
{
public:
	Vec3D<T> location;
	Rotator<T> rotation;
	Vec3D<T> Scale = { 1.f, 1.f, 1.f };
};

typedef Transform<float> FTransform;
typedef Transform<int> ITransform;