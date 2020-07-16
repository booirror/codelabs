#ifndef _PARROT_TEXTURE2D_H_
#define _PARROT_TEXTURE2D_H_
#include <cstring>
#include <algorithm>
#include "Type.h"
#include "Vector.h"
namespace parrot
{
class Texture2D
{
public:
	Texture2D():m_width(0), m_height(0), m_pixelBuffer(nullptr) {}
	Texture2D(UINT width, UINT height) :m_width(width), m_height(height), m_pixelBuffer(nullptr)
	{
		if (width == 0 || height == 0) return;
		m_pixelBuffer = new Vector4f*[m_height];
		for (int i = 0; i < m_height; i++) {
			m_pixelBuffer[i] = new Vector4f[m_width];
			memset(m_pixelBuffer[i], 0, m_width*sizeof(Vector4f));
		}
	}
	~Texture2D()
	{
		if (m_pixelBuffer != nullptr) {
			for (int i = 0; i < m_height; i++) {
				delete[] m_pixelBuffer[i];
			}
			delete[] m_pixelBuffer;
			m_pixelBuffer = nullptr;
		}
	}

	Texture2D(const Texture2D& rhs) : m_width(rhs.m_width), m_height(rhs.m_height), m_pixelBuffer(nullptr)
	{
		if (m_width > 0 && m_height > 0) {
			m_pixelBuffer = new Vector4f*[m_height];
			for (int i = 0; i < m_height; i++) {
				m_pixelBuffer[i] = new Vector4f[m_width];
				memcpy(this->m_pixelBuffer[i], rhs.m_pixelBuffer[i], m_width*sizeof(Vector4f));
			}
		}
	}

	Texture2D& operator=(const Texture2D& rhs)
	{
		if (this == &rhs) return *this;
		Texture2D temp(rhs);
		this->m_width = rhs.m_width;
		this->m_height = rhs.m_height;
		std::swap(m_pixelBuffer, temp.m_pixelBuffer);
		return *this;
	}

	//²ÉÑù
	Vector4f sample(const Vector2f& tex)
	{
		if (tex.u >= 0 && tex.u <= 1 && tex.v >= 0 && tex.v <= 1)
		{
			UINT x = tex.u * (m_width - 1);// 0 <= x <= m_width-1
			UINT y = tex.v * (m_height - 1);
			return m_pixelBuffer[y][x];
		}
		else
		{
			float u, v;
			if (tex.u > 1)
				u = tex.u - static_cast<int>(tex.u);
			else if (tex.u < 0)
				u = (static_cast<int>(-tex.u) + 1) + tex.u;
			if (tex.v > 1)
				v = tex.v - static_cast<int>(tex.v);
			else if (tex.v < 0)
				v = (static_cast<int>(-tex.v) + 1) + tex.v;
			UINT x = u * (m_width - 1);
			UINT y = v * (m_height - 1);
			return m_pixelBuffer[y][x];
		}
	}

	void setColor(int x, int y, const Vector4f& v)
	{
		m_pixelBuffer[y][x] = v;
	}
private:
	UINT m_width;
	UINT m_height;
	Vector4f* *m_pixelBuffer;
};
}

#endif