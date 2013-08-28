// MultiThreadInternetServer.cpp : �������̨Ӧ�ó������ڵ㡣
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
// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: ���Ĵ�������Է���������Ҫ
			_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
			SOCKET sock;
			initSocket(&sock);
		}
	}
	else
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: GetModuleHandle ʧ��\n"));
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
		printf("�ͻ���IP��ַ��%s\n",inet_ntoa(clinetSockAddress.sin_addr));
	}
	

}

DWORD WINAPI recvThread (PVOID pvParam)
{
	printf ("�����̺߳�:%x\n",GetCurrentThreadId());
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
			printf("%s�������\n",&(result[0]));
	}
	closesocket(*sock);
	
	return ret;
}


