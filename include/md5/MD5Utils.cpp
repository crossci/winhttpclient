#include "MD5Utils.h"
#include "md5.h"
#include "..\fileutils\FileUtils.h"
#define BUFSIZE 1024*16
std::string MD5Utils::getFileMD5(const char* filePath)
{
	MD5 md5;
	static unsigned char buf[BUFSIZE];
	int readCount = 0;
	FILE* file = FileUtils::openFile(filePath, "rb");
	if (file)
	{
		for (;;) {
			readCount = fread(buf, 1, BUFSIZE, file);
			if (readCount <= 0)
				break;
			md5.update(buf, readCount);
		}
		md5.finalize();
		return md5.hexdigest();
	}
	return "";
}

std::string MD5Utils::getMD5(const char* str)
{
	MD5 md5(str);
	return md5.hexdigest();
}
