#pragma once
#pragma comment(lib, "Winhttp.lib")
#include <string>
#include <comutil.h>
//#include <windows.h>
#include <Winhttp.h>
using namespace std;
class WinHttpClient
{
public:
	WinHttpClient();
	bool openSession();
	bool setTimeouts(unsigned int resolveTimeout = 0,
		unsigned int connectTimeout = 60000,
		unsigned int sendTimeout = 30000,
		unsigned int receiveTimeout = 30000);
	bool CrackUrl(const wstring &url,
		URL_COMPONENTS& urlComp,
		wchar_t* szHostName,
		wchar_t* szURLPath);
	bool getConnect(const LPWSTR szHostName, INTERNET_PORT nPort);
	HINTERNET openRequest(const wstring &action, const LPWSTR path, INTERNET_SCHEME nScheme);
	void addRequestHeaders(HINTERNET hRequest,const wstring &header);
	void addRequestCookies(HINTERNET hRequest, const wstring &Cookies);
	void setProxy(HINTERNET hRequest,
		const wstring &proxy,
		const wstring &proxyUsername,
		const wstring &proxyPassword);
	void setIEProxy(HINTERNET hRequest, const wstring& requestURL);
	void disableAutoRedirect(HINTERNET hRequest);
	bool sendRequest(HINTERNET hRequest);
	DWORD sendData(HINTERNET hRequest, LPCVOID data,unsigned int size);
	void receiveResponse(HINTERNET hRequest);
	int getStatusCode(HINTERNET hRequest);
	wstring getResponseCookies(HINTERNET hRequest);
	int getContentLenght(HINTERNET hRequest);
	bool hasData(HINTERNET hRequest, DWORD& len);
	char* readData(HINTERNET hRequest, DWORD len);
private:
	wstring queryHeaders(HINTERNET hRequest,int queryType);
public:
	HINTERNET	m_sessionHandle;
	HINTERNET	m_connect;
};