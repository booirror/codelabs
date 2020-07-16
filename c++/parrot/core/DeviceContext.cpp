#include "DeviceContext.h"
#include <time.h>
namespace parrot
{
//startVertexLocation û��Ҫ��
void DeviceContext::drawIndexed(UINT indexCount, UINT startIndexLocation, UINT startVertexLocation)
{
	auto matrix = MathUtil::matrixScreenTransform(mpDevice->getClientWidth(), mpDevice->getClientHeight());
	for (int i = startIndexLocation; i < indexCount / 3; ++i)
	{
		
		Vertex& p1 = mVertices[mIndices[3 * i]];
		Vertex& p2 = mVertices[mIndices[3 * i + 1]];
		Vertex& p3 = mVertices[mIndices[3 * i + 2]];

		if (!backfaceCulling(p1, p2, p3)) {
			continue;
		}
		VertexOut v1 = transformToProj(p1);
		VertexOut v2 = transformToProj(p2);
		VertexOut v3 = transformToProj(p3);

		if (clip(v1) || clip(v2) || clip(v3)) {
			continue;
		}
		toCVV(v1);
		toCVV(v2);
		toCVV(v3);

		this->transformToScreen(matrix, v1);
		this->transformToScreen(matrix, v2);
		this->transformToScreen(matrix, v3);
		drawTriangle(v1, v2, v3);
	}
}

VertexOut DeviceContext::transformToProj(const Vertex& v)
{
	VertexOut out = mpShader->VS(v);
	out.deepTest = 1 / out.projPos.w;
	
	out.color.x *= out.deepTest;
	out.color.y *= out.deepTest;
	out.color.z *= out.deepTest;

	out.normal.x *= out.deepTest;
	out.normal.y *= out.deepTest;
	out.normal.z *= out.deepTest;

	out.texPos.u *= out.deepTest;
	out.texPos.v *= out.deepTest;
	return out;
}

bool DeviceContext::clip(const VertexOut& v)
{
	//cvvΪ x-1,1  y-1,1  z0,1
	if (v.projPos.x >= -v.projPos.w && v.projPos.x <= v.projPos.w &&
		v.projPos.y >= -v.projPos.w && v.projPos.y <= v.projPos.w &&
		v.projPos.z >= 0.f && v.projPos.z <= v.projPos.w)
	{
		return false;
	}
	return true;
}

bool DeviceContext::backfaceCulling(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	if (mMode == WIREFRAME) {
		return true;
	}
	else {
		//���㻺��˳���룬�������˳��
		Vector4f p1 = v2.pos - v1.pos;
		Vector4f p2 = v3.pos - v1.pos;
		Vector4f normal = p1.cross(p2);
		Vector4f viewDirection = v1.pos - mCameraPos;
		if (normal.dot(viewDirection) < 0) {
			return true;
		}
		else {
			return false;
		}
	}
}

void DeviceContext::drawTriangle(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	if (mMode == WIREFRAME) {
		bresenhamDrawLine(v1.projPos.x, v1.projPos.y, v2.projPos.x, v2.projPos.y);
		bresenhamDrawLine(v1.projPos.x, v1.projPos.y, v3.projPos.x, v3.projPos.y);
		bresenhamDrawLine(v2.projPos.x, v2.projPos.y, v3.projPos.x, v3.projPos.y);
	}
	else {
		// SOLIDE
		rasterization(v1, v2, v3);
	}
}

void DeviceContext::rasterization(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	//�ж��Ƿ���ƽ�׻���ƽ��������
	if (v1.projPos.y == v2.projPos.y)
	{
		if (v1.projPos.y < v3.projPos.y)
		{//ƽ��
			drawTriangleTop(v1, v2, v3);
		}
		else
		{//ƽ��
			drawTriangleBottom(v3, v1, v2);
		}
	}
	else if (v1.projPos.y == v3.projPos.y)
	{
		if (v1.projPos.y < v2.projPos.y)
		{//ƽ��
			drawTriangleTop(v1, v3, v2);
		}
		else
		{//ƽ��
			drawTriangleBottom(v2, v1, v3);
		}
	}
	else if (v2.projPos.y == v3.projPos.y)
	{
		if (v2.projPos.y < v1.projPos.y)
		{//ƽ��
			drawTriangleTop(v2, v3, v1);
		}
		else
		{//ƽ��
			drawTriangleBottom(v1, v2, v3);
		}
	}
	//һ�������� ����ָ��ƽ�������κ�ƽ��������
	else
	{
		//����yֵ��������������
		std::vector<VertexOut> vertices{ v1, v2, v3 };
		std::sort(vertices.begin(), vertices.end(), [](VertexOut v1, VertexOut v2) {
			return v1.projPos.y < v2.projPos.y; });
		VertexOut top = vertices[0];
		VertexOut middle = vertices[1];
		VertexOut bottom = vertices[2];

		//��ֵ���м��
		float middleX = (middle.projPos.y - top.projPos.y) * (bottom.projPos.x - top.projPos.x) /
			(bottom.projPos.y - top.projPos.y) + top.projPos.x;
		float dy = middle.projPos.y - top.projPos.y;
		float t = dy / (bottom.projPos.y - top.projPos.y);

		VertexOut newMiddle = top.interpolate(bottom, t);
		newMiddle.projPos.x = middleX;
		newMiddle.projPos.y = middle.projPos.y;

		//ƽ��
		drawTriangleTop(middle, newMiddle, bottom);
		//ƽ��
		drawTriangleBottom(top, middle, newMiddle);
	}
}

void DeviceContext::bresenhamDrawLine(int x1, int y1, int x2, int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int stepx = 1;
	int stepy = 1;

	if (dx >= 0)
	{
		stepx = 1;
	}
	else
	{
		stepx = -1;
		dx = abs(dx);
	}

	if (dy >= 0)
	{
		stepy = 1;
	}
	else
	{
		stepy = -1;
		dy = abs(dy);
	}

	int deltaX = 2 * dx;
	int deltaY = 2 * dy;
	if (dx > dy)
	{
		int error = deltaY - dx;
		for (int i = 0; i <= dx; ++i)
		{
			if (x1 >= 0 && x1 < mpDevice->getClientWidth() && y1 >= 0 && y1 < mpDevice->getClientHeight())
				mpDevice->drawPixel(x1, y1, Vector4f(0.f, 0.f, 0.f, 1.f));
			if (error >= 0)
			{
				error -= deltaX;
				y1 += stepy;
			}
			error += deltaY;
			x1 += stepx;
		}
	}
	else
	{
		int error = deltaX - dy;
		for (int i = 0; i <= dy; i++)
		{
			if (x1 >= 0 && x1 < mpDevice->getClientWidth() && y1 >= 0 && y1 < mpDevice->getClientHeight())
				mpDevice->drawPixel(x1, y1, Vector4f(0.f, 0.f, 0.f, 1.f));
			if (error >= 0)
			{
				error -= deltaY;
				x1 += stepx;
			}
			error += deltaX;
			y1 += stepy;
		}
	}
}


void DeviceContext::drawTriangleTop(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	float dy = 0;//ÿ��y����һ������
	for (float y = v1.projPos.y; y <= v3.projPos.y; y += 1.f)
	{
		//��������
		int yIndex = static_cast<int>(y + 0.5f);
		if (yIndex >= 0 && yIndex < mpDevice->getClientHeight())
		{
			float t = dy / (v3.projPos.y - v1.projPos.y);

			//��ֵ�������Ҷ���
			VertexOut new1 = v1.interpolate(v3, t);
			VertexOut new2 = v2.interpolate(v3, t);
			dy += 1.f;
			//ɨ�������
			if (new1.projPos.x < new2.projPos.x)
			{
				scanlineFill(new1, new2, yIndex);
			}
			else
			{
				scanlineFill(new2, new1, yIndex);
			}
		}

	}
}

//��ƽ�������� v1Ϊ�϶���
void DeviceContext::drawTriangleBottom(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	float dy = 0;//ÿ��y����һ������
	for (float y = v1.projPos.y; y <= v2.projPos.y; y += 1.f)
	{
		//��������
		int yIndex = static_cast<int>(y + 0.5f);

		if (yIndex >= 0 && yIndex < mpDevice->getClientHeight())
		{
			float t = dy / (v2.projPos.y - v1.projPos.y);

			//��ֵ���Ҷ���
			VertexOut new1 = v1.interpolate(v2, t);
			VertexOut new2 = v1.interpolate(v3, t);
			dy += 1.f;

			//ɨ�������
			if (new1.projPos.x < new2.projPos.x)
			{
				scanlineFill(new1, new2, yIndex);
			}
			else
			{
				scanlineFill(new2, new1, yIndex);
			}
		}
	}
}

void DeviceContext::scanlineFill(const VertexOut& left, const VertexOut& right, int yIndex)
{
	float dx = right.projPos.x - left.projPos.x;

	for (float x = left.projPos.x; x <= right.projPos.x; x += 1.f)
	{
		//��������
		int xIndex = static_cast<int>(x + .5f);

		if (xIndex >= 0 && xIndex < mpDevice->getClientWidth())
		{
			//��ֵϵ��
			float lerpFactor = 0;
			if (dx != 0)
			{
				lerpFactor = (x - left.projPos.x) / dx;
			}

			//��Ȳ���
			//1/z����x����y'�����Թ�ϵ��
			float oneDivZ = MathUtil::interpolate(left.deepTest, right.deepTest, lerpFactor);
			if (oneDivZ >= mpDevice->getZ(xIndex, yIndex))
			{
				mpDevice->setZ(xIndex, yIndex, oneDivZ);

				float w = 1 / oneDivZ;
				//��ֵ���� ԭ����Ҫ��ֵ����Ϣ������oneDivZ
				//���ڵõ���ֵ�����Ϣ��Ҫ����oneDivZ�õ���ʵֵ
				VertexOut out = left.interpolate(right, lerpFactor);
				out.projPos.y = yIndex;
				out.texPos.u *= w;
				out.texPos.v *= w;
				out.normal.x *= w;
				out.normal.y *= w;
				out.normal.z *= w;
				out.color.x *= w;
				out.color.y *= w;
				out.color.z *= w;
				//�����ص���ɫ
				mpDevice->drawPixel(xIndex, yIndex, mpShader->PS(out));
			}
		}
	}
}

}