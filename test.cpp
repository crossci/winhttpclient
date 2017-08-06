#include "WinHttpClient.h"
#include <iostream>
#include "stringutils\stringutils.h"
#include "base\PtrHelper.h"
#include "fileutils\FileUtils.h"
using namespace std;

void downloadFile(const char* url)
{
	string surl = url;
	wstring wurl = stringutils::s2ws(surl);
	WinHttpClient httpClient;
	httpClient.openSession();
	URL_COMPONENTS urlComp;
	wchar_t szHostName[MAX_PATH] = L"";
	wchar_t szURLPath[MAX_PATH * 5] = L"";
	httpClient.CrackUrl(wurl, urlComp, szHostName, szURLPath);
	httpClient.getConnect(urlComp.lpszHostName, urlComp.nPort);
	HINTERNET hRequest = httpClient.openRequest(L"GET", urlComp.lpszUrlPath,urlComp.nScheme);
	//httpClient.setProxy(hRequest,L"127.0.0.1:8087",L"",L"");
	if (hRequest)
	{
		httpClient.sendRequest(hRequest);
		httpClient.receiveResponse(hRequest);
		int statusCode = httpClient.getStatusCode(hRequest);
		if (statusCode == 200)
		{
			wcout << httpClient.getContentLenght(hRequest) << endl;
			DWORD len;
			FILE* pFile = FileUtils::openFile("httpdownload.html", "wb");
			if (pFile)
			{
				while (httpClient.hasData(hRequest, len))
				{
					CPtrHelper<char> readData = httpClient.readData(hRequest, len);
					fwrite(readData, 1, len, pFile);
				}
				fclose(pFile);
			}
		}
	}
}
int main(int argc,char* argv[])
{

	downloadFile("http://127.0.0.1/index.txt");
	system("pause");
}

