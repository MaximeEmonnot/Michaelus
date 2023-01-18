#pragma once
#include "Math.h"

template <typename T>
class Mat4{
public:
	constexpr Mat4() = default;

	constexpr Mat4(T v)
		:
		Mat4(v, v, v, v,
			v, v, v, v,
			v, v, v, v,
			v, v, v, v )
	{
	}

	constexpr Mat4(
		T t1, T u1, T v1, T w1,
		T t2, T u2, T v2, T w2,
		T t3, T u3, T v3, T w3,
		T t4, T u4, T v4, T w4
	)
	{
		mArray[0][0] = t1;
		mArray[0][1] = u1;
		mArray[0][3] = v1;
		mArray[0][3] = w1;

		mArray[1][0] = t2;
		mArray[1][1] = u2;
		mArray[1][3] = v2;
		mArray[1][3] = w2;

		mArray[2][0] = t3;
		mArray[2][1] = u3;
		mArray[2][3] = v3;
		mArray[2][3] = w3;

		mArray[3][0] = t4;
		mArray[3][1] = u4;
		mArray[3][3] = v4;
		mArray[3][3] = w4;
	}

	constexpr Mat4 operator* (const Mat4& rhs)
	{
		Mat4 out = Mat4(
			mArray[0][0] * rhs.mArray[0][0] + mArray[0][1] * rhs.mArray[1][0] + mArray[0][2] * rhs.mArray[2][0] + mArray[0][3] * rhs.mArray[3][0], mArray[0][0] * rhs.mArray[0][1] + mArray[0][1] * rhs.mArray[1][1] + mArray[0][2] * rhs.mArray[2][1] + mArray[0][3] * rhs.mArray[3][1], mArray[0][0] * rhs.mArray[0][2] + mArray[0][1] * rhs.mArray[1][2] + mArray[0][2] * rhs.mArray[2][2] + mArray[0][3] * rhs.mArray[3][2], mArray[0][0] * rhs.mArray[0][3] + mArray[0][1] * rhs.mArray[1][3] + mArray[0][2] * rhs.mArray[2][3] + mArray[0][3] * rhs.mArray[3][3],
			mArray[1][0] * rhs.mArray[0][0] + mArray[1][1] * rhs.mArray[1][0] + mArray[1][2] * rhs.mArray[2][0] + mArray[1][3] * rhs.mArray[3][0], mArray[1][0] * rhs.mArray[0][1] + mArray[1][1] * rhs.mArray[1][1] + mArray[1][2] * rhs.mArray[2][1] + mArray[1][3] * rhs.mArray[3][1], mArray[1][0] * rhs.mArray[0][2] + mArray[1][1] * rhs.mArray[1][2] + mArray[1][2] * rhs.mArray[2][2] + mArray[1][3] * rhs.mArray[3][2], mArray[1][0] * rhs.mArray[0][3] + mArray[1][1] * rhs.mArray[1][3] + mArray[1][2] * rhs.mArray[2][3] + mArray[1][3] * rhs.mArray[3][3],
			mArray[2][0] * rhs.mArray[0][0] + mArray[2][1] * rhs.mArray[1][0] + mArray[2][2] * rhs.mArray[2][0] + mArray[2][3] * rhs.mArray[3][0], mArray[2][0] * rhs.mArray[0][1] + mArray[2][1] * rhs.mArray[1][1] + mArray[2][2] * rhs.mArray[2][1] + mArray[2][3] * rhs.mArray[3][1], mArray[2][0] * rhs.mArray[0][2] + mArray[2][1] * rhs.mArray[1][2] + mArray[2][2] * rhs.mArray[2][2] + mArray[2][3] * rhs.mArray[3][2], mArray[2][0] * rhs.mArray[0][3] + mArray[2][1] * rhs.mArray[1][3] + mArray[2][2] * rhs.mArray[2][3] + mArray[2][3] * rhs.mArray[3][3],
			mArray[3][0] * rhs.mArray[0][0] + mArray[3][1] * rhs.mArray[1][0] + mArray[3][2] * rhs.mArray[2][0] + mArray[3][3] * rhs.mArray[3][0], mArray[3][0] * rhs.mArray[0][1] + mArray[3][1] * rhs.mArray[1][1] + mArray[3][2] * rhs.mArray[2][1] + mArray[3][3] * rhs.mArray[3][1], mArray[3][0] * rhs.mArray[0][2] + mArray[3][1] * rhs.mArray[1][2] + mArray[3][2] * rhs.mArray[2][2] + mArray[3][3] * rhs.mArray[3][2], mArray[3][0] * rhs.mArray[0][3] + mArray[3][1] * rhs.mArray[1][3] + mArray[3][2] * rhs.mArray[2][3] + mArray[3][3] * rhs.mArray[3][3]
		);

		return out;
	}

	static constexpr Mat4 Rotate(Mat4 matrix, float angle, Vec3D<T> axis)
	{
		const float cosAngle = MMath::Cos(angle);
		const float sinAngle = MMath::Sin(angle);

		axis.Normalize();

		Mat4 rotation = Mat4(
			MMath::Square(axis.x) * (1 - cosAngle) + cosAngle, axis.x * axis.y * (1 - cosAngle) - axis.z * sinAngle, axis.x * axis.z * (1 - cosAngle) + axis.y * sinAngle, 0,
			axis.y * axis.x * (1 - cosAngle) + axis.z * sinAngle, MMath::Square(axis.y) * (1 - cosAngle) + cosAngle, axis.y * axis.z * (1 - cosAngle) - axis.x * sinAngle, 0,
			axis.z * axis.x * (1 - cosAngle) - axis.y * sinAngle, axis.z * axis.y * (1 - cosAngle) + axis.x * sinAngle, MMath::Square(axis.z) * (1 - cosAngle) + cosAngle, 0,
			0, 0, 0, 1
		);

		return matrix * rotation;
	}

	static constexpr Mat4 LookAt(Vec3D<T> eyePosition, Vec3D<T> centerPosition, Vec3D<T> upVector)
	{
		Vec3D<T> z_axis = (centerPosition - eyePosition).GetNormalized();
		Vec3D<T> x_axis = z_axis.Cross(upVector).GetNormalized();
		Vec3D<T> y_axis = x_axis.Cross(z_axis);

		z_axis = -z_axis;

		Mat4 viewMatrix = {
			x_axis.x, x_axis.y, x_axis.z, -x_axis.Dot(eyePosition),
			y_axis.x, y_axis.y, y_axis.z, -y_axis.Dot(eyePosition),
			z_axis.x, z_axis.y, z_axis.z, -z_axis.Dot(eyePosition),
			0		, 0		  , 0		, 1		
		};

		return viewMatrix;
	}

	static constexpr Mat4 Perspective(float fieldOfView, float aspectRatio, float zNear, float zFar)
	{
		Mat4 perspectiveMatrix = {
			1.f / (aspectRatio * MMath::Tan(fieldOfView / 2.f)), 0, 0, 0,
			0, 1.f / MMath::Tan(fieldOfView / 2.f), 0, 0,
			0, 0, -(zFar + zNear)/(zFar - zNear), -(2 * zFar * zNear)/(zFar - zNear),
			0, 0, -1, 0
		};

		return perspectiveMatrix;
	}

public:
	T mArray[4][4] = { 0 };
};

typedef Mat4<int> IMat4;
typedef Mat4<float> FMat4;
typedef Mat4<long long> LLMat4;