#pragma once
#include "Quaternion.h"
#include "Vec3D.h"

/*
 * Classe Transform
 * D�finit une position (Vec3D), une rotation (Quaternion) et une taille (Vec3D)
 */
template <typename T>
struct Transform
{
public:
	// Op�rateur d'addition entre deux Transform
	Transform operator+(const Transform& rhs) const
	{
		Transform out = *this;
		out.location += rhs.location;
		out.rotation *= rhs.rotation;
		return out;
	}

	Vec3D<T> location;
	Quaternion<T> rotation;
	Vec3D<T> Scale = { 1.f, 1.f, 1.f };
};

typedef Transform<float> FTransform;
typedef Transform<int> ITransform;