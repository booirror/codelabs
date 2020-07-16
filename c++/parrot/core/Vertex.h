#ifndef _PARROT_VERTEX_H_
#define _PARROT_VERTEX_H_
#include "Vector.h"
#include "MathUtil.h"
namespace parrot
{

class Vertex
{
public:
	Vector4f pos;
	Vector4f color;
	Vector2f texPos;
	Vector4f normal;

	Vertex() = default;
	Vertex(Vector4f pos, Vector4f color, Vector2f texPos, Vector4f normal)
		:pos(pos), color(color), texPos(texPos), normal(normal){}

	Vertex(const Vertex& rhs) :pos(rhs.pos), color(rhs.color), texPos(rhs.texPos), normal(rhs.normal){}

};

class VertexOut
{
public:
	Vector4f pos;//世界变换后的坐标
	Vector4f projPos;//投影变换后的坐标
	Vector2f texPos;// 纹理坐标
	Vector4f normal;//法线
	Vector4f color;
	float deepTest; // 1 / z

	VertexOut() = default;
	VertexOut(Vector4f pos, Vector4f projPos, Vector4f normal, Vector2f texPos, Vector4f color, float deep) :
		pos(pos), projPos(projPos), normal(normal), texPos(texPos), color(color), deepTest(deep)
	{
	}

	VertexOut(const VertexOut& rhs) :pos(rhs.pos), projPos(rhs.projPos), texPos(rhs.texPos), normal(rhs.normal),
		color(rhs.color), deepTest(rhs.deepTest)
	{
	}

	VertexOut& operator= (const VertexOut& rhs)
	{
		if (this == &rhs)
			return *this;
		this->normal = rhs.normal;
		this->pos = rhs.pos;
		this->projPos = rhs.projPos;
		this->texPos = rhs.texPos;
		this->color = rhs.color;
		this->deepTest = rhs.deepTest;
		return *this;
	}

	inline VertexOut interpolate(const VertexOut& v2, float t) const
	{
		return VertexOut(
			this->pos.interpolate(v2.pos, t),
			this->projPos.interpolate(v2.projPos, t),
			this->normal.interpolate(v2.normal, t),
			this->texPos.interpolate(v2.texPos, t),			
			this->color.interpolate(v2.color, t),
			MathUtil::interpolate(this->deepTest, v2.deepTest, t)
			);
	}
};

}

#endif