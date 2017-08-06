#pragma once
#include "..\base\define.h"
#include "..\base\PtrHelper.h"
#include <string>
class DLL_SAMPLE_API MD5Utils
{
public:
	static std::string getFileMD5(const char* filePath);

	static std::string getMD5(const char* str);
};