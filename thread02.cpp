// thread02.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<windows.h>

HANDLE sema1;
HANDLE sema2;
 int x;
 int y = 4;

DWORD WINAPI thread1(LPVOID parameter)
{
  
   while(true)
   {
	   WaitForSingleObject(sema1, INFINITE);
           printf("thread1 get sema1 !\n");
	   printf("请输入一个整数：\n");
	   scanf("%d",&x);
	   Sleep(1000);
	   printf("thread1 send sema2 !\n");
	   ReleaseSemaphore(sema2, 1, NULL);
   }
   return 0;
}

DWORD WINAPI thread2(LPVOID parameter)
{
   while(true)
   {
	   WaitForSingleObject(sema2, INFINITE);
           printf("thread2 get sema2 !\n");
	   printf("%d\n",x + y);
	   Sleep(1000);
	   printf("thread2 send sema1 !\n");
	   ReleaseSemaphore(sema1, 1, NULL);
   }
   return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD dwid;
	sema1 = CreateSemaphore(NULL, 1, 1, NULL);
	sema2 = CreateSemaphore(NULL, 0, 1, NULL);
	CreateThread(NULL, 0, thread1, NULL, 0, &dwid);
	CreateThread(NULL, 0, thread2, NULL, 0, &dwid);
	while(true)
	{
		Sleep(2000);
	}
	return 0;
}

