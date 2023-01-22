#pragma once

template <typename T>
class Rotator
{
public:
	Rotator() = default;

	constexpr Rotator(T roll, T pitch, T yaw)
		:
		roll(roll),
		pitch(pitch),
		yaw(yaw)
	{}

	Rotator& operator=(const Rotator& rhs)
	{
		roll = rhs.roll;
		pitch = rhs.pitch;
		yaw = rhs.yaw;
		return *this;
	}

	template<typename T2>
	Rotator(Rotator<T2> r)
		:
		Rotator(static_cast<T>(r.roll), static_cast<T>(r.pitch), static_cast<T>(r.yaw))
	{}

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

public:
	T roll = static_cast<T>(0);
	T pitch = static_cast<T>(0);
	T yaw = static_cast<T>(0);
};

typedef Rotator<float> FRotator;
typedef Rotator<int> IRotator;