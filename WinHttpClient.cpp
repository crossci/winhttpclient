#include "WinHttpClient.h"

WinHttpClient::WinHttpClient()
	:m_sessionHandle(NULL),
	m_connect(NULL)
{
	clear();
}

WinHttpClient::~WinHttpClient()
{
	clear();
}
bool WinHttpClient::openSession(bool async)
{
	if (m_sessionHandle == NULL)
	{
		int flag = 0;
		if (async)
			flag = WINHTTP_FLAG_ASYNC;
		m_sessionHandle = ::WinHttpOpen(L"WinHttpClient",
			WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
			WINHTTP_NO_PROXY_NAME,
			WINHTTP_NO_PROXY_BYPASS,
			flag);
		if (m_sessionHandle == NULL)
		{
			//m_dwLastError = ::GetLastError();
			return false;
		}
	}
	setTimeouts();

	return true;
}

bool WinHttpClient::setTimeouts(unsigned int resolveTimeout,
	unsigned int connectTimeout,
	unsigned int sendTimeout,
	unsigned int receiveTimeout)
{

	::WinHttpSetTimeouts(m_sessionHandle,
		resolveTimeout,
		connectTimeout,
		sendTimeout,
		receiveTimeout);
	return true;
}
bool WinHttpClient::CrackUrl(const wstring &url,
	URL_COMPONENTS& urlComp,
	wchar_t* szHostName,
	wchar_t* szURLPath)
{
	memset(&urlComp, 0, sizeof(urlComp));
	urlComp.dwStructSize = sizeof(urlComp);
	urlComp.lpszHostName = szHostName;
	urlComp.dwHostNameLength = MAX_PATH;
	urlComp.lpszUrlPath = szURLPath;
	urlComp.dwUrlPathLength = MAX_PATH * 5;
	urlComp.dwSchemeLength = 1; // None zero

	if (::WinHttpCrackUrl(url.c_str(), url.size(), 0, &urlComp))
	{
		return true;
	}
	return false;
}
bool WinHttpClient::getConnect(const LPWSTR szHostName, INTERNET_PORT nPort)
{
	if (m_connect == NULL)
	{
		m_connect = ::WinHttpConnect(m_sessionHandle, szHostName, nPort, 0);
		if (m_connect != NULL)
		{
			return true;
		}
	}
	return false;
}

HINTERNET WinHttpClient::openRequest(const wstring &action,const LPWSTR path, INTERNET_SCHEME nScheme)
{
	DWORD dwOpenRequestFlag = (nScheme == INTERNET_SCHEME_HTTPS) ? WINHTTP_FLAG_SECURE : 0;
	HINTERNET hRequest = NULL;
	hRequest = ::WinHttpOpenRequest(m_connect,
		action.c_str(),
		path,
		NULL,
		WINHTTP_NO_REFERER,
		WINHTTP_DEFAULT_ACCEPT_TYPES,
		dwOpenRequestFlag);
	if (nScheme == INTERNET_SCHEME_HTTPS)
	{
		DWORD options = SECURITY_FLAG_IGNORE_CERT_CN_INVALID
			| SECURITY_FLAG_IGNORE_CERT_DATE_INVALID
			| SECURITY_FLAG_IGNORE_UNKNOWN_CA;
		::WinHttpSetOption(hRequest,
			WINHTTP_OPTION_SECURITY_FLAGS,
			(LPVOID)&options,
			sizeof(DWORD));
	}
	return hRequest;
}
void WinHttpClient::addRequestHeaders(HINTERNET hRequest,const wstring &header)
{
	if (header.size() > 0)
	{
		::WinHttpAddRequestHeaders(hRequest, header.c_str(), header.size(), WINHTTP_ADDREQ_FLAG_COALESCE_WITH_SEMICOLON);
	}
}
void WinHttpClient::addRequestCookies(HINTERNET hRequest, const wstring &Cookies)
{
	if (Cookies.size() > 0)
	{
		wstring cookies1 = L"Cookie: ";
		cookies1 += Cookies;
		::WinHttpAddRequestHeaders(hRequest, cookies1.c_str(), cookies1.size(), WINHTTP_ADDREQ_FLAG_COALESCE_WITH_SEMICOLON);
	}
}

void WinHttpClient::setProxy(HINTERNET hRequest,
	const wstring &proxy,
	const wstring &proxyUsername,
	const wstring &proxyPassword)
{
	if (proxy.size() > 0)
	{
		WINHTTP_PROXY_INFO proxyInfo;
		memset(&proxyInfo, 0, sizeof(proxyInfo));
		proxyInfo.dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
		wchar_t szProxy[MAX_PATH] = L"";
		wcscpy_s(szProxy, MAX_PATH, proxy.c_str());
		proxyInfo.lpszProxy = szProxy;

		::WinHttpSetOption(hRequest, WINHTTP_OPTION_PROXY, &proxyInfo, sizeof(proxyInfo));

		if (proxyUsername.size() > 0)
		{
			WinHttpSetOption(hRequest, WINHTTP_OPTION_PROXY_USERNAME, (LPVOID)proxyUsername.c_str(), proxyUsername.size() * sizeof(wchar_t));
			if (proxyPassword.size() > 0)
			{
				::WinHttpSetOption(hRequest, WINHTTP_OPTION_PROXY_PASSWORD, (LPVOID)proxyPassword.c_str(), proxyPassword.size() * sizeof(wchar_t));
			}
		}
	}
}
void WinHttpClient::setIEProxy(HINTERNET hRequest,const wstring& requestURL)
{
	WINHTTP_CURRENT_USER_IE_PROXY_CONFIG proxyConfig;
	memset(&proxyConfig, 0, sizeof(proxyConfig));
	if (::WinHttpGetIEProxyConfigForCurrentUser(&proxyConfig))
	{
		if (proxyConfig.lpszAutoConfigUrl != NULL)
		{
			WINHTTP_AUTOPROXY_OPTIONS autoProxyOptions;
			memset(&autoProxyOptions, 0, sizeof(autoProxyOptions));
			autoProxyOptions.dwFlags = WINHTTP_AUTOPROXY_AUTO_DETECT | WINHTTP_AUTOPROXY_CONFIG_URL;
			autoProxyOptions.dwAutoDetectFlags = WINHTTP_AUTO_DETECT_TYPE_DHCP;
			autoProxyOptions.lpszAutoConfigUrl = proxyConfig.lpszAutoConfigUrl;
			autoProxyOptions.fAutoLogonIfChallenged = TRUE;
			autoProxyOptions.dwReserved = 0;
			autoProxyOptions.lpvReserved = NULL;

			WINHTTP_PROXY_INFO proxyInfo;
			memset(&proxyInfo, 0, sizeof(proxyInfo));

			if (::WinHttpGetProxyForUrl(m_sessionHandle, requestURL.c_str(), &autoProxyOptions, &proxyInfo))
			{
				if (::WinHttpSetOption(hRequest, WINHTTP_OPTION_PROXY, &proxyInfo, sizeof(proxyInfo)))
				{
				}
				if (proxyInfo.lpszProxy != NULL)
				{
					::GlobalFree(proxyInfo.lpszProxy);
				}
				if (proxyInfo.lpszProxyBypass != NULL)
				{
					::GlobalFree(proxyInfo.lpszProxyBypass);
				}
			}
		}
		else if (proxyConfig.lpszProxy != NULL)
		{
			WINHTTP_PROXY_INFO proxyInfo;
			memset(&proxyInfo, 0, sizeof(proxyInfo));
			proxyInfo.dwAccessType = WINHTTP_ACCESS_TYPE_NAMED_PROXY;
			wchar_t szProxy[MAX_PATH] = L"";
			wcscpy_s(szProxy, MAX_PATH, proxyConfig.lpszProxy);
			proxyInfo.lpszProxy = szProxy;

			if (proxyConfig.lpszProxyBypass != NULL)
			{
				wchar_t szProxyBypass[MAX_PATH] = L"";
				wcscpy_s(szProxyBypass, MAX_PATH, proxyConfig.lpszProxyBypass);
				proxyInfo.lpszProxyBypass = szProxyBypass;
			}
			::WinHttpSetOption(hRequest, WINHTTP_OPTION_PROXY, &proxyInfo, sizeof(proxyInfo));
		}

		if (proxyConfig.lpszAutoConfigUrl != NULL)
		{
			::GlobalFree(proxyConfig.lpszAutoConfigUrl);
		}
		if (proxyConfig.lpszProxy != NULL)
		{
			::GlobalFree(proxyConfig.lpszProxy);
		}
		if (proxyConfig.lpszProxyBypass != NULL)
		{
			::GlobalFree(proxyConfig.lpszProxyBypass);
		}
	}
}
void WinHttpClient::disableAutoRedirect(HINTERNET hRequest)
{
	DWORD dwDisableFeature = WINHTTP_DISABLE_REDIRECTS;
	::WinHttpSetOption(hRequest, WINHTTP_OPTION_DISABLE_FEATURE, &dwDisableFeature, sizeof(dwDisableFeature));
}
bool WinHttpClient::sendRequest(HINTERNET hRequest, void* param)
{
	if (::WinHttpSendRequest(hRequest,
		WINHTTP_NO_ADDITIONAL_HEADERS,
		0,
		WINHTTP_NO_REQUEST_DATA,
		0,
		0,
		(DWORD_PTR)param))
	{
		return true;
	}
	return false;
}
DWORD WinHttpClient::sendData(HINTERNET hRequest, LPCVOID data, unsigned int size)
{
	DWORD dwWritten = 0;
	if (!::WinHttpWriteData(hRequest,
		data,
		size,
		&dwWritten))
	{
		return dwWritten;
	}
	return 0;
}
void WinHttpClient::receiveResponse(HINTERNET hRequest)
{
	::WinHttpReceiveResponse(hRequest, NULL);
}

int WinHttpClient::getStatusCode(HINTERNET hRequest)
{
	wstring ret = queryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE);
	if (ret.size() > 0)
	{
		return _wtoi(ret.c_str());
	}
	return 0;
}
wstring WinHttpClient::getResponseCookies(HINTERNET hRequest)
{
	return queryHeaders(hRequest, WINHTTP_QUERY_SET_COOKIE);
}
int WinHttpClient::getContentLenght(HINTERNET hRequest)
{
	wstring ret = queryHeaders(hRequest, WINHTTP_QUERY_CONTENT_LENGTH);
	if (ret.size() > 0)
	{
		return _wtoi(ret.c_str());
	}
	return 0;
}
bool WinHttpClient::hasData(HINTERNET hRequest, DWORD& len)
{
	::WinHttpQueryDataAvailable(hRequest, &len);
	return len > 0;
}
char* WinHttpClient::readData(HINTERNET hRequest, DWORD len)
{
	char* ret = new char[len];
	if (ret)
	{
		DWORD dwRead = 0;
		if (::WinHttpReadData(hRequest,
			ret,
			len,
			&dwRead))
		{
			return ret;
		}
	}
	return ret;
}

void WinHttpClient::setAsyncCallback(HINTERNET hRequest, WINHTTP_STATUS_CALLBACK lpfnInternetCallback)
{
	::WinHttpSetStatusCallback(hRequest, lpfnInternetCallback, WINHTTP_CALLBACK_FLAG_ALL_NOTIFICATIONS, 0);
}
wstring WinHttpClient::queryHeaders(HINTERNET hRequest, int queryType)
{
	DWORD dwSize = 0;
	BOOL bResult = FALSE;
	wstring ret = L"";
	bResult = ::WinHttpQueryHeaders(hRequest,
		queryType,
		WINHTTP_HEADER_NAME_BY_INDEX,
		NULL,
		&dwSize,
		WINHTTP_NO_HEADER_INDEX);
	if (bResult || (!bResult && (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)))
	{
		wchar_t *szHeader = new wchar_t[dwSize];
		if (szHeader != NULL)
		{
			memset(szHeader, 0, dwSize * sizeof(wchar_t));
			if (::WinHttpQueryHeaders(hRequest,
				queryType,
				WINHTTP_HEADER_NAME_BY_INDEX,
				szHeader,
				&dwSize,
				WINHTTP_NO_HEADER_INDEX))
			{
				ret.assign(szHeader);
			}
			delete[] szHeader;
		}
	}
	return ret;
}

void WinHttpClient::clear()
{
	if (m_sessionHandle != NULL)
	{
		::WinHttpCloseHandle(m_sessionHandle);
	}
	if (m_connect)
	{
		::WinHttpCloseHandle(m_connect);
	}
}