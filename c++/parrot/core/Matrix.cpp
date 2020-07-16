#include <cfloat>
#include "Matrix.h"
#include "MathUtil.h"

namespace parrot
{
Matrix Matrix::identity()
{
	return Matrix(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1);
}

void Matrix::setZero()
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m[i][j] = 0.0f;
		}
	}
}

bool Matrix::operator==(const Matrix& rhs) const
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (abs(m[i][j] - rhs.m[i][j]) >FLT_EPSILON)
				return false;
		}
	}
	return true;
}

Matrix Matrix::operator+(const Matrix& rhs) const
{
	Matrix m;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m.m[i][j] = this->m[i][j] + rhs.m[i][j];
		}
	}
	return m;
}

Matrix Matrix::operator-(const Matrix& rhs) const
{
	Matrix m;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m.m[i][j] = this->m[i][j] - rhs.m[i][j];
		}
	}
	return m;
}

Matrix Matrix::operator*(const Matrix& rhs) const
{
	Matrix mat;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
		{
			mat.m[j][i] = 
				(m[j][0] * rhs.m[0][i]) +
				(m[j][1] * rhs.m[1][i]) +
				(m[j][2] * rhs.m[2][i]) +
				(m[j][3] * rhs.m[3][i]);
		}
	return mat;
}

Matrix Matrix::adj() const
{
	const Matrix& mat = *this;
	float a1 = MathUtil::matrixAdjElem(mat._22, mat._23, mat._24, mat._32, mat._33, mat._34, mat._42, mat._43, mat._44);
	float a2 = MathUtil::matrixAdjElem(mat._21, mat._23, mat._24, mat._31, mat._33, mat._34, mat._41, mat._43, mat._44);
	float a3 = MathUtil::matrixAdjElem(mat._21, mat._22, mat._24, mat._31, mat._32, mat._34, mat._41, mat._42, mat._44);
	float a4 = MathUtil::matrixAdjElem(mat._21, mat._22, mat._23, mat._31, mat._32, mat._33, mat._41, mat._42, mat._43);
	float b1 = MathUtil::matrixAdjElem(mat._12, mat._13, mat._14, mat._32, mat._33, mat._34, mat._42, mat._43, mat._44);
	float b2 = MathUtil::matrixAdjElem(mat._11, mat._13, mat._14, mat._31, mat._33, mat._34, mat._41, mat._43, mat._44);
	float b3 = MathUtil::matrixAdjElem(mat._11, mat._12, mat._14, mat._31, mat._32, mat._34, mat._41, mat._42, mat._44);
	float b4 = MathUtil::matrixAdjElem(mat._11, mat._12, mat._13, mat._31, mat._32, mat._33, mat._41, mat._42, mat._43);
	float c1 = MathUtil::matrixAdjElem(mat._12, mat._13, mat._14, mat._22, mat._23, mat._24, mat._42, mat._43, mat._44);
	float c2 = MathUtil::matrixAdjElem(mat._11, mat._13, mat._14, mat._21, mat._23, mat._24, mat._41, mat._43, mat._44);
	float c3 = MathUtil::matrixAdjElem(mat._11, mat._12, mat._14, mat._21, mat._22, mat._24, mat._41, mat._42, mat._44);
	float c4 = MathUtil::matrixAdjElem(mat._11, mat._12, mat._13, mat._21, mat._22, mat._23, mat._41, mat._42, mat._43);
	float d1 = MathUtil::matrixAdjElem(mat._12, mat._13, mat._14, mat._22, mat._23, mat._24, mat._32, mat._33, mat._34);
	float d2 = MathUtil::matrixAdjElem(mat._11, mat._13, mat._14, mat._21, mat._23, mat._24, mat._31, mat._33, mat._34);
	float d3 = MathUtil::matrixAdjElem(mat._11, mat._12, mat._14, mat._21, mat._22, mat._24, mat._31, mat._32, mat._34);
	float d4 = MathUtil::matrixAdjElem(mat._11, mat._12, mat._13, mat._21, mat._22, mat._23, mat._31, mat._32, mat._33);

	Matrix result(
		a1, -a2, a3, -a4,
		-b1, b2, -b3, b4,
		c1, -c2, c3, -c4,
		-d1, d2, -d3, d4
		);
	return result.transpose();
}


}