#ifndef _PARROT_MATHUTIL_H__
#define _PARROT_MATHUTIL_H__
#include "Type.h"
#include "Matrix.h"
#include "Vector.h"

namespace parrot{


class MathUtil
{
public:
	static float PI;
	
	static Matrix matrixLookAtLH(Vector4f eye, Vector4f lookAt, Vector4f up);

	//获取投影矩阵 同dx中的XMMatrixPerspectiveFovLH
	//									    观察角            宽高比       近裁剪平面   远裁剪平面       
	static Matrix matrixPerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ);

	//投影出来的坐标转屏幕坐标变换矩阵
	static Matrix matrixScreenTransform(int clientWidth, int clientHeight);

	static float matrixAdjElem(
		float a1, float a2, float a3,
		float b1, float b2, float b3,
		float c1, float c2, float c3);



	static inline float interpolate(float x1, float x2, float t)
	{
		return x1 + (x2 - x1)*t;
	}

	//
	static float clamp(float x, float min, float max)
	{
		if (x <= min) return min;
		else if (x >= max) return max;
		return x;
	}

	//角度转弧度
	static inline float degree2Radian(float degree) { return degree*(PI / 180.0f); }

	//颜色转化为UINT
	static inline UINT colorToUInt(const Vector4f& color)
	{
		BYTE red = 255 * color.x/*  color.w*/;
		BYTE green = 255 * color.y/* color.w*/;
		BYTE blue = 255 * color.z /* color.w*/;
		return (UINT)(blue | (green << 8) | (red << 16));
		//BYTE alpha = 255 * color.w;
		//return (UINT)(blue | (green << 8) | (red << 16) | (alpha << 24));
	}

	//求入射向量关于法线的反射向量
	//公式 R = I - 2(I·N)N
	static Vector4f reflect(const Vector4f& i, const Vector4f& normal)
	{
		float temp = 2.f*i.dot(normal);
		return i - (normal*temp);
	}
};

}
#endif