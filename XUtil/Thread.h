#ifndef __THREAD_H__
#define __THREAD_H__

#ifndef _WIN32
#include <pthread.h>
#include <unistd.h>
#else
#include <Windows.h>
#endif

#include "Def.h"
#include <list>
#include "Mutex.h"

#ifndef _WIN32
typedef std::list<pthread_t> thread_id_list;
#else
typedef std::list<HANDLE> thread_id_list;
#endif

typedef thread_id_list::iterator thread_id_iterator;

NS_ZMD_BEGIN

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

NS_ZMD_END

#endif//__THREAD_H__
