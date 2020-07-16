#pragma once
#include <windows.h>
#include "core/Type.h"
#include "core/Device.h"
#include "core/DeviceContext.h"
#include "core/Shader.h"
#include "core/MathUtil.h"
#include "core/Texture2D.h"
#include "core/Vertex.h"
#include "core/Vector.h"
#include "core/Light.h"
#include "core/TextureShader.h"
#include "App.h"
//using namespace parrot;

class GeometryGenerator
{
private:
	GeometryGenerator() {}

public:
	//单例模式
	static GeometryGenerator* GetInstance()
	{
		static GeometryGenerator instance;
		return &instance;
	}

	//基本网络结构：顶点集合+索引集合
	struct MeshData
	{
		std::vector<parrot::Vertex>	vertices;
		std::vector<parrot::UINT>	indices;
	};

	//创建一个立方体：指定宽(X方向)、高(Y方向)、深(Z方向)
	void CreateBox(float width, float height, float depth, MeshData &mesh)
	{
		using namespace parrot;
		mesh.vertices.clear();
		mesh.indices.clear();

		//一共24个顶点(每面4个)
		mesh.vertices.resize(24);
		//一共36个索引(每面6个)
		mesh.indices.resize(36);

		float halfW = width * 0.5f;
		float halfH = height * 0.5f;
		float halfD = depth * 0.5f;

		//眼睛面向z轴正方向
		//构建顶点
		//前面
		mesh.vertices[0].pos = Vector4f(-halfW, -halfH, -halfD, 1.f);
		mesh.vertices[0].normal = Vector4f(0.f, 0.f, -1.f);
		mesh.vertices[0].color = Vector4f(1.f, 0.f, 0.f, 1.f);
		mesh.vertices[0].texPos = Vector2f(0.f, 1.f);
		mesh.vertices[1].pos = Vector4f(-halfW, halfH, -halfD, 1.f);
		mesh.vertices[1].normal = Vector4f(0.f, 0.f, -1.f);
		mesh.vertices[1].color = Vector4f(0.f, 0.f, 0.f, 1.f);
		mesh.vertices[1].texPos = Vector2f(0.f, 0.f);
		mesh.vertices[2].pos = Vector4f(halfW, halfH, -halfD, 1.f);
		mesh.vertices[2].normal = Vector4f(0.f, 0.f, -1.f);
		mesh.vertices[2].color = Vector4f(1.f, 0.f, 0.f, 1.f);
		mesh.vertices[2].texPos = Vector2f(1.f, 0.f);
		mesh.vertices[3].pos = Vector4f(halfW, -halfH, -halfD, 1.f);
		mesh.vertices[3].normal = Vector4f(0.f, 0.f, -1.f);
		mesh.vertices[3].color = Vector4f(0.f, 1.f, 0.f, 1.f);
		mesh.vertices[3].texPos = Vector2f(1.f, 1.f);
		//左侧面
		mesh.vertices[4].pos = Vector4f(-halfW, -halfH, halfD, 1.f);
		mesh.vertices[4].normal = Vector4f(-1.f, 0.f, 0.f);
		mesh.vertices[4].color = Vector4f(0.f, 0.f, 1.f, 1.f);
		mesh.vertices[4].texPos = Vector2f(0.f, 1.f);
		mesh.vertices[5].pos = Vector4f(-halfW, halfH, halfD, 1.f);
		mesh.vertices[5].normal = Vector4f(-1.f, 0.f, 0.f);
		mesh.vertices[5].color = Vector4f(1.f, 1.f, 0.f, 1.f);
		mesh.vertices[5].texPos = Vector2f(0.f, 0.f);
		mesh.vertices[6].pos = Vector4f(-halfW, halfH, -halfD, 1.f);
		mesh.vertices[6].normal = Vector4f(-1.f, 0.f, 0.f);
		mesh.vertices[6].color = Vector4f(0.f, 0.f, 0.f, 1.f);
		mesh.vertices[6].texPos = Vector2f(1.f, 0.f);
		mesh.vertices[7].pos = Vector4f(-halfW, -halfH, -halfD, 1.f);
		mesh.vertices[7].normal = Vector4f(-1.f, 0.f, 0.f);
		mesh.vertices[7].color = Vector4f(1.f, 1.f, 1.f, 1.f);
		mesh.vertices[7].texPos = Vector2f(1.f, 1.f);
		//背面
		mesh.vertices[8].pos = Vector4f(halfW, -halfH, halfD, 1.f);
		mesh.vertices[8].normal = Vector4f(0.f, 0.f, 1.f);
		mesh.vertices[8].color = Vector4f(1.f, 0.f, 1.f, 1.f);
		mesh.vertices[8].texPos = Vector2f(0.f, 1.f);
		mesh.vertices[9].pos = Vector4f(halfW, halfH, halfD, 1.f);
		mesh.vertices[9].normal = Vector4f(0.f, 0.f, 1.f);
		mesh.vertices[9].color = Vector4f(0.f, 1.f, 1.f, 1.f);
		mesh.vertices[9].texPos = Vector2f(0.f, 0.f);
		mesh.vertices[10].pos = Vector4f(-halfW, halfH, halfD, 1.f);
		mesh.vertices[10].normal = Vector4f(0.f, 0.f, 1.f);
		mesh.vertices[10].color = Vector4f(1.f, 1.f, 0.f, 1.f);
		mesh.vertices[10].texPos = Vector2f(1.f, 0.f);
		mesh.vertices[11].pos = Vector4f(-halfW, -halfH, halfD, 1.f);
		mesh.vertices[11].normal = Vector4f(0.f, 0.f, 1.f);
		mesh.vertices[11].color = Vector4f(0.f, 0.f, 1.f, 1.f);
		mesh.vertices[11].texPos = Vector2f(1.f, 1.f);
		//右侧面
		mesh.vertices[12].pos = Vector4f(halfW, -halfH, -halfD, 1.f);
		mesh.vertices[12].normal = Vector4f(1.f, 0.f, 0.f);
		mesh.vertices[12].color = Vector4f(0.f, 1.f, 0.f, 1.f);
		mesh.vertices[12].texPos = Vector2f(0.f, 1.f);
		mesh.vertices[13].pos = Vector4f(halfW, halfH, -halfD, 1.f);
		mesh.vertices[13].normal = Vector4f(1.f, 0.f, 0.f);
		mesh.vertices[13].color = Vector4f(1.f, 0.f, 0.f, 1.f);
		mesh.vertices[13].texPos = Vector2f(0.f, 0.f);
		mesh.vertices[14].pos = Vector4f(halfW, halfH, halfD, 1.f);
		mesh.vertices[14].normal = Vector4f(1.f, 0.f, 0.f);
		mesh.vertices[14].color = Vector4f(0.f, 1.f, 1.f, 1.f);
		mesh.vertices[14].texPos = Vector2f(1.f, 0.f);
		mesh.vertices[15].pos = Vector4f(halfW, -halfH, halfD, 1.f);
		mesh.vertices[15].normal = Vector4f(1.f, 0.f, 0.f);
		mesh.vertices[15].color = Vector4f(1.f, 0.f, 1.f, 1.f);
		mesh.vertices[15].texPos = Vector2f(1.f, 1.f);
		//上面
		mesh.vertices[16].pos = Vector4f(-halfW, halfH, -halfD, 1.f);
		mesh.vertices[16].normal = Vector4f(0.f, 1.f, 0.f);
		mesh.vertices[16].color = Vector4f(0.f, 0.f, 0.f, 1.f);
		mesh.vertices[16].texPos = Vector2f(0.f, 1.f);
		mesh.vertices[17].pos = Vector4f(-halfW, halfH, halfD, 1.f);
		mesh.vertices[17].normal = Vector4f(0.f, 1.f, 0.f);
		mesh.vertices[17].color = Vector4f(1.f, 1.f, 0.f, 1.f);
		mesh.vertices[17].texPos = Vector2f(0.f, 0.f);
		mesh.vertices[18].pos = Vector4f(halfW, halfH, halfD, 1.f);
		mesh.vertices[18].normal = Vector4f(0.f, 1.f, 0.f);
		mesh.vertices[18].color = Vector4f(0.f, 1.f, 1.f, 1.f);
		mesh.vertices[18].texPos = Vector2f(1.f, 0.f);
		mesh.vertices[19].pos = Vector4f(halfW, halfH, -halfD, 1.f);
		mesh.vertices[19].normal = Vector4f(0.f, 1.f, 0.f);
		mesh.vertices[19].color = Vector4f(1.f, 0.f, 0.f, 1.f);
		mesh.vertices[19].texPos = Vector2f(1.f, 1.f);
		//底面
		mesh.vertices[20].pos = Vector4f(-halfW, -halfH, halfD, 1.f);
		mesh.vertices[20].normal = Vector4f(0.f, -1.f, 0.f);
		mesh.vertices[20].color = Vector4f(0.f, 0.f, 1.f, 1.f);
		mesh.vertices[20].texPos = Vector2f(0.f, 1.f);
		mesh.vertices[21].pos = Vector4f(-halfW, -halfH, -halfD, 1.f);
		mesh.vertices[21].normal = Vector4f(0.f, -1.f, 0.f);
		mesh.vertices[21].color = Vector4f(1.f, 1.f, 1.f, 1.f);
		mesh.vertices[21].texPos = Vector2f(0.f, 0.f);
		mesh.vertices[22].pos = Vector4f(halfW, -halfH, -halfD, 1.f);
		mesh.vertices[22].normal = Vector4f(0.f, -1.f, 0.f);
		mesh.vertices[22].color = Vector4f(0.f, 1.f, 0.f, 1.f);
		mesh.vertices[22].texPos = Vector2f(1.f, 0.f);
		mesh.vertices[23].pos = Vector4f(halfW, -halfH, halfD, 1.f);
		mesh.vertices[23].normal = Vector4f(0.f, -1.f, 0.f);
		mesh.vertices[23].color = Vector4f(1.f, 0.f, 1.f, 1.f);
		mesh.vertices[23].texPos = Vector2f(1.f, 1.f);

		//构建索引
		mesh.indices[0] = 0;
		mesh.indices[1] = 1;
		mesh.indices[2] = 2;
		mesh.indices[3] = 0;
		mesh.indices[4] = 2;
		mesh.indices[5] = 3;

		mesh.indices[6] = 4;
		mesh.indices[7] = 5;
		mesh.indices[8] = 6;
		mesh.indices[9] = 4;
		mesh.indices[10] = 6;
		mesh.indices[11] = 7;

		mesh.indices[12] = 8;
		mesh.indices[13] = 9;
		mesh.indices[14] = 10;
		mesh.indices[15] = 8;
		mesh.indices[16] = 10;
		mesh.indices[17] = 11;

		mesh.indices[18] = 12;
		mesh.indices[19] = 13;
		mesh.indices[20] = 14;
		mesh.indices[21] = 12;
		mesh.indices[22] = 14;
		mesh.indices[23] = 15;

		mesh.indices[24] = 16;
		mesh.indices[25] = 17;
		mesh.indices[26] = 18;
		mesh.indices[27] = 16;
		mesh.indices[28] = 18;
		mesh.indices[29] = 19;

		mesh.indices[30] = 20;
		mesh.indices[31] = 21;
		mesh.indices[32] = 22;
		mesh.indices[33] = 20;
		mesh.indices[34] = 22;
		mesh.indices[35] = 23;
	}
};


class BoxDemo : public parrot::App
{
public:
	BoxDemo();
	~BoxDemo();
public:
	bool Init(HINSTANCE hInstance, HWND hWnd);
	void Update(float dt);
	void Render();
	void Clear();

	//鼠标事件控制
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
public:
	inline parrot::Device* GetDevice() { return m_pDevice; }
private:
	int							m_width, m_height;
	HINSTANCE					m_hInstance;
	HWND							m_hWnd;

	//parrot::Device*				m_pDevice;
	parrot::DeviceContext*			m_pImmediateContext;
	parrot::TextureShader*					m_pShader;

	parrot::Matrix						m_worldViewProj;			//世界视角投影矩阵
	parrot::Matrix						m_world;					//世界变换矩阵
	parrot::Matrix						m_worldInvTranspose;		//世界变换逆矩阵的转置 用于调整法线

	std::vector<parrot::Vertex>		m_vertices;				//顶点缓冲
	std::vector<parrot::UINT>			m_indices;				//索引缓冲

	GeometryGenerator::MeshData	m_box;

	parrot::Texture2D					m_tex;					//纹理

	parrot::Material				m_material;				//材质
	parrot::DirectionalLight		m_dirLight;				//平行光源

	//控制摄像机位置角度等
	float						m_theta;
	float						m_phi;
	float						m_radius;
	POINT						m_lastMousePos;
};

