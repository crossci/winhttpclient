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
	~WinHttpClient();
	bool openSession(bool async=false);
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
	static void addRequestHeaders(HINTERNET hRequest, const wstring &header);
	static void addRequestCookies(HINTERNET hRequest, const wstring &Cookies);
	static void setProxy(HINTERNET hRequest,
		const wstring &proxy,
		const wstring &proxyUsername,
		const wstring &proxyPassword);
	void setIEProxy(HINTERNET hRequest, const wstring& requestURL);
	static void disableAutoRedirect(HINTERNET hRequest);
	static bool sendRequest(HINTERNET hRequest,void* param);
	static DWORD sendData(HINTERNET hRequest, LPCVOID data, unsigned int size);
	static void receiveResponse(HINTERNET hRequest);
	static int getStatusCode(HINTERNET hRequest);
	static wstring getResponseCookies(HINTERNET hRequest);
	static int getContentLenght(HINTERNET hRequest);
	static bool hasData(HINTERNET hRequest, DWORD& len);
	static char* readData(HINTERNET hRequest, DWORD len);
	static void setAsyncCallback(HINTERNET hRequest, WINHTTP_STATUS_CALLBACK lpfnInternetCallback);
	void clear();
private:
	static wstring queryHeaders(HINTERNET hRequest, int queryType);
public:
	HINTERNET	m_sessionHandle;
	HINTERNET	m_connect;
};