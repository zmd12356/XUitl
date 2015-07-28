#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template < class T >
class Singleton
{
public:
    static T* instance()
	{
		static T _instance;
		return  &_instance;
	}
protected:
	Singleton() {}
private:
	Singleton(const Singleton &);
    Singleton& operator = (const Singleton &);
};

#endif//__SINGLETON_H__
