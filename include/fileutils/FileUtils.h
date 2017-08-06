#pragma once
#include "..\base\define.h"
#include <vector>
#include "..\base\PtrHelper.h"
class DLL_SAMPLE_API FileUtils
{
public:
	static bool splitFileName(const char* fileName, char* name, char* ext);

	static void getTimeArray(std::vector<int>& vector);

	static void GetCurrentDir(char* curDir);

	static void generateFullPath(const char* fileName,char* pathName);

	static FILE* openFile(const char* fileName, const char* mode);

	static int getFileLen(const char* fileName);

	static CPtrHelper<char> getBinFile(const char* fileName, int& fileLen);

	static CPtrHelper<char> getTXTFile(const char* fileName, int& fileLen);

	static CPtrHelper<char> getFile(const char* fileName, int& fileLen,const char* readType="rb");

	static bool writeToFile(const char* fileName, const char* data, int dataLen, const char* writeType);
};
