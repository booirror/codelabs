#ifndef _PARROT_VECTOR_H_
#define _PARROT_VECTOR_H_
#include "Matrix.h"

namespace parrot
{
	//纹理坐标
	class Vector2f
	{
	public:
		float u;
		float v;
	public:
		Vector2f() = default;
		Vector2f(float x, float y) : u(x), v(y){}
		Vector2f(const Vector2f& rhs) :u(rhs.u), v(rhs.v){}
		Vector2f& operator=(const Vector2f& rhs)
		{
			if (this == &rhs) {
				return *this;
			}
			u = rhs.u;
			v = rhs.v;
			return *this;
		}

		Vector2f interpolate(const Vector2f& v, float t) const;

	};

	///////////////////////////////////////////
	class Vector3f 
	{
	public:
		float x;
		float y;
		float z;

	public:
		Vector3f() = default;
		Vector3f(float _x, float _y, float _z) :x(_x), y(_y), z(_z){}
		Vector3f(const Vector3f& v) : x(v.x), y(v.y), z(v.z){}
		Vector3f& operator= (const Vector3f& rhs)
		{
			if (this == &rhs) {
				return *this;
			}
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			return *this;
		}

		Vector3f interpolate(const Vector3f& v, float t) const;

		float length() const;
		
		
		bool operator==(const Vector3f& rhs) const;
		
		
	};

	/////////////////////////////////////////////
	class Vector4f 
	{
	public:
		float x;
		float y;
		float z;
		float w;//0表示向量，1表示点

	public:
		Vector4f() = default;
		Vector4f(float _x, float _y, float _z, float _w = 0.0f) :x(_x), y(_y), z(_z), w(_w){}
		Vector4f(const Vector4f& rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w){}
		Vector4f& operator=(const Vector4f& rhs)
		{
			if (this == &rhs) {
				return *this;
			}
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
			return *this;
		}

		Vector4f operator*(const Matrix& rhs) const;
		float length() const {
			float sq = x*x + y*y + z*z;
			return sqrt(sq);
		}
		Vector4f normalize();
		Vector4f interpolate(const Vector4f& v, float t) const;

		Vector4f operator*(const Vector4f& rhs) const;
		Vector4f operator+(const Vector4f& rhs) const;
		Vector4f operator-(const Vector4f& rhs) const;
		Vector4f operator-() const;
		Vector4f operator*(float factor) const;
		float dot(const Vector4f& rhs) const;
		Vector4f cross(const Vector4f& rhs) const;
	};
}

#endif