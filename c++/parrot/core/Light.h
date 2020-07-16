#ifndef _LIGHT_H_
#define _LIGHT_H_


#include <windows.h>
#include <algorithm>
#include "MathUtil.h"

namespace parrot {
	//平行光
	struct DirectionalLight
	{
		DirectionalLight() { ZeroMemory(this, sizeof(this)); }

		Vector4f ambient;	//环境光
		Vector4f diffuse;	//漫反射光
		Vector4f specular;	//高光
		Vector4f direction;	//光照方向
	};

	//点光源
	struct PointLight
	{
		PointLight() { ZeroMemory(this, sizeof(this)); }

		Vector4f ambient;
		Vector4f diffuse;
		Vector4f specular;

		Vector4f position;//光源位置

		Vector4f att;     //衰减系数

		float range;      //光照范围
	};
	//聚光灯
	struct SpotLight
	{
		SpotLight() { ZeroMemory(this, sizeof(this)); }

		Vector4f ambient;
		Vector4f diffuse;
		Vector4f specular;

		Vector4f position;	//光照位置

		Vector4f direction;	//光照方向

		Vector4f att;		//衰减系数
		float range;			//光照范围
		float spot;			//光照强度系数   

	};

	//材质
	struct Material
	{
		Material() { ZeroMemory(this, sizeof(this)); }

		Vector4f ambient;
		Vector4f diffuse;
		Vector4f specular;//w表示高光强度
		Vector4f reflect;
	};

	//计算平行光
	inline void computeDirectionalLight(
		const Material& mat,				//材质
		const DirectionalLight& L,		//平行光
		Vector4f normal,					//顶点法线
		Vector4f toEye,					//顶点到眼睛的向量
		Vector4f& ambient,				//计算结果:环境光
		Vector4f& diffuse,				//计算结果:漫反射光
		Vector4f& spec)					//计算结果:高光
	{
		// 结果初始化为0
		ambient = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
		diffuse = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
		spec = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);

		// 光线方向
		Vector4f lightVec = -L.direction;
		// 环境光直接计算
		ambient = mat.ambient * L.ambient;

		// 计算漫反射系数
		//光线、法线方向归一化
		lightVec.normalize();
		float diffuseFactor = lightVec.dot(normal);
		// 顶点背向光源不再计算

		if (diffuseFactor > 0.0f)
		{
			//入射光线关于法线的反射向量
			Vector4f v = MathUtil::reflect(-lightVec, normal);

			float specFactor = pow(max(v.dot(toEye), 0.0f), mat.specular.w);

			//计算漫反射光
			diffuse = mat.diffuse * L.diffuse * diffuseFactor;
			//计算高光
			spec = mat.specular * L.specular * specFactor;
		}
	}

	//计算点光源
	inline void computePointLight(
		const Material& mat,        //材质
		PointLight L,				//点光源
		Vector4f pos,				//顶点位置
		Vector4f normal,				//顶点法线
		Vector4f toEye,				//顶点到眼睛的向量
		Vector4f& ambient,			//计算结果:环境光
		Vector4f& diffuse,			//计算结果:漫反射光
		Vector4f& spec)				//计算结果:高光
	{
		ambient = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
		diffuse = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
		spec = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);

		//光照方向：顶点到光源
		Vector4f lightVec = L.position - pos;

		//顶点到光源距离
		float d = lightVec.length();

		//超过范围不再计算
		if (d > L.range)
			return;

		//归一化光照方向
		lightVec = lightVec * (1.f / d);

		//计算环境光
		ambient = mat.ambient * L.ambient;

		//漫反射系数
		float diffuseFactor = lightVec.dot(normal);

		if (diffuseFactor > 0.0f)
		{
			Vector4f v = MathUtil::reflect(-lightVec, normal);
			float specFactor = pow(max(v.dot(toEye), 0.0f), mat.specular.w);
			//计算漫反射光
			diffuse = mat.diffuse * L.diffuse * diffuseFactor;
			//计算高光
			spec = mat.specular * L.specular * specFactor;
		}

		// 计算衰减
		float att = 1.f / L.att.dot(Vector4f(1.f, d, d*d));
		diffuse = diffuse * att;
		spec = diffuse * att;
	}

	//计算聚光灯
	inline void  computeSpotLight(
		const Material& mat,            //材质
		const SpotLight& L,				//聚光灯
		Vector4f pos,					//顶点位置
		Vector4f normal,					//顶点法线
		Vector4f toEye,					//顶点到眼睛向量
		Vector4f& ambient,				//计算结果:环境光
		Vector4f& diffuse,				//计算结果:漫反射光
		Vector4f& spec)					//计算结果:高光
	{
		//初始化结果
		ambient = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
		diffuse = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
		spec = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);

		//光照方向：顶点到光源
		Vector4f lightVec = L.position - pos;

		//顶点到光源距离
		float d = lightVec.length();

		//距离大于光照方向不再计算
		if (d > L.range)
			return;

		//归一化光照方向
		lightVec = lightVec * (1.f / d);

		//计算环境光
		ambient = mat.ambient * L.ambient;
		//计算漫反射系数
		float diffuseFactor = lightVec.dot(normal);

		if (diffuseFactor > 0.0f)
		{
			Vector4f v = MathUtil::reflect(-lightVec, normal);
			float specFactor = pow(max(v.dot(toEye), 0.0f), mat.specular.w);
			//漫反射光
			diffuse = mat.diffuse * L.diffuse * diffuseFactor;
			//高光
			spec = mat.specular * L.specular * specFactor;
		}

		//聚光衰减系数
		float spot = pow(max(-lightVec.dot(L.direction), 0.0f), L.spot);
		//衰减系数
		float att = spot / L.att.dot(Vector4f(1.0f, d, d*d));
		ambient = ambient * spot;
		diffuse = diffuse * att;
		spec = spec * att;
	}
}


#endif