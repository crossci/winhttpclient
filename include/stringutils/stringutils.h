#include <string>
#include "..\base\define.h"

class DLL_SAMPLE_API stringutils
{
public:
	static void converToUpper(std::string& src);

	static void converToLower(std::string& src);

	static std::string ws2s(const std::wstring& ws);

	static std::wstring s2ws(const std::string& s);
};