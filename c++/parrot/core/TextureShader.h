#ifndef _PARROT_TEXTURE_SHADER_H_
#define _PARROT_TEXTURE_SHADER_H_
#include "Shader.h"
#include "Texture2D.h"
#include "Light.h"
namespace parrot
{
class TextureShader : public Shader
{
public:
	TextureShader(){}
	virtual ~TextureShader(){}

	virtual VertexOut VS(const Vertex& vin) override
	{
		return Shader::VS(vin);
	}

	virtual Vector4f PS(VertexOut& pin) override
	{
		//单位化法线
		pin.normal.normalize();

		//纹理采样
		Vector4f texColor = mTexture.sample(pin.texPos);

		//顶点到观察点向量
		Vector4f toEye = (mEyePos - pin.pos).normalize();

		//初始化颜色值全部为0
		Vector4f ambient(0.f, 0.f, 0.f, 0.f);
		Vector4f diffuse(0.f, 0.f, 0.f, 0.f);
		Vector4f specular(0.f, 0.f, 0.f, 0.f);

		//光源计算后得到的环境光、漫反射 、高光
		Vector4f A, D, S;

		computeDirectionalLight(mMatrial, mDirLight, pin.normal, toEye, A, D, S);

		ambient = ambient + A;
		diffuse = diffuse + D;
		specular = specular + S;


		//纹理+光照计算公式： 纹理*(环境光+漫反射光)+高光
		Vector4f litColor = texColor * (ambient + diffuse) + specular;

		//最终颜色透明度使用漫反射光的透明度
		litColor.w = mMatrial.diffuse.w * texColor.w;

		return litColor;
	}
	void setEyePos(const Vector4f& epos){ mEyePos = epos; }
	void setTexture(const Texture2D& tex) { mTexture = tex; }
	void setMaterial(const Material& m) { mMatrial = m; }
	void setDirLight(const DirectionalLight& light) { mDirLight = light; }
private:
	Texture2D mTexture;
	Vector4f mEyePos;
	Material mMatrial;
	DirectionalLight mDirLight;
};
}

#endif