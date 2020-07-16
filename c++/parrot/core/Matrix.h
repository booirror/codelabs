#ifndef _PARROT_MATRIX_H
#define _PARROT_MATRIX_H
#include <cmath>
namespace parrot
{
	class Matrix
	{
	public:
		union
		{
			float m[4][4];
			struct
			{
				float _11; float _12; float _13; float _14;
				float _21; float _22; float _23; float _24;
				float _31; float _32; float _33; float _34;
				float _41; float _42; float _43; float _44;
			};
		};
	public:
		Matrix() = default;

		Matrix(
			float a1, float a2, float a3, float a4,
			float b1, float b2, float b3, float b4,
			float c1, float c2, float c3, float c4,
			float d1, float d2, float d3, float d4)
		{
			_11 = a1; _12 = a2; _13 = a3; _14 = a4;
			_21 = b1; _22 = b2; _23 = b3; _24 = b4;
			_31 = c1; _32 = c2; _33 = c3; _34 = c4;
			_41 = d1; _42 = d2; _43 = d3; _44 = d4;
		}

		Matrix(const Matrix& rhs)
		{
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					m[i][j] = rhs.m[i][j];
				}
			}
		}

		Matrix operator=(const Matrix& rhs)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j) {
					m[i][j] = rhs.m[i][j];
				}
			}
			return *this;
		}
	public:

		static Matrix identity();
		void setZero();
		bool operator==(const Matrix& rhs) const;
		Matrix operator+(const Matrix& rhs) const;
		Matrix operator-(const Matrix& rhs) const;
		Matrix operator*(const Matrix& rhs) const;

		//转置
		Matrix transpose() const
		{
			const Matrix& mat = *this;
			return Matrix(mat._11, mat._21, mat._31, mat._41,
				mat._12, mat._22, mat._32, mat._42,
				mat._13, mat._23, mat._33, mat._43,
				mat._14, mat._24, mat._34, mat._44);
		}

		//矩阵对应行列式	
		/************************************************************************/
		/* a11a22a33a44 - a11a22a34a43 - a11a23a32a44 + a11a23a34a42
		+ a11a24a32a43 - a11a24a33a42 - a12a21a33a44 + a12a21a34a43
		+ a12a23a31a44 - a12a23a34a41 - a12a24a31a43 + a12a24a33a41
		+ a13a21a32a44 - a13a21a34a42 - a13a22a31a44 + a13a22a34a41
		+ a13a24a31a42 - a13a24a32a41 - a14a21a32a43 + a14a21a33a42
		+ a14a22a31a43 - a14a22a33a41 - a14a23a31a42 + a14a23a32a41                                                                     */
		/************************************************************************/
		float det() const
		{
			const Matrix& mat = *this;
			float result =
				mat._11*mat._22*mat._33*mat._44 - mat._11*mat._22*mat._34*mat._43 -
				mat._11*mat._23*mat._32*mat._44 + mat._11*mat._23*mat._34*mat._42 +

				mat._11*mat._24*mat._32*mat._43 - mat._11*mat._24*mat._33*mat._42 -
				mat._12*mat._21*mat._33*mat._44 + mat._12*mat._21*mat._34*mat._43 +

				mat._12*mat._23*mat._31*mat._44 - mat._12*mat._23*mat._34*mat._41 -
				mat._12*mat._24*mat._31*mat._43 + mat._12*mat._24*mat._33*mat._41 +

				mat._13*mat._21*mat._32*mat._44 - mat._13*mat._21*mat._34*mat._42 -
				mat._13*mat._22*mat._31*mat._44 + mat._13*mat._22*mat._34*mat._41 +

				mat._13*mat._24*mat._31*mat._42 - mat._13*mat._24*mat._32*mat._41 -
				mat._14*mat._21*mat._32*mat._43 + mat._14*mat._21*mat._33*mat._42 +

				mat._14*mat._22*mat._31*mat._43 - mat._14*mat._22*mat._33*mat._41 -
				mat._14*mat._23*mat._31*mat._42 + mat._14*mat._23*mat._32*mat._41;

			return result;
		}
		
		//伴随矩阵 代数余子式组成的矩阵的转置
		Matrix adj() const;

		//逆矩阵 = 伴随矩阵/(行列式值的绝对值)
		static Matrix inverse(const Matrix& mat)
		{
			float det = abs(mat.det());
			Matrix adj = mat.adj();
			Matrix inverse;
			for (int i = 0; i < 4; ++i) {
				for (int j = 0; j < 4; ++j) {
					inverse.m[i][j] = adj.m[i][j] / det;
				}
			}
			return inverse;
		}

		//缩放
		Matrix scale(float sx, float sy, float sz)
		{
			return Matrix(
				sx, 0, 0, 0,
				0, sy, 0, 0,
				0, 0, sz, 0,
				0,  0, 0, 1
				);
		}

		//平移
		Matrix translate(float offsetX, float offsetY, float offsetZ)
		{
			return Matrix(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				offsetX, offsetY, offsetZ, 1
			);
		}

		//绕x轴旋转
		Matrix rotateX(float angle)
		{
			return Matrix(
				1, 0, 0, 0,
				0, cos(angle), sin(angle), 0,
				0, -sin(angle), cos(angle), 0,
				0, 0, 0, 1
				);
		}
		//绕y轴旋转
		Matrix rotateY(float angle)
		{
			return Matrix(
				cos(angle), 0, -sin(angle), 0,
				0,          1, 0, 0,
				sin(angle), 0, cos(angle), 0,
				0,          0, 0, 1
				);
		}
		//绕z轴旋转
		Matrix rotateZ(float angle)
		{
			return Matrix(
				 cos(angle), sin(angle), 0, 0,
				-sin(angle), cos(angle), 0, 0,
				0,                0,     1, 0,
				0,                0,     0, 1
				);
		}


	};

}// namespace end;

#endif

