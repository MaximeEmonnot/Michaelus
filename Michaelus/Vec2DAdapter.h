#pragma once
#include "Vec2D.h"
#include "Vec3D.h"

/*
 * Classe Vec2DAdapter
 * Permet l'adapatation d'un vecteur 2D dans un espace 3D
 * On considère la composante Z du Vec2D comme étant nulle
 */
template <typename T>
class Vec2DAdapter<T> : public Vec3D<T>
{
public:
	// Constructeur adaptant le Vec2D
	Vec2DAdapter(const Vec2D<T>& vec2d)
		:
		vec2d(vec2d)
	{}

	// Opérateur de négation surchargé, pour utiliser directement le Vec2D
	Vec3D<T> operator-() override
	{
		Vec3D<T> out = Vec3D<T>(vec2d.GetX(), vec2d.GetY(), static_cast<T>(0));
		out.x *= -1;
		out.y *= -1;
		out.z *= -1;
		return *this;
	}

	// Opérateur d'addition entre deux Vec3D surchargé, pour utiliser directement le Vec2D
	Vec3D<T> operator+(const Vec3D<T>& rhs) const override {
		Vec3D out = Vec3D(vec2d.GetX(), vec2d.GetY(), static_cast<T>(0));
		out.x += rhs.x;
		out.y += rhs.y;
		out.z += rhs.z;
		return out;
	}

	// Opérateur de soustraction entre deux Vec3D surchargé, pour utiliser directement le Vec2D
	Vec3D<T> operator-(const Vec3D<T>& rhs) const override {
		Vec3D out = Vec3D(vec2d.GetX(), vec2d.GetY(), static_cast<T>(0));
		out.x -= rhs.x;
		out.y -= rhs.y;
		out.z -= rhs.z;
		return out;
	}

	// Opérateur de multiplication entre un Vec3D et un scalaire surchargé, pour utiliser directement le Vec2D
	Vec3D<T> operator*(T scale) const override {
		Vec3D out = Vec3D(vec2d.GetX(), vec2d.GetY(), static_cast<T>(0));
		out.x *= scale;
		out.y *= scale;
		out.z *= scale;
		return out;
	}
	// Opérateur d'égalité entre deux Vec3D surchargé, pour utiliser directement le Vec2D
	bool operator==(const Vec3D<T>& rhs) const override {
		return vec2d.GetX() == rhs.x && vec2d.GetY() == rhs.y && rhs.z == static_cast<T>(0);
	}

	// Récupération de la longeur du Vec2D dans l'espace 3D
	float GetLength() const override {
		return vec2d.GetLength();
	}

	// Récupération du vecteur 3D normalisé, défini par le Vec2D
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

	// Produit scalaire entre deux Vec3D surchargé, pour utiliser directement le Vec2D
	float Dot(const Vec3D<T>& rhs) const override {
		return vec2d.GetX() * rhs.x + vec2d.GetY() * rhs.y;
	}

	// Produit vectoriel entre deux Vec3D surchargé, pour utiliser directement le Vec2D
	Vec3D<T> Cross(const Vec3D<T>& rhs) const override {
		return Vec3D(
			vec2d.GetY() * rhs.z,
			 -vec2d.GetX() * rhs.z,
			vec2d.GetX() * rhs.y - vec2d.GetY() * rhs.x
		);
	}

	// Méthode ToString() pour l'affichage du Vec2D dans l'espace 3D
	std::string ToString() const override {
		return "X = " + std::to_string(vec2d.GetX()) + " Y = " + std::to_string(vec2d.GetY()) + " Z = 0";
	}

	// Récupération de la composante X du Vec2D
	T GetX() const override {
		return vec2d.GetX();
	}

	// Récupération de la composante Y du Vec2D
	T GetY() const override {
		return vec2d.GetY();
	}

	// Récupération de la composante Z nulle du Vec2D
	T GetZ() const override {
		return static_cast<T>(0);
	}

private:
	Vec2D<T> vec2d;
};