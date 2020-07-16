#ifndef _DEBUG_HELPER_H_
#define _DEBUG_HELPER_H_

#include "cocos2d.h"

class DebugHelper : public cocos2d::Ref{
public:

	static DebugHelper* getInstance();
	void captureScreenShot(const std::string &file="screenshot", std::function<void(const std::string&)> callback = nullptr);
private:
	int index = 0;
};

#endif