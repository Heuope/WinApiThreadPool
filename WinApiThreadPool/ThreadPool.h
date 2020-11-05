#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <windows.h>
#include <process.h>

typedef void (*Procedure)(void*);

class ThreadStatus
{
public:
	ThreadStatus(int id);
	ThreadStatus();
	void WriteLog(std::string message);
	bool isWorking;
	void* threadHandle;
	int id;
	Procedure func;
	void* funcArgs;
};

class ThreadPool
{
private:
	std::map<int, ThreadStatus> threadMap;
	int threadLimit;
	int GetFreeThreadId();
	static DWORD WINAPI _stdcall funcWrapper(void* args);

public:
	ThreadPool(int maxTasks);

	void run(Procedure proc, void* args);
	void waitAll();
};