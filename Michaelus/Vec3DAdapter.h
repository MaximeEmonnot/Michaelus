#pragma once
#include "Vec2D.h"
#include "Vec3D.h"

/*
 * Classe Vec3DAdapter
 * Permet l'adapatation d'un vecteur 3D dans un espace 2D
 * On consid�re uniquement les composantes X et Y du Vec3D
 */
template <typename T>
class Vec3DAdapter<T> : public Vec2D<T>
{
public:
	// Constructeur adaptant le Vec3D
	Vec3DAdapter(const Vec3D<T>& vec3d)
		:
		vec3d(vec3d)
	{}

	// Op�rateur de n�gation surcharg�, pour utiliser directement le Vec3D
	Vec2D<T> operator-() override
	{
		Vec2D<T> out = Vec2D<T>(vec3d.GetX(), vec3d.GetY());
		out.x *= -1;
		out.y *= -1;
		return out;
	}

	// Op�rateur d'addition entre deux Vec2D surcharg�, pour utiliser directement le Vec3D
	Vec2D<T> operator+(const Vec2D<T>& rhs) const override
	{
		Vec2D out = Vec2D(vec3d.GetX(), vec3d.GetY());
		out.x += rhs.x;
		out.y += rhs.y;
		return out;
	}

	// Op�rateur de soustraction entre deux Vec2D surcharg�, pour utiliser directement le Vec3D
	Vec2D<T> operator-(const Vec2D<T>& rhs) const override
	{
		Vec2D out = Vec2D(vec3d.GetX(), vec3d.GetY());
		out.x -= rhs.x;
		out.y -= rhs.y;
		return out;
	}

	// Op�rateur de multiplication entre un Vec2D et un scalaire surcharg�, pour utiliser directement le Vec3D
	Vec2D<T> operator*(T scale) const override
	{
		Vec2D out = Vec2D(vec3d.GetX(), vec3d.GetY());
		out.x *= scale;
		out.y *= scale;
		return out;
	}

	// Op�rateur d'�galit� entre deux Vec2D surcharg�, pour utiliser directement le Vec3D
	bool operator==(const Vec2D<T>& rhs) const override
	{
		return vec3d.GetX() == rhs.x && vec3d.GetY() == rhs.y;
	}

	// R�cup�ration de la longeur du Vec3D dans l'espace 2D
	float GetLength() const override
	{
		return sqrtf(vec3d.GetX() * vec3d.GetX() + vec3d.GetY() * vec3d.GetY());
	}

	// R�cup�ration du vecteur 2D normalis�, d�fini par le Vec3D
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

	// M�thode ToString() pour l'affichage du Vec3D dans l'espace 2D
	std::string ToString() const override
	{
		return "X = " + std::to_string(vec3d.GetX()) + " Y = " + std::to_string(vec3d.GetY());
	}

	// R�cup�ration de la composante X du Vec3D
	T GetX() const override
	{
		return vec3d.GetX();
	}

	// R�cup�ration de la compostante Y du Vec3D
	T GetY() const override
	{
		return vec3d.GetY();
	}

private:
	Vec3D<T> vec3d;
};