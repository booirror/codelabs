#ifndef _LIGHT_H_
#define _LIGHT_H_


#include <windows.h>
#include <algorithm>
#include "MathUtil.h"

namespace parrot {
	//ƽ�й�
	struct DirectionalLight
	{
		DirectionalLight() { ZeroMemory(this, sizeof(this)); }

		Vector4f ambient;	//������
		Vector4f diffuse;	//�������
		Vector4f specular;	//�߹�
		Vector4f direction;	//���շ���
	};

	//���Դ
	struct PointLight
	{
		PointLight() { ZeroMemory(this, sizeof(this)); }

		Vector4f ambient;
		Vector4f diffuse;
		Vector4f specular;

		Vector4f position;//��Դλ��

		Vector4f att;     //˥��ϵ��

		float range;      //���շ�Χ
	};
	//�۹��
	struct SpotLight
	{
		SpotLight() { ZeroMemory(this, sizeof(this)); }

		Vector4f ambient;
		Vector4f diffuse;
		Vector4f specular;

		Vector4f position;	//����λ��

		Vector4f direction;	//���շ���

		Vector4f att;		//˥��ϵ��
		float range;			//���շ�Χ
		float spot;			//����ǿ��ϵ��   

	};

	//����
	struct Material
	{
		Material() { ZeroMemory(this, sizeof(this)); }

		Vector4f ambient;
		Vector4f diffuse;
		Vector4f specular;//w��ʾ�߹�ǿ��
		Vector4f reflect;
	};

	//����ƽ�й�
	inline void computeDirectionalLight(
		const Material& mat,				//����
		const DirectionalLight& L,		//ƽ�й�
		Vector4f normal,					//���㷨��
		Vector4f toEye,					//���㵽�۾�������
		Vector4f& ambient,				//������:������
		Vector4f& diffuse,				//������:�������
		Vector4f& spec)					//������:�߹�
	{
		// �����ʼ��Ϊ0
		ambient = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
		diffuse = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
		spec = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);

		// ���߷���
		Vector4f lightVec = -L.direction;
		// ������ֱ�Ӽ���
		ambient = mat.ambient * L.ambient;

		// ����������ϵ��
		//���ߡ����߷����һ��
		lightVec.normalize();
		float diffuseFactor = lightVec.dot(normal);
		// ���㱳���Դ���ټ���

		if (diffuseFactor > 0.0f)
		{
			//������߹��ڷ��ߵķ�������
			Vector4f v = MathUtil::reflect(-lightVec, normal);

			float specFactor = pow(max(v.dot(toEye), 0.0f), mat.specular.w);

			//�����������
			diffuse = mat.diffuse * L.diffuse * diffuseFactor;
			//����߹�
			spec = mat.specular * L.specular * specFactor;
		}
	}

	//������Դ
	inline void computePointLight(
		const Material& mat,        //����
		PointLight L,				//���Դ
		Vector4f pos,				//����λ��
		Vector4f normal,				//���㷨��
		Vector4f toEye,				//���㵽�۾�������
		Vector4f& ambient,			//������:������
		Vector4f& diffuse,			//������:�������
		Vector4f& spec)				//������:�߹�
	{
		ambient = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
		diffuse = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
		spec = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);

		//���շ��򣺶��㵽��Դ
		Vector4f lightVec = L.position - pos;

		//���㵽��Դ����
		float d = lightVec.length();

		//������Χ���ټ���
		if (d > L.range)
			return;

		//��һ�����շ���
		lightVec = lightVec * (1.f / d);

		//���㻷����
		ambient = mat.ambient * L.ambient;

		//������ϵ��
		float diffuseFactor = lightVec.dot(normal);

		if (diffuseFactor > 0.0f)
		{
			Vector4f v = MathUtil::reflect(-lightVec, normal);
			float specFactor = pow(max(v.dot(toEye), 0.0f), mat.specular.w);
			//�����������
			diffuse = mat.diffuse * L.diffuse * diffuseFactor;
			//����߹�
			spec = mat.specular * L.specular * specFactor;
		}

		// ����˥��
		float att = 1.f / L.att.dot(Vector4f(1.f, d, d*d));
		diffuse = diffuse * att;
		spec = diffuse * att;
	}

	//����۹��
	inline void  computeSpotLight(
		const Material& mat,            //����
		const SpotLight& L,				//�۹��
		Vector4f pos,					//����λ��
		Vector4f normal,					//���㷨��
		Vector4f toEye,					//���㵽�۾�����
		Vector4f& ambient,				//������:������
		Vector4f& diffuse,				//������:�������
		Vector4f& spec)					//������:�߹�
	{
		//��ʼ�����
		ambient = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
		diffuse = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
		spec = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);

		//���շ��򣺶��㵽��Դ
		Vector4f lightVec = L.position - pos;

		//���㵽��Դ����
		float d = lightVec.length();

		//������ڹ��շ����ټ���
		if (d > L.range)
			return;

		//��һ�����շ���
		lightVec = lightVec * (1.f / d);

		//���㻷����
		ambient = mat.ambient * L.ambient;
		//����������ϵ��
		float diffuseFactor = lightVec.dot(normal);

		if (diffuseFactor > 0.0f)
		{
			Vector4f v = MathUtil::reflect(-lightVec, normal);
			float specFactor = pow(max(v.dot(toEye), 0.0f), mat.specular.w);
			//�������
			diffuse = mat.diffuse * L.diffuse * diffuseFactor;
			//�߹�
			spec = mat.specular * L.specular * specFactor;
		}

		//�۹�˥��ϵ��
		float spot = pow(max(-lightVec.dot(L.direction), 0.0f), L.spot);
		//˥��ϵ��
		float att = spot / L.att.dot(Vector4f(1.0f, d, d*d));
		ambient = ambient * spot;
		diffuse = diffuse * att;
		spec = spec * att;
	}
}


#endif