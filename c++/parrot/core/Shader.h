#ifndef _PARROT_SHADER_H_
#define _PARROT_SHADER_H_
#include "Vertex.h"

namespace parrot
{
class Shader
{
public:
	Shader(){}
	virtual ~Shader() {}

	virtual VertexOut VS(const Vertex& vin)
	{
		VertexOut out;
		out.projPos = vin.pos * mWorldViewProj;

		out.pos = vin.pos * mWorld;
		out.normal = out.normal * mWorldInverseTranspose;

		out.color = vin.color;
		out.texPos = vin.texPos;
		return out;
	}

	virtual Vector4f PS(VertexOut& pin)
	{
		return pin.color;
	}

	void setWorldViewProj(const Matrix& worldViewProj) { mWorldViewProj = worldViewProj; }
	void setWorld(const Matrix& world) { mWorld = world; }
	void setWorldInvTranspose(const Matrix& worldInvTrans){ mWorldInverseTranspose = worldInvTrans; }
private:
	Matrix mWorldViewProj;
	Matrix mWorld;
	Matrix mWorldInverseTranspose;
};

}
#endif