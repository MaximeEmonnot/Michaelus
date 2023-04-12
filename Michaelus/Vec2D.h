#pragma once

#include <functional>
#include <string>

#include "Math.h"

/*
 * Classe Vec2D
 * Définit un vecteur dans un espace 2D
 */
template <typename T>
class Vec2D {
public:
	// Classe de hashage pour l'utilisation de la classe dans des std::unordered_map
	class Hash
	{
	public: 
		size_t operator()(const Vec2D& v) const {
			std::hash<T> hasher;
			return (hasher(v.x) << 32 | hasher(v.y));
		}
	};

public:
	// Constructeur par défaut
	Vec2D() = default;

	// Constructeur définissant directement X Y
	constexpr Vec2D(T x, T	y)
		:
		x(x),
		y(y)
	{}

	// Constructeur définissant le vecteur 2D du point A au point B
	constexpr Vec2D(Vec2D pos0, Vec2D pos1)
		:
		x(pos1.x - pos0.x),
		y(pos1.y - pos0.y)
	{}

	// Constructeur de copie
	Vec2D& operator= (const Vec2D& rhs) {
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	// Conversion implicite
	template <typename T2>
	Vec2D(Vec2D<T2> v)
		:
		Vec2D(static_cast<T>(v.GetX()), static_cast<T>(v.GetY()))
	{}

	// Opérateur de négation
	virtual Vec2D operator-() {
		x *= -1;
		y *= -1;
		return *this;
	}

	// Opérateurs d'addition entre deux Vec2D
	virtual Vec2D operator+(const Vec2D& rhs) const {
		Vec2D out = *this;
		out.x += rhs.x;
		out.y += rhs.y;
		return out;
	}
	void operator+=(const Vec2D& rhs) {
		*this = *this + rhs;
	}

	// Opérateurs de soustraction entre deux Vec2D
	virtual Vec2D operator-(const Vec2D& rhs) const {
		Vec2D out = *this;
		out.x -= rhs.x;
		out.y -= rhs.y;
		return out;
	}
	void operator-=(const Vec2D& rhs) {
		*this = *this - rhs;
	}

	// Opérateurs de multiplication entre un Vec2D et un scalaire
	virtual Vec2D operator*(T scale) const {
		Vec2D out = *this;
		out.x *= scale;
		out.y *= scale;
		return out;
	}
	void operator*=(T scale) {
		*this = *this * scale;
	}

	// Opérateurs de division entre un Vec2D et un scalaire
	Vec2D operator/(T scale) const {
		T invScale = 1 / scale;
		return *this * invScale;
	}
	void operator/=(T scale) {
		*this = *this / scale;
	}

	// Opérateur d'égalité entre deux Vec2D
	virtual bool operator==(const Vec2D& rhs) const {
		return x == rhs.x && y == rhs.y;
	}

	// Opérateur d'inégalité entre deux Vec2D
	bool operator!=(const Vec2D& rhs) const {
		return !(*this == rhs);
	}

	// Récupération de la longueur d'un vecteur 2D
	virtual float GetLength() const {
		return MMath::Sqrt(static_cast<float>(x * x + y * y));
	}

	// Récupération du vecteur 2D normalisé
	virtual Vec2D GetNormalized() const {
		Vec2D v = *this;
		if (GetLength() > 0) {
			float magnitude = 1 / GetLength();
			v.x *= static_cast<T>(magnitude);
			v.y *= static_cast<T>(magnitude);
			return v;
		}
		return Vec2D();
	}

	// Normalisation du Vec2D
	void Normalize() {
		*this = GetNormalized();
	}

	// Produit scalaire entre deux Vec2D
	virtual float Dot(const Vec2D& rhs) const {
		return static_cast<float>(x * rhs.x + y * rhs.y);
	}

	// Produit scalaire entre deux Vec2D
	static float DotProduct(const Vec2D& lhs, const Vec2D& rhs)
	{
		return lhs.Dot(rhs);
	}

	// Méthode ToString() pour l'affichage du Vec2D 
	virtual std::string ToString() const
	{
		return "X = " + std::to_string(x) + " Y = " + std::to_string(y);
	}

	// Calcul de la distance entre deux points A et B
	static float Distance(const Vec2D<T>& lhs, const Vec2D<T>& rhs)
	{
		Vec2D<T> v(lhs, rhs);
		return v.GetLength();
	}

	// Récupération de la composante X du Vec2D
	virtual T GetX() const
	{
		return x;
	}

	// Récupération de la composante Y du Vec2D
	virtual T GetY() const
	{
		return y;
	}

protected:
	T x = static_cast<T>(0);
	T y = static_cast<T>(0);
};

typedef Vec2D<int> IVec2D;
typedef Vec2D<long long> LLVec2D;
typedef Vec2D<float> FVec2D;