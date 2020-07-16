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
		//��λ������
		pin.normal.normalize();

		//�������
		Vector4f texColor = mTexture.sample(pin.texPos);

		//���㵽�۲������
		Vector4f toEye = (mEyePos - pin.pos).normalize();

		//��ʼ����ɫֵȫ��Ϊ0
		Vector4f ambient(0.f, 0.f, 0.f, 0.f);
		Vector4f diffuse(0.f, 0.f, 0.f, 0.f);
		Vector4f specular(0.f, 0.f, 0.f, 0.f);

		//��Դ�����õ��Ļ����⡢������ ���߹�
		Vector4f A, D, S;

		computeDirectionalLight(mMatrial, mDirLight, pin.normal, toEye, A, D, S);

		ambient = ambient + A;
		diffuse = diffuse + D;
		specular = specular + S;


		//����+���ռ��㹫ʽ�� ����*(������+�������)+�߹�
		Vector4f litColor = texColor * (ambient + diffuse) + specular;

		//������ɫ͸����ʹ����������͸����
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