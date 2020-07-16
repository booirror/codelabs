#include <memory>
#include "BoxDemo.h"
#include "WindowWrapper.h"
int main()
{
	using namespace parrot;

	auto wnd = WindowWrapper::getInstance();
	wnd->setApp(std::dynamic_pointer_cast<App>(std::make_shared<BoxDemo>()));
	wnd->createWindow("hello", 800, 600);
	wnd->run();
	return 0;
}