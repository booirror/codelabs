#ifndef _WINDOW_WRAPPER_H_
#define _WINDOW_WRAPPER_H_
#include <windows.h>
#include <windowsx.h>
#include <string>
#include <memory>
#include "App.h"
namespace parrot {

class WindowWrapper {
public:
	WindowWrapper() = default;
	static WindowWrapper* getInstance();
	HWND createWindow(const char*name, int width, int height);
	void setApp(const std::shared_ptr<App> &sapp) { app = sapp; }
	void run();
public:
	HWND ghWnd;
	HINSTANCE g_hInstance;
	std::shared_ptr<App> app;
	std::string g_title;
	int g_width;
	int g_height;
};

}

#endif