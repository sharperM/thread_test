// thread_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <TlHelp32.h >
/*#include <WinNT.h>*/
#include <iostream>
#include <process.h>
#include <string>
#include <winsock.h>
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <shlwapi.h >
#pragma comment(lib, "Ws2_32.lib")

long g_x = 0;
using namespace std;
DWORD WINAPI ThreadFunc1 (PVOID pvParam)
{
	for (int i = 0;i<50000;i++)
	{
		++g_x;
	}
	return 0;
}

DWORD WINAPI ThreadFunc2 (PVOID pvParam)
{
	for (int i = 0;i<50000;i++)
	{
		++g_x;
	}
	char str[] ="- This, a sample string.zhou";
	printf ("%s\n",str);
	char * pch;
	printf ("Splitting string \"%s\" into tokens:\n",str);
	pch = strtok (str," ,.-");
	while (pch != NULL)
	{
		printf ("%s\n",pch);
		pch = strtok (NULL, " ,.-");
	}
	printf ("%s\n",str);
	free(malloc(1000));
	return 0;
}
UINT WINAPI ThreadFunc3 (PVOID pvParam)
{
	for (int i = 0;i<50000;i++)
	{
		++g_x;
	}	
// 	cout<<(TCHAR*)TEXT("线程id")<<GetCurrentThreadId()<<endl;
	printf ("%x\n",GetCurrentThreadId());
	_endthreadex( 0 );
	return 0;
}
DWORD WINAPI SendThread (PVOID pvParam)
{
	char buf[100]  ;
	memcpy(buf,"我在打酱油，酱油，油…………………………%d\n",100);
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct sockaddr_in serviceAddr;
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	serviceAddr.sin_family = AF_INET;
	serviceAddr.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	serviceAddr.sin_port = htons( 27015 );
	connect( ConnectSocket, (SOCKADDR*) &serviceAddr, sizeof(serviceAddr) );
	if (SOCKET_ERROR == send(ConnectSocket,(const char*)buf,sizeof(buf),0))
		return 0;
	shutdown(ConnectSocket,1);
	SOCKET * pSock = (SOCKET*) pvParam;
	
	closesocket(ConnectSocket);
	return 0;
}

VOID SuspenProcess(DWORD dwProcessID, BOOL fSuspend)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,dwProcessID);

	if ( hSnapshot != INVALID_HANDLE_VALUE )
	{
		THREADENTRY32 te = {sizeof (te)};
		BOOL fOk = Thread32First(hSnapshot, &te );
		for (; fOk; fOk = Thread32Next(hSnapshot, &te))
		{
			HANDLE hThread = OpenThread( THREAD_SUSPEND_RESUME, FALSE, te.th32ThreadID);
			if ( hThread != NULL )
			{
				if (fSuspend)
				{
					SuspendThread(hThread);
				}else
					ResumeThread(hThread);
			}
			CloseHandle(hThread);
		}
		CloseHandle(hSnapshot);
	}
}



int _tmain(int argc, _TCHAR* argv[])
{
	int x = 0;
	UINT nThreadId1,nThreadId2;
	DWORD dwThreadId1,dwThreadId2 ;
	HANDLE hThread1, hThread2, hThread3, hThread4;
// 	hThread1 = CreateThread(NULL,0,ThreadFunc1,(PVOID)&x, 0, &dwThreadId1);
	
	hThread2 = CreateThread(NULL,0,ThreadFunc2,(PVOID)&x, 0, &dwThreadId2);
	WaitForSingleObject( hThread2, INFINITE );

// 	CloseHandle(hThread1);
	CloseHandle(hThread2);
	std::string* pStr = new std::string();
	delete pStr;
	hThread3 =(HANDLE) _beginthreadex(NULL,0,&ThreadFunc3,(PVOID)&x,0 , &nThreadId1);
	WaitForSingleObject( hThread3, INFINITE );
	CloseHandle(hThread3);
	hThread4 =(HANDLE) _beginthreadex(NULL,0,&ThreadFunc3,(PVOID)&x,0 , &nThreadId2);
	WaitForSingleObject( hThread4, INFINITE );
	CloseHandle(hThread4);
	std::cout << g_x << std::endl;

	WORD wVersionRequested;

	WSADATA wsaData;

	int err;   

	err = wVersionRequested = MAKEWORD( 2, 0 ); 


	WSAStartup( wVersionRequested, &wsaData );

	struct sockaddr_in sa;
	sa.sin_family			= AF_INET;
	sa.sin_port				= htons (27015);
	sa.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	SOCKET sock				= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	connect( sock, (SOCKADDR *)&sa, sizeof(sa));
	unsigned long			ulTime;
	for (int i = 0; i<100; ++i)
	{
		CloseHandle(CreateThread(NULL,0,SendThread,(PVOID)&i, 0, &dwThreadId2));

// 		SendThread(&i);
		printf("发送第%d次\n",i);

// 		Sleep(100);
		
	}
	closesocket(sock);
// 	int iSize = recv (sock, (char *) &ulTime, 4, 0) ; 
// 	ulTime = ntohl (ulTime) ;
	
	_CrtDumpMemoryLeaks();
	return 0;
}