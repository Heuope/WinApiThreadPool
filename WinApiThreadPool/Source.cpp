#include <iostream>
#include "ThreadPool.h"

void Test1(void* args)
{
	Sleep(100);
	std::cerr << "hello world" << std::endl;
}

void Test2(void* args)
{
	Sleep(200);
	std::cerr << "hello world" << std::endl;
}

int main()
{
	ThreadPool tp(2);

	tp.Run(Test1, nullptr);
	tp.Run(Test2, nullptr);

	tp.Run(Test2, nullptr);
	tp.Run(Test1, nullptr);

	tp.WaitAll();
	return 0;
}