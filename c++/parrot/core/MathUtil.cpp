#include "MathUtil.h"

namespace parrot
{
	float MathUtil::PI = 3.1415926535898f;


	Matrix MathUtil::matrixLookAtLH(Vector4f eye, Vector4f lookAt, Vector4f up)
{

	Vector4f zaxis = lookAt - eye;
	zaxis.normalize();
	Vector4f xaxis = up.cross(zaxis).normalize();
	Vector4f yaxis = zaxis.cross(xaxis);

	return Matrix(
		xaxis.x, yaxis.x, zaxis.x, 0,
		xaxis.y, yaxis.y, zaxis.y, 0,
		xaxis.z, yaxis.z, zaxis.z, 0,
		-xaxis.dot(eye), -yaxis.dot(eye), -zaxis.dot(eye), 1
		);
}

	Matrix MathUtil::matrixPerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ)
{
	Matrix mat;
	mat.setZero();
	// tan(fovAngleY*0.5f)
	float height = cos(fovAngleY*0.5f) / sin(fovAngleY*0.5f);
	mat._11 = height / aspectRatio;
	mat._22 = height;
	mat._33 = farZ / (farZ - nearZ);
	mat._34 = 1.f;
	mat._43 = (nearZ * farZ) / (nearZ - farZ);
	return mat;
}

	Matrix MathUtil::matrixScreenTransform(int clientWidth, int clientHeight)
{
	return Matrix(
		clientWidth / 2, 0, 0, 0,
		0, clientHeight / 2, 0, 0,
		0, 0, 1, 0,
		clientWidth / 2, clientHeight / 2, 0, 1
		);
}

	float MathUtil::matrixAdjElem(
	float a1, float a2, float a3,
	float b1, float b2, float b3,
	float c1, float c2, float c3)
{
	return a1*(b2*c3 - c2*b3) - a2*(b1*c3 - c1*b3) + a3*(b1*c2 - c1*b2);
}

}