// test_Winhttp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <Winhttp.h>
#include <WinInet.h>
void test_winHttp();
void httpConnect();
void httpWriteData();
int WINAPI Dumper(HWND hX, int intCtrlID, HINTERNET hResource);
#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "Winhttp.lib")
int _tmain(int argc, _TCHAR* argv[])
{
// 	test_winHttp();
// 	httpConnect();
	httpWriteData;
	return 0;
}

void test_winHttp()
{


	URL_COMPONENTS urlComp;
	LPCWSTR pwszUrl1 = 
		L"http://search.msn.com/results.asp?RS=CHECKED&FORM=MSNH&v=1&q=wininet";
	DWORD dwUrlLen = 0;

	// Initialize the URL_COMPONENTS structure.
	ZeroMemory(&urlComp, sizeof(urlComp));
	urlComp.dwStructSize = sizeof(urlComp);

	// Set required component lengths to non-zero, 
	// so that they are cracked.
	urlComp.dwSchemeLength    = (DWORD)-1;
	urlComp.dwHostNameLength  = (DWORD)-1;
	urlComp.dwUrlPathLength   = (DWORD)-1;
	urlComp.dwExtraInfoLength = (DWORD)-1;

	// Crack the URL.
	if (!WinHttpCrackUrl( pwszUrl1, (DWORD)wcslen(pwszUrl1), 0, &urlComp))
	{
		printf("Error %u in WinHttpCrackUrl.\n", GetLastError());
	}
	else
	{
		// Change the search data. New data is the same length.
		urlComp.lpszExtraInfo = L"?RS=CHECKED&FORM=MSNH&v=1&q=winhttp";

		// Obtain the size of the new URL and allocate memory.
		WinHttpCreateUrl( &urlComp, 0, NULL, &dwUrlLen);
		LPWSTR pwszUrl2 = new WCHAR[dwUrlLen];

		// Create a new URL.
		if(!WinHttpCreateUrl( &urlComp, 0, pwszUrl2, &dwUrlLen))
		{
			printf( "Error %u in WinHttpCreateUrl.\n", GetLastError());
		}
		else
		{
			// Show both URLs.
			printf( "Old URL:  %S\nNew URL:  %S\n", pwszUrl1, pwszUrl2);
		}

		// Free allocated memory.
		delete [] pwszUrl2;
	}
}

void httpConnect()
{

	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL, 
		hConnect = NULL,
		hRequest = NULL;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen( L"WinHTTP Example/1.0",  
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME, 
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect( hSession, L"www.microsoft.com",
		INTERNET_DEFAULT_HTTPS_PORT, 0);

	// Create an HTTP request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest( hConnect, L"GET", NULL,
		NULL, WINHTTP_NO_REFERER, 
		WINHTTP_DEFAULT_ACCEPT_TYPES, 
		WINHTTP_FLAG_SECURE);

	// Send a request.
	if (hRequest)
		bResults = WinHttpSendRequest( hRequest,
		WINHTTP_NO_ADDITIONAL_HEADERS,
		0, WINHTTP_NO_REQUEST_DATA, 0, 
		0, 0);


	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse( hRequest, NULL);

	// Keep checking for data until there is nothing left.
	if (bResults)
		do 
		{
			// Check for available data.
			dwSize = 0;
			if (!WinHttpQueryDataAvailable( hRequest, &dwSize))
				printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());

			// Allocate space for the buffer.
			pszOutBuffer = new char[dwSize+1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				dwSize=0;
			}
			else
			{
				// Read the Data.
				ZeroMemory(pszOutBuffer, dwSize+1);

				if (!WinHttpReadData( hRequest, (LPVOID)pszOutBuffer, 
					dwSize, &dwDownloaded))
					printf( "Error %u in WinHttpReadData.\n", GetLastError());
				else
					printf( "%s\n", pszOutBuffer);

				// Free the memory allocated to the buffer.
				delete [] pszOutBuffer;
			}

		} while (dwSize > 0);


		// Report any errors.
		if (!bResults)
			printf("Error %d has occurred.\n", GetLastError());

		// Close any open handles.
		if (hRequest) WinHttpCloseHandle(hRequest);
		if (hConnect) WinHttpCloseHandle(hConnect);
		if (hSession) WinHttpCloseHandle(hSession);
}

void httpWriteData()
{
	LPSTR pszData = "WinHttpWriteData Example";
	DWORD dwBytesWritten = 0;
	BOOL  bResults = FALSE;
	HINTERNET hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(  L"A WinHTTP Example Program/1.0", 
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME, 
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect( hSession, L"www.wingtiptoys.com",
		INTERNET_DEFAULT_HTTP_PORT, 0);

	// Create an HTTP Request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest( hConnect, L"PUT", 
		L"/writetst.txt", 
		NULL, WINHTTP_NO_REFERER, 
		WINHTTP_DEFAULT_ACCEPT_TYPES, 
		0);

	// Send a Request.
	if (hRequest) 
		bResults = WinHttpSendRequest( hRequest, 
		WINHTTP_NO_ADDITIONAL_HEADERS,
		0, WINHTTP_NO_REQUEST_DATA, 0, 
		(DWORD)strlen(pszData), 0);

	// Write data to the server.
	if (bResults)
		bResults = WinHttpWriteData( hRequest, pszData, 
		(DWORD)strlen(pszData), 
		&dwBytesWritten);

	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse( hRequest, NULL);

	// Report any errors.
	if (!bResults)
		printf("Error %d has occurred.\n",GetLastError());


	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
}

int WINAPI Dumper(HWND hX, int intCtrlID, HINTERNET hResource)
{
	LPTSTR lpszData;    // buffer for the data
	DWORD  dwSize;       // size of the data available
	DWORD  dwDownloaded; // size of the downloaded data
	DWORD  dwSizeSum=0;  // size of the data in the textbox
	LPTSTR lpszHolding;  // buffer to merge the textbox data and buffer

	// Set the cursor to an hourglass.
	SetCursor(LoadCursor(NULL,IDC_WAIT));

	// This loop handles reading the data.
	do
	{
		// The call to InternetQueryDataAvailable determines the
		// amount of data available to download.
		if (!InternetQueryDataAvailable(hResource,&dwSize,0,0))
		{
			printf("InternetQueryDataAvailable failed (%d)\n", GetLastError());
			SetCursor(LoadCursor(NULL,IDC_ARROW));
			return FALSE;
		}
		else
		{
			// Allocate a buffer of the size returned by
			// InternetQueryDataAvailable.
			lpszData = new TCHAR[dwSize+1];

			// Read the data from the HINTERNET handle.
			if(!InternetReadFile(hResource,
				(LPVOID)lpszData,
				dwSize,
				&dwDownloaded))
			{
				printf("InternetReadFile failed (%d)\n", GetLastError());
				delete[] lpszData;
				break;
			}
			else
			{
				// Add a null terminator to the end of the data buffer
				lpszData[dwDownloaded]='\0';

				// Allocate the holding buffer.
				lpszHolding = new TCHAR[dwSizeSum + dwDownloaded + 1];

				// Check if there has been any data written
				// to the textbox.
				if (dwSizeSum != 0)
				{
					// Retrieve the data stored in the textbox if any
					GetDlgItemText(hX,intCtrlID,
						(LPTSTR)lpszHolding,
						dwSizeSum);

					// Add a null terminator at the end of the
					// textbox data.
					lpszHolding[dwSizeSum]='\0';
				}
				else
				{
					// Make the holding buffer an empty string.
					lpszHolding[0]='\0';
				}

				size_t cchDest = dwSizeSum + dwDownloaded + dwDownloaded + 1;
				LPTSTR* ppszDestEnd = 0;
				size_t* pcchRemaining = 0;

				// Add the new data to the holding buffer
				HRESULT hr = StringCchCatEx(lpszHolding,
					cchDest,
					lpszData,
					ppszDestEnd,
					pcchRemaining,
					STRSAFE_NO_TRUNCATION);

				if(SUCCEEDED(hr))
				{
					// Write the holding buffer to the textbox.
					SetDlgItemText(hX,intCtrlID,(LPTSTR)lpszHolding);

					// Delete the two buffers.
					delete[] lpszHolding;
					delete[] lpszData;

					// Add the size of the downloaded data to the
					// textbox data size.
					dwSizeSum = dwSizeSum + dwDownloaded + 1;

					// Check the size of the remaining data.
					// If it is zero, break.
					if (dwDownloaded == 0)
						break;
					else
					{
						//  TODO: Insert error handling code here.
					}
				}
			}
		}
	}
	while(TRUE);

	// Close the HINTERNET handle.
	InternetCloseHandle(hResource);

	// Set the cursor back to an arrow.
	SetCursor(LoadCursor(NULL,IDC_ARROW));

	return TRUE;
}
