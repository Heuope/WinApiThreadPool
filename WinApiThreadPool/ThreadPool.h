#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <mutex>
#include <string>
#include <windows.h>
#include <process.h>
#include <list>

typedef void (*Procedure)(void*);

class Task
{
public:
	Procedure proc;
	void* args;
};

class ThreadPool
{
private:
	static DWORD WINAPI _stdcall FuncWrapper(void* args);

public:
	static int threadLimit;
	static bool exitFlag;
	static std::mutex mutex;
	static std::list<Task> tasks;
	static int currentNumOfThreads;

	ThreadPool(int maxThreads);

	static void WriteLog(std::string message);
	void Run(Procedure proc, void* args);
	void WaitAll();
};