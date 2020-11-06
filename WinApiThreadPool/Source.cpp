#include <iostream>
#include "ThreadPool.h"

void Test(void* args)
{
	Sleep(100);
	std::cerr << "hello world" << std::endl;
}

int main()
{
	ThreadPool tp(5);

	for (int i = 0; i < 10; i++)
	{
		tp.run(Test, nullptr);
	}

	tp.waitAll();
	return 0;
}