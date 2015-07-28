#include "Thread.h"

#ifndef _WIN32
#include<string.h>
#include<sys/types.h>
#else
#pragma warning(disable:4996)
#include <process.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

Thread::Thread(const char* name)
{
#ifndef _WIN32
	pthread_attr_init(&attr);
#else
	strcpy(thread_name, name);
#endif
}

Thread::~Thread()
{
#ifndef _WIN32
	pthread_attr_destroy(&attr);
#endif
}

bool Thread::activate(int threads)
{
	Guard guard(id_mutex);
	if(threads <= 0)
	{
		return false;
	}
	for(int i=0; i<threads; i++)
	{
#ifndef _WIN32
		pthread_t tid;
		if(pthread_create(&tid, &attr, run0, this) != 0)
			return false;
		id_list.push_back(tid);
#else
		unsigned int dwThreadID = 0;
		HANDLE hThread;
		hThread = (HANDLE)_beginthreadex(NULL, 0, run0, this, 0, &dwThreadID);
		id_list.push_back(hThread);
#endif
	}
	return true;
}

bool Thread::kill_all()
{
	Guard guard(id_mutex);
	if(id_list.size() == 0)
	{
		return false;
	}
	thread_id_iterator it;
	for(it = id_list.begin(); it != id_list.end(); it++)
	{
#ifndef _WIN32
		//pthread_cancel((*it)); Android NDK不支持 可使用 pthread_kill 实现sigaction 函数处理 
#else
		TerminateThread((*it), 0);
#endif
	}
	id_list.clear();
	return true;
}

bool Thread::join()
{
	Guard guard(id_mutex);
	if(id_list.size() == 0)
	{
		return true;
	}
	thread_id_iterator it;
	for(it = id_list.begin(); it != id_list.end(); it++)
	{
#ifndef _WIN32
        int s = pthread_join((*it), 0);
#else
        DWORD s = WaitForSingleObject((*it), INFINITE);
#endif
		if(s != 0)
		{
			printf("thread_join s=%d\n", s);
		}
	}
	id_list.clear();
	return false;
}

#ifndef _WIN32
void* Thread::run0(void* pVoid)
#else
unsigned int Thread::run0(void* pVoid)
#endif
{
	Thread* p_this = (Thread*)pVoid;
	if (p_this)
	{
		p_this->svc();
	}
	return 0;
}

void Thread::sleep(int ms)
{
#ifndef _WIN32
    struct timeval tv;
    tv.tv_sec = ms / 1000;
    tv.tv_usec = (ms % 1000) * 1000;
    select(0, NULL, NULL, NULL, &tv);
#else
    Sleep(ms);
#endif
}
