#include "WinHttpClient.h"
#include <iostream>
#include "stringutils\stringutils.h"
#include "base\PtrHelper.h"
#include "fileutils\FileUtils.h"
using namespace std;
int size = 0;
FILE* pFile = FileUtils::openFile("httpdownload.html", "wb");
void CALLBACK RequesterStatusCallback(
	HINTERNET hInternet,
	DWORD_PTR dwContext,
	DWORD dwInternetStatus,
	LPVOID lpvStatusInformation,
	DWORD dwStatusInformationLength
	)
{
	WinHttpClient* client = (WinHttpClient*)dwContext;

	cout << "Status = " << dwInternetStatus << endl;
	int statusCode = 0;
	DWORD len = 0;
	CPtrHelper<char> readData = NULL;
	switch (dwInternetStatus)
	{
	case WINHTTP_CALLBACK_FLAG_SENDREQUEST_COMPLETE:
		WinHttpClient::receiveResponse(hInternet);
		statusCode = WinHttpClient::getStatusCode(hInternet);
		if (statusCode == 200)
		{
			wcout << WinHttpClient::getContentLenght(hInternet) << endl;
			WinHttpClient::hasData(hInternet, len);
			/*FILE* pFile = FileUtils::openFile("httpdownload.html", "wb");
			if (pFile)
			{
				while (WinHttpClient::hasData(hInternet, len))
				{
					CPtrHelper<char> readData = WinHttpClient::readData(hInternet, len);
					fwrite(readData, 1, len, pFile);
				}
				fclose(pFile);
			}*/
		}
		break;
	case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE:
		len = *(int*)lpvStatusInformation;
		if (len > 0)
		{
			readData = WinHttpClient::readData(hInternet, len);
			size += len;
			fwrite(readData, 1, len, pFile);
		}
		else
		{
			WinHttpCloseHandle(hInternet);
			delete client;
			fclose(pFile);
		}
		cout << "size = " << size << ",len = " << len << endl;
		//pRequester->OnHeadersAvailable();
		break;
	case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
		WinHttpClient::hasData(hInternet, len);
		//pRequester->OnReadComplete(dwStatusInformationLength);
		break;
	case WINHTTP_CALLBACK_STATUS_REQUEST_ERROR:
		//pRequester->OnRequestError((LPWINHTTP_ASYNC_RESULT)lpvStatusInformation);
		break;
	case WINHTTP_CALLBACK_STATUS_HANDLE_CLOSING:
		//pRequester->OnHandleClosing(hInternet);
		//pRequester->m_hRequest = NULL;
		
		break;
	default:
		;
	}
}
void downloadFile(const char* url)
{
	string surl = url;
	wstring wurl = stringutils::s2ws(surl);
	WinHttpClient* httpClient = new WinHttpClient();
	httpClient->openSession(true);
	URL_COMPONENTS urlComp;
	wchar_t szHostName[MAX_PATH] = L"";
	wchar_t szURLPath[MAX_PATH * 5] = L"";
	httpClient->CrackUrl(wurl, urlComp, szHostName, szURLPath);
	httpClient->getConnect(urlComp.lpszHostName, urlComp.nPort);
	HINTERNET hRequest = httpClient->openRequest(L"GET", urlComp.lpszUrlPath, urlComp.nScheme);
	//httpClient.setProxy(hRequest,L"127.0.0.1:8087",L"",L"");
	if (hRequest)
	{
		httpClient->setAsyncCallback(hRequest, RequesterStatusCallback);
		httpClient->sendRequest(hRequest, httpClient);
	/*	httpClient.receiveResponse(hRequest);
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
		}*/
	}
}
int main(int argc,char* argv[])
{

	downloadFile("http://192.168.1.17:8090/API/Game/ToDayGameLog?id=2256&pageindex=1&pagesize=10&key=string");
	system("pause");
}

