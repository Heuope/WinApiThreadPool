#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <windows.h>
#include <process.h>

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