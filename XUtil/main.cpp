//zmd12356@qq.com 2013-7

#include <iostream>
#include "Singleton.h"
#include "Mutex.h"

USING_NS_ZMD;

class XABC : public Singleton<XABC>
{
public:
	void sayHello(){std::cout<< "HelloABC" << std::endl;};
};

class temp{};

int main(int argc, char* argv[])
{
	Mutex* mutex = new Thread_Mutex();
	Guard guard(mutex);
	if (mutex->trylock())
	{
		std::cout<< "try ok" << std::endl;
	}

	temp *t = NULL;
	delete t;
	XABC* abc = XABC::instance();
	XABC* bcd = XABC::instance();
	if (abc == bcd)
	{
		std::cout<< "same" << std::endl;
	}
	std::cout<< "Hello" << std::endl;
	return 0;
}

