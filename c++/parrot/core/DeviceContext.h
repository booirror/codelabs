#ifndef _DEVICE_CONTEXT_H_
#define _DEVICE_CONTEXT_H_
#include <vector>
#include <algorithm>
#include "Device.h"
#include "Vertex.h"
#include "Shader.h"

namespace parrot
{
	enum RENDER_MODE
	{
		WIREFRAME,
		SOLIDE,
	};

	class DeviceContext
	{
	public:
		DeviceContext() :mMode(WIREFRAME), mCameraPos(Vector4f(0.f, 0.f, 0.f))
		{

		}

		~DeviceContext()
		{

		}


	public:
		void init(Device* pdevice) { mpDevice = pdevice; }
		void setRenderMode(RENDER_MODE mode){ mMode = mode; }
		void setVertexBuffer(const std::vector<Vertex>& vertices) { mVertices = vertices; }
		void setCameraPos(const Vector4f& pos) { mCameraPos = pos; }
		void setIndexBuffer(std::vector<UINT> indices){ mIndices = indices; }
		void setShader(Shader* shader){ mpShader = shader; }
		void drawIndexed(UINT indexCount, UINT startIndexLocation, UINT startVertexLocation);

	private:
		void toCVV(VertexOut& v)
		{
			v.projPos.x /= v.projPos.w;
			v.projPos.y /= v.projPos.w;
			v.projPos.z /= v.projPos.w;
			v.projPos.w = 1;
		}
		bool clip(const VertexOut& v);
		VertexOut transformToProj(const Vertex& v);
		void transformToScreen(const Matrix& m, VertexOut& v)
		{
			v.projPos = v.projPos * m;
		}
		bool backfaceCulling(const Vertex& v1, const Vertex& v2, const Vertex& v3);
		void bresenhamDrawLine(int x1, int y1, int x2, int y2);
		void scanlineFill(const VertexOut& left, const VertexOut& right, int yindex);
		void drawTriangle(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);
		void drawTriangleTop(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);
		void drawTriangleBottom(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);
		void rasterization(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);
	private:
		Device* mpDevice;
		RENDER_MODE mMode;
		std::vector<Vertex> mVertices;
		std::vector<UINT> mIndices;
		Shader* mpShader;
		Vector4f mCameraPos;
	};
}

#endif