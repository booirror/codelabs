#ifndef _PARROT_BITMAP_LOADER_H
#define _PARROT_BITMAP_LOADER_H
#include <string>
#include "Texture2D.h"
namespace parrot
{

class BitmapLoader{
public:
	Texture2D loadBitmapToColorArray(const std::wstring& filePath);
};
}
#endif