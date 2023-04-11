#pragma once

/*
 * Classe Rotator
 * Définit une rotation dans l'espace
 */
template <typename T>
[[deprecated("Classe dépreciée : Utilisez Quaternion à la place")]]
class Rotator
{
public:
	// Constructeur par défaut
	Rotator() = default;

	// Constructeur définissant les angles Roll (axe X), Pitch (axe Y) et Yaw (axe Z)
	constexpr Rotator(T roll, T pitch, T yaw)
		:
		roll(roll),
		pitch(pitch),
		yaw(yaw)
	{}

	// Constructeur de copie
	Rotator& operator=(const Rotator& rhs)
	{
		roll = rhs.roll;
		pitch = rhs.pitch;
		yaw = rhs.yaw;
		return *this;
	}

	// Conversion implicite
	template<typename T2>
	Rotator(Rotator<T2> r)
		:
		Rotator(static_cast<T>(r.roll), static_cast<T>(r.pitch), static_cast<T>(r.yaw))
	{}

	// Opérateurs d'addition entre deux Rotator
	Rotator operator+(const Rotator& rhs) const
	{
		Rotator out = *this;
		out.roll += rhs.roll;
		out.pitch += rhs.pitch;
		out.yaw += rhs.yaw;

		return out;
	}
	void operator+=(const Rotator& rhs)
	{
		*this = *this + rhs;
	}

	// Opérateurs de multiplication entre un Rotator et un scalaire
	Rotator operator*(T scale) const
	{
		Rotator out = *this;
		out.roll *= scale;
		out.pitch *= scale;
		out.yaw *= scale;

		return out;
	}
	void operator*=(T scale)
	{
		*this = *this * scale;
	}

public:
	T roll = static_cast<T>(0);
	T pitch = static_cast<T>(0);
	T yaw = static_cast<T>(0);
};

typedef Rotator<float> FRotator;
typedef Rotator<int> IRotator;