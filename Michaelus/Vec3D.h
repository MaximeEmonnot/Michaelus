#pragma once

#include <functional>
#include <string>

#include "Math.h"

/*
 * Classe Vec3D
 * Définit un vecteur dans un espace 3D
 */
template <typename T>
class Vec3D {
public:
	// Classe de hashage pour l'utilisation de la classe dans des std::unordered_map
	class Hash
	{
	public:
		size_t operator()(const Vec3D& v) const {
			std::hash<T> hasher;
			return (hasher(v.x) << 42 | hasher(v.y) << 21 | hasher(v.z));
		}
	};

public:
	// Constructeur par défaut
	Vec3D() = default;

	// Constructeur définissant directement X Y Z
	constexpr Vec3D(T x, T	y, T z)
		:
		x(x),
		y(y),
		z(z)
	{}

	// Constructeur définissant le vecteur 3D du point A au point B
	constexpr Vec3D(Vec3D pos0, Vec3D pos1)
		:
		x(pos1.x - pos0.x),
		y(pos1.y - pos0.y),
		z(pos1.z - pos0.z)
	{}

	// Constructeur de copie
	Vec3D& operator= (const Vec3D& rhs) {
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	// Conversion implicite
	template <typename T2>
	Vec3D(Vec3D<T2> v)
		:
		Vec3D(static_cast<T>(v.x), static_cast<T>(v.y), static_cast<T>(v.z))
	{}

	// Opérateur de négation
	virtual Vec3D operator-() {
		x *= -1;
		y *= -1;
		z *= -1;
		return *this;
	}

	// Opérateurs d'addition entre deux Vec3D
	virtual Vec3D operator+(const Vec3D& rhs) const {
		Vec3D out = *this;
		out.x += rhs.x;
		out.y += rhs.y;
		out.z += rhs.z;
		return out;
	}
	void operator+=(const Vec3D& rhs) {
		*this = *this + rhs;
	}

	// Opérateurs de soustraction entre deux Vec3D
	virtual Vec3D operator-(const Vec3D& rhs) const {
		Vec3D out = *this;
		out.x -= rhs.x;
		out.y -= rhs.y;
		out.z -= rhs.z;
		return out;
	}
	void operator-=(const Vec3D& rhs) {
		*this = *this - rhs;
	}

	// Opérateurs de multiplication entre un Vec3D et un scalaire
	virtual Vec3D operator*(T scale) const {
		Vec3D out = *this;
		out.x *= scale;
		out.y *= scale;
		out.z *= scale;
		return out;
	}
	void operator*=(T scale) {
		*this = *this * scale;
	}

	// Opérateurs de division entre un Vec3D et un scalaire
	Vec3D operator/(T scale) const {
		T invScale = 1 / scale;
		return *this * invScale;
	}
	void operator/=(T scale) {
		*this = *this / scale;
	}

	// Opérateur d'égalité entre deux Vec3D
	virtual bool operator==(const Vec3D& rhs) const {
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	// Opérateur d'inégalité entre deux Vec3D
	bool operator!=(const Vec3D& rhs) const {
		return !(*this == rhs);
	}

	// Récupération de la longueur du Vec3D
	virtual float GetLength() const {
		return MMath::Sqrt(x * x + y * y + z * z);
	}

	// Récupération du vecteur 3D normalisé
	virtual Vec3D GetNormalized() const {
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

	// Normalisation du Vec3D
	void Normalize() {
		*this = GetNormalized();
	}

	// Produit scalaire entre deux Vec3D
	virtual float Dot(const Vec3D& rhs) const{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}

	// Produit vectoriel entre deux Vec3D
	virtual Vec3D Cross(const Vec3D& rhs) const
	{
		return Vec3D(
			y * rhs.z - z * rhs.y,
			z * rhs.x - x * rhs.z,
			x * rhs.y - y * rhs.x
		);
	}

	// Produit scalaire entre deux Vec3D
	static float DotProduct(const Vec3D& lhs, const Vec3D& rhs)
	{
		return lhs.Dot(rhs);
	}

	// Produit vectoriel entre deux Vec3D
	static Vec3D CrossProduct(const Vec3D& lhs, const Vec3D& rhs)
	{
		return lhs.Cross(rhs);
	}

	// Méthode ToString() pour l'affichage du Vec3D 
	virtual std::string ToString() const
	{
		return "X = " + std::to_string(x) + " Y = " + std::to_string(y) + " Z = " + std::to_string(z);
	}

	// Calcul de la distance entre deux points A et B
	static float Distance(const Vec3D<T>& lhs, const Vec3D<T>& rhs)
	{
		Vec3D<T> v(lhs, rhs);
		return v.GetLength();
	}

	// Récupération de la composante X du Vec3D
	virtual T GetX() const
	{
		return x;
	}

	// Récupération de la composante Y du Vec3D
	virtual T GetY() const
	{
		return y;
	}

	// Récupération de la composante Z du Vec3D
	virtual T GetZ() const
	{
		return z;
	}

protected:
	T x = static_cast<T>(0);
	T y = static_cast<T>(0);
	T z = static_cast<T>(0);
};

typedef Vec3D<int> IVec3D;
typedef Vec3D<long long> LLVec3D;
typedef Vec3D<float> FVec3D;