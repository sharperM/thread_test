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

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	STARTUPINFO si = {sizeof (si)};
//	PROCESS_INFORMATION pi;
//	DWORD dwExitCode;
//	TCHAR szCommandLine[] = TEXT("WORDPAD C:\\Documents and Settings\\Administrator\\桌面\\麦绍培\\Notepad2_CN\\License.txt");
//	bool bIsSucess = CreateProcess(TEXT("C:\\Documents and Settings\\Administrator\\桌面\\麦绍培\\Notepad2_CN\\Notepad2.exe"),szCommandLine,NULL,NULL,
//		FALSE,0,NULL,NULL,&si,&pi);
//// 	TCHAR szCommandLine[] = TEXT("WORDPAD C:\\Documents and Settings\\Administrator\\桌面\\麦绍培\\Notepad2_CN\\License.txt");
//// 	CreateProcess(TEXT("D:\\My Documents\\Visual Studio 2010\\Projects\\thread_test\\Debug\\thread_test.exe"),szCommandLine,NULL,NULL,
//// 	FALSE,0,NULL,NULL,&si,&pi);
//	std::cout << pi.dwProcessId << std::endl;
//	if ( bIsSucess )
//	{
//		CloseHandle(pi.hThread);
//		CloseHandle(pi.hProcess);
//// 		WaitForSingleObject(pi.hProcess,INFINITE);
//// 		GetExitCodeProcess(pi.hProcess, &dwExitCode);		
//	}
//	return 0;
//}


int _tmain(int argc, _TCHAR* argv[])
{
	LoadLibrary(TEXT("Ws2_32.lib"));

	int x = 0;
	UINT nThreadId1,nThreadId2;
	DWORD dwThreadId1,dwThreadId2 ;
	HANDLE hThread1, hThread2, hThread3, hThread4;
// 	hThread1 = CreateThread(NULL,0,ThreadFunc1,(PVOID)&x, 0, &dwThreadId1);
	
	hThread2 = CreateThread(NULL,0,ThreadFunc2,(PVOID)&x, 0, &dwThreadId2);
	WaitForSingleObject( hThread2, INFINITE );

// 	CloseHandle(hThread1);
	CloseHandle(hThread2);
	malloc(100);
	std::string* pStr = new std::string();
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
	sa.sin_port				= htons (IPPORT_TIMESERVER);
	sa.sin_addr.S_un.S_addr = inet_addr("132.163.135.130");
	SOCKET sock				= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	connect( sock, (SOCKADDR *)&sa, sizeof(sa));
	unsigned long			ulTime;
	recv (sock, (char *) &ulTime, 4, MSG_PEEK) ; 
	int iSize = recv (sock, (char *) &ulTime, 4, 0) ; 
	ulTime = ntohl (ulTime) ;
	_CrtDumpMemoryLeaks();
	return 0;
}