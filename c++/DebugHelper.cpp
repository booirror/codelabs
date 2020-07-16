#include "DebugHelper.h"


using namespace cocos2d;

static DebugHelper* _instance = nullptr;

DebugHelper* DebugHelper::getInstance()
{
	if (_instance == nullptr) {
		_instance = new (std::nothrow) DebugHelper();
	}
	return _instance;
}

void DebugHelper::captureScreenShot(const std::string& file, std::function<void(const std::string&)> callback)
{
	Image::Format format;
	//进行后缀判断
	char buff[256];
	sprintf(buff, "%s%d.jpg", file.c_str(), index++);
	std::string fileName = buff;
	if (std::string::npos != fileName.find_last_of(".")) {
		auto extension = fileName.substr(fileName.find_last_of("."), fileName.length());
		if (!extension.compare(".png")) {
			format = Image::Format::PNG;
		}
		else if (!extension.compare(".jpg")) {
			format = Image::Format::JPG;
		}
		else {
			CCLOG("cocos2d: the image can only be saved as JPG or PNG format");
			return;
		}
	}
	else {
		CCLOG("cocos2d: the image can only be saved as JPG or PNG format");
		return;
	}
	auto size = Director::getInstance()->getWinSize();
	//获取屏幕尺寸，初始化一个空的渲染纹理对象
	auto renderTexture = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
	//清空并开始获取
	renderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
	//遍历场景节点对象，填充纹理到RenderTexture中
	Director::getInstance()->getRunningScene()->visit();
	//结束获取
	renderTexture->end();
	//保存文件
	renderTexture->saveToFile(fileName, format);
	if (callback) {
		//使用schedule在下一帧中调用callback函数
		auto fullPath = FileUtils::getInstance()->getWritablePath() + fileName;
		auto scheduleCallback = [&, fullPath, callback](float dt) {
			callback(fullPath);
		};
		auto _schedule = Director::getInstance()->getRunningScene()->getScheduler();
		_schedule->schedule(scheduleCallback, this, 0.0f, 0, 0.0f, false, "screenshot");
	}
}

