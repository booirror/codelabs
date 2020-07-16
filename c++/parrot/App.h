#ifndef _PARROT_APP_H
#define _PARROT_APP_H
#include "core/Device.h"
namespace parrot{
class App {
public:
	App() = default;
	virtual ~App(){}
	virtual bool Init(HINSTANCE hInstance, HWND hWnd)=0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual void Clear() = 0;

	//鼠标事件控制
	virtual void OnMouseDown(WPARAM btnState, int x, int y){}
	virtual void OnMouseUp(WPARAM btnState, int x, int y){}
	virtual void OnMouseMove(WPARAM btnState, int x, int y){}

	inline Device* GetDevice() { return m_pDevice; }
public:
	Device* m_pDevice;
};
}
#endif