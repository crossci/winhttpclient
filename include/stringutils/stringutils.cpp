#include "stringutils.h"
#include <algorithm>
void stringutils::converToUpper(std::string& src)
{
	transform(src.begin(), src.end(), src.begin(), toupper);
}

void stringutils::converToLower(std::string& src)
{
	transform(src.begin(), src.end(), src.begin(), tolower);
}

std::string stringutils::ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL);					// curLocale = "C";
	setlocale(LC_ALL, "chs");                                           //���ĵ���
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = ws.size() + 1;										//��Ҫת���Ŀ��ַ�������1
	char *_Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);											//��ʼ��������
	size_t   i;
	wcstombs_s(&i, _Dest, _Dsize, _Source, ws.size());
	std::string result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, curLocale.c_str());								//���û�ԭ����locale
	return result;
}

std::wstring stringutils::s2ws(const std::string& s)
{
	std::string curlLocale = setlocale(LC_ALL, NULL);
	setlocale(LC_ALL, "chs");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;

	wchar_t* _Dest = new wchar_t[_Dsize];
	size_t i;
	mbstowcs_s(&i, _Dest, _Dsize, _Source, s.size());
	std::wstring result = _Dest;
	delete[] _Dest;
	setlocale(LC_ALL, curlLocale.c_str());
	return result;
}
