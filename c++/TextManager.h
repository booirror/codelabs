#ifndef _TEXT_MANAGER_H
#define _TEXT_MANAGER_H

#include "cocos2d.h"

class TextManager {
public:
	TextManager();
	~TextManager();

	static TextManager* getInstance();
	const char* getString(const std::string& key);
private:

	cocos2d::Dictionary *dict;
};

#define gtext TextManager::getInstance()->getString

#endif