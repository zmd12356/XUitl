#ifndef __MUTEX_H__
#define __MUTEX_H__

#ifndef _WIN32
#include <pthread.h>
//#include <unistd.h>
#else
#include <Windows.h>
#endif

class Mutex
{
public:
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual bool trylock() = 0;
};

class Guard
{
public:
    Guard(Mutex* pMutex)
    {
        m_pMutex = pMutex;
        if (0 != m_pMutex)
		{
            m_pMutex->lock();
        }
    }
    Guard(Mutex& mutex)
    {
        m_pMutex = &mutex;
        m_pMutex->lock();
    }
    ~Guard()
    {
        if (0 != m_pMutex)
		{
            m_pMutex->unlock();
        }
    }
private:
	Mutex* volatile m_pMutex;
};

#ifndef _WIN32

class Thread_Mutex : public Mutex
{
public:
    Thread_Mutex()
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&_mutex, &attr);
    }
    ~Thread_Mutex()
    {
        pthread_mutex_destroy(&m_mutex);
    }
    virtual void lock()
    {
        pthread_mutex_lock(&m_mutex);
    }
    virtual void unlock()
    {
        pthread_mutex_unlock(&m_mutex);
    }
    virtual bool trylock()
    {
        return (pthread_mutex_trylock(&m_mutex) == 0);
    }
private:
    pthread_mutex_t m_mutex;
};

#else

class Thread_Mutex : public Mutex
{
public:
    Thread_Mutex()
    {
        ::InitializeCriticalSection(&m_cs);
    }
    ~Thread_Mutex()
    {
        ::DeleteCriticalSection(&m_cs);
    }
    virtual void lock() {
        ::EnterCriticalSection(&m_cs);
    }
    virtual void unlock() {
        ::LeaveCriticalSection(&m_cs);
    }
    virtual bool trylock() {
		return !(::TryEnterCriticalSection(&m_cs) == 0);
    }
    CRITICAL_SECTION m_cs;
};
#endif	//Thread_Mutex

//--------------Shared_Thread_Mutex-----------------
class Shared_Thread_Mutex
{
public:
    Shared_Thread_Mutex(unsigned int size = 8)
    {
        m_pMutex = new Thread_Mutex[size];
        m_poolSize = size;
        m_poolCounter = 0;
    }
    ~Shared_Thread_Mutex()
    {
        delete[] m_pMutex;
        m_pMutex = 0;
    }
    Thread_Mutex* operator & ()
    {
        unsigned int index = m_poolCounter++;
        index %= m_poolSize;
        return &m_pMutex[index];
    }
private:
    Thread_Mutex* m_pMutex;
    unsigned int m_poolSize;
    volatile unsigned int m_poolCounter;
};

#endif	//__MUTEX_H__
