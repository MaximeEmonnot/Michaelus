#pragma once
#include "Vec3D.h"

/*
 * Classe Quarternion
 * Définit une rotation dans l'espace
 */
template <typename T>
class Quaternion
{
public:
	// Constructeur par défaut
	constexpr Quaternion()
		:
		Quaternion(1, 0, 0, 0)
	{}

	// Constructeur définissant directement W X Y Z
	constexpr Quaternion(T w, T x, T y, T z)
		:
		w(w),
		x(x),
		y(y),
		z(z)
	{}

	// Constructeur à partir des 3 angles Roll (axe X) Pitch (axe Y) et Yaw (axe Z)
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

	// Constructeur à partir d'un axe de rotation et d'un angle
	constexpr Quaternion(const Vec3D<T>& axis, T angle)
	{
		const float halfAngle = 0.5f * angle;
		const float sin = MMath::Sin(halfAngle);
		const float cos = MMath::Cos(halfAngle);
		const Vec3D<T> normalizedAxis = axis.GetNormalized();

		w = cos;
		x = sin * normalizedAxis.GetX();
		y = sin * normalizedAxis.GetY();
		z = sin * normalizedAxis.GetZ();
	}

	// Constructeur de copie
	Quaternion& operator=(const Quaternion& rhs)
	{
		w = rhs.w;
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	// Conversion implicite 
	template<typename T2>
	Quaternion(Quaternion<T2> q)
		:
		Quaternion(static_cast<T>(q.w), static_cast<T>(q.x), static_cast<T>(q.y), static_cast<T>(q.z))
	{}

	// Opérateurs d'addition entre deux Quaternion
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

	// Opérateurs de multiplication entre deux Quaternion
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

	// Opérateurs de multiplication entre un Quaternion et un scalaire
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

	// Réalise la rotation d'un vecteur 3D
	FVec3D RotateVector(const FVec3D& v) const
	{
		const FVec3D q(x, y, z);
		const FVec3D t = FVec3D::CrossProduct(q, v) * 2.f;
		const FVec3D result = v + (t * w) + FVec3D::CrossProduct(q, t);
		
		return result;
	}

	// Réalise la rotation inverse d'un vecteur 3D
	FVec3D UnrotateVector(const FVec3D& v) const
	{
		const FVec3D q(-x, -y, -z);
		const FVec3D t = FVec3D::CrossProduct(q, v) * 2.f;
		const FVec3D result = v + (t * w) + FVec3D::CrossProduct(q, t);

		return result;
	}

	// Récupère le conjugué du Quaternion
	Quaternion GetConjugate() const
	{
		Quaternion out = *this;

		out.x *= -1;
		out.y *= -1;
		out.z *= -1;

		return out;
	}

	// Récupère la norme du Quaternion
	float GetNorm() const
	{
		return MMath::Sqrt(MMath::Square(w) + MMath::Square(x) + MMath::Square(y) + MMath::Square(z));
	}

	// Récupère le Quaternion unitaire
	Quaternion GetUnit() const
	{
		float norm = GetNorm();

		return Quaternion(w / norm, x / norm, y / norm, z / norm);
	}

	// Méthode ToString() pour l'affichage du Quaternion
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