#ifndef __THREAD_H__
#define __THREAD_H__

#ifndef _WIN32
#include <pthread.h>
#include <unistd.h>
#else
#include <Windows.h>
#endif

#include <list>
#include "Mutex.h"

using namespace std;

#ifndef _WIN32
typedef list<pthread_t> thread_id_list;
#else
typedef list<HANDLE> thread_id_list;
#endif

typedef thread_id_list::iterator thread_id_iterator;

class Thread
{
public:
	Thread(const char* name="");
	~Thread();
private:
#ifndef _WIN32
	pthread_attr_t			attr;
#endif
	Thread_Mutex			id_mutex;
	thread_id_list			id_list;
protected:
	char					thread_name[256];
public:
	virtual int				svc() = 0;
	bool					activate(int threads = 1);
	bool					kill_all();
	bool					join();
    static void             sleep(int ms);

private:
#ifndef _WIN32
	static void*					run0(void* pVoid);
#else
	static unsigned int __stdcall	run0(void* pVoid);
#endif
};

#endif//__THREAD_H__
