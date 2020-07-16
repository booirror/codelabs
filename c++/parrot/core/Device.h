#ifndef _PARROT_DEVICE_H_
#define _PARROT_DEVICE_H_
#include "Type.h"
#include "Vector.h"
#include "MathUtil.h"

namespace parrot {
class Device {
public:
	Device(int w, int h) : m_width(w), m_height(h), m_pFramebuffer(nullptr)
	{
		m_width = w;
		m_height = h;
		if (w > 0 && h > 0) {
			m_zBuffer = new float[w*h];
		}
	}
	~Device()
	{
		if (m_pFramebuffer) {
			delete m_pFramebuffer;
		}
		if (m_zBuffer) {
			delete m_zBuffer;
		}
	}


	inline void drawPixel(int x, int y, const Vector4f &color)
	{
		m_pFramebuffer[m_width*y + x] = MathUtil::colorToUInt(color);
	}
	float getZ(int x, int y) const
	{
		if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
			return m_zBuffer[y*m_width + x];
		}
		else {
			return 1.0f;
		}
	}
	inline void setZ(int x, int y, float z)
	{
		if (x >= 0 && x < m_width && y >= 0 && y < m_height)
		{
			m_zBuffer[y*m_width+x] = z;
		}
	}
	inline UINT*& getFrameBuffer() { return m_pFramebuffer; }
	inline int getClientWidth() { return m_width; }
	inline int getClientHeight() { return m_height; }
	
	void clearBuffer(const Vector4f &color)
	{
		for (int y = 0; y < m_height; ++y)
		{
			for (int x = 0; x < m_width; ++x)
			{
				m_pFramebuffer[m_width*y + x] = MathUtil::colorToUInt(color);
				m_zBuffer[y*m_width + x] = 0;
			}
		}
	}
private:
	int m_width;
	int m_height;
	UINT* m_pFramebuffer;
	float *m_zBuffer;
};
}
#endif