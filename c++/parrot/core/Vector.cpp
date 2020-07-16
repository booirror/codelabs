#include <cmath>
#include <cfloat>
#include "Vector.h"
#include "MathUtil.h"

namespace parrot
{
	float Vector3f::length() const
	{
		float sq = x*x + y*y + z*z;
		return sqrt(sq);
	}

	Vector4f Vector4f::normalize()
	{
		float len = length();
		if (len != 0.0f)
		{
			float v = 1.0f / len;
			x *= v;
			y *= v;
			z *= v;
		}
		return *this;
	}

	float Vector4f::dot(const Vector4f& v) const
	{
		return x*v.x + y*v.y + z*v.z;
	}

	Vector4f Vector4f::cross(const Vector4f& rhs) const
	{
		float m1 = y*rhs.z - z*rhs.y;
		float m2 = z*rhs.x - x*rhs.z;
		float m3 = x*rhs.y - y*rhs.x;
		return Vector4f(m1, m2, m3, 0.0f);
	}

	bool Vector3f::operator==(const Vector3f& rhs) const
	{
		if (abs(x - rhs.x) <= FLT_EPSILON
			&& abs(y - rhs.y) <= FLT_EPSILON
			&& abs(z - rhs.z) <= FLT_EPSILON)
			return true;
		
		else
			return false;
	}


	Vector4f Vector4f::operator*(const Vector4f& rhs) const
	{
		return Vector4f(x*rhs.x, y*rhs.y, z*rhs.z, w*rhs.w);
	}

	Vector4f Vector4f::operator*(float f) const
	{
		return Vector4f(x*f, y*f, z*f, w);
	}

	Vector4f Vector4f::operator+(const Vector4f& rhs) const
	{
		return Vector4f(x + rhs.x, y + rhs.y, z + rhs.z, 0.0f);
	}

	Vector4f Vector4f::operator-(const Vector4f& rhs) const
	{
		return Vector4f(x - rhs.x, y - rhs.y, z - rhs.z, 0.0f);
	}

	Vector4f Vector4f::operator-() const
	{
		return Vector4f(-x, -y, -z, w);
	}

	Vector4f Vector4f::operator* (const Matrix& rhs) const
	{
		return Vector4f(
			x*rhs._11 + y*rhs._21 + z*rhs._31 + w*rhs._41,
			x*rhs._12 + y*rhs._22 + z*rhs._32 + w*rhs._42,
			x*rhs._13 + y*rhs._23 + z*rhs._33 + w*rhs._43,
			x*rhs._14 + y*rhs._24 + z*rhs._34 + w*rhs._44);
	}

	Vector2f Vector2f::interpolate(const Vector2f& v, float t) const
	{
		return Vector2f(
			MathUtil::interpolate(this->u, v.u, t),
			MathUtil::interpolate(this->v, v.v, t)
			);
	}

	inline Vector3f Vector3f::interpolate(const Vector3f& v, float t) const
	{
		return Vector3f(
			MathUtil::interpolate(this->x, v.x, t),
			MathUtil::interpolate(this->y, v.y, t),
			MathUtil::interpolate(this->z, v.z, t)
			);
	}

	Vector4f Vector4f::interpolate(const Vector4f& v, float t) const
	{
		return Vector4f(
			MathUtil::interpolate(this->x, v.x, t),
			MathUtil::interpolate(this->y, v.y, t),
			MathUtil::interpolate(this->z, v.z, t),
			this->w
			);
	}
}