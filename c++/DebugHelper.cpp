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
	//���к�׺�ж�
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
	//��ȡ��Ļ�ߴ磬��ʼ��һ���յ���Ⱦ�������
	auto renderTexture = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888, GL_DEPTH24_STENCIL8);
	//��ղ���ʼ��ȡ
	renderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
	//���������ڵ�����������RenderTexture��
	Director::getInstance()->getRunningScene()->visit();
	//������ȡ
	renderTexture->end();
	//�����ļ�
	renderTexture->saveToFile(fileName, format);
	if (callback) {
		//ʹ��schedule����һ֡�е���callback����
		auto fullPath = FileUtils::getInstance()->getWritablePath() + fileName;
		auto scheduleCallback = [&, fullPath, callback](float dt) {
			callback(fullPath);
		};
		auto _schedule = Director::getInstance()->getRunningScene()->getScheduler();
		_schedule->schedule(scheduleCallback, this, 0.0f, 0, 0.0f, false, "screenshot");
	}
}

