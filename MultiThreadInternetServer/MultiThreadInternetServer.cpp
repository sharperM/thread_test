// MultiThreadInternetServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MultiThreadInternetServer.h"
#include <winsock2.h>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma comment(lib, "Ws2_32.lib")


void initSocket(SOCKET *sock /*out*/);
DWORD WINAPI recvThread (PVOID pvParam);
// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 更改错误代码以符合您的需要
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: 在此处为应用程序的行为编写代码。
			SOCKET sock;
			initSocket(&sock);
		}
	}
	else
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

	return nRetCode;
}



void initSocket(SOCKET *sock /*out*/)
{
	WORD wVersionRequested;

	WSADATA wsaData;

	int err;   
	wVersionRequested = MAKEWORD( 2, 0 );
	if (WSAStartup( wVersionRequested, &wsaData )!= NO_ERROR)
	{
		return;
	}
	struct sockaddr_in serverSockAddress;
	serverSockAddress.sin_family			= AF_INET;
	serverSockAddress.sin_port				= htons (27015);
	serverSockAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if ((*sock= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))==INVALID_SOCKET)
		return;
	if (bind(*sock,(SOCKADDR*)&serverSockAddress,sizeof(serverSockAddress))== SOCKET_ERROR)
	{
		wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
		return;
	}
	if (listen(*sock,100)== SOCKET_ERROR)
		return;
	struct sockaddr_in clinetSockAddress;
	int length  = sizeof(clinetSockAddress);
	SOCKET connetSocket;
	DWORD dwThreadId;
	while(INVALID_SOCKET!=(connetSocket = accept(*sock, (SOCKADDR*)&clinetSockAddress,&length)))
	{
		CreateThread(NULL,0,recvThread,(PVOID)&connetSocket, 0, &dwThreadId);
		printf("客户端IP地址：%s\n",inet_ntoa(clinetSockAddress.sin_addr));
	}
	

}

DWORD WINAPI recvThread (PVOID pvParam)
{
	printf ("接收线程号:%x\n",GetCurrentThreadId());
	SOCKET * sock =(SOCKET *) pvParam;
	char buf[1] = {0};
	std::vector<char> result;
	int ret ;
	while((ret = recv(*sock,(char*)buf,1,0/*MSG_OOB*/))>0)
	{
		result.push_back(*buf);
	}

	if (ret<0)
	{
		wprintf(L"recv failed with error: %ld\n", WSAGetLastError());
	}else if (ret == 0)
	{
		if (result.size()>0)
			printf("%s接收完成\n",&(result[0]));
	}
	closesocket(*sock);
	
	return ret;
}


