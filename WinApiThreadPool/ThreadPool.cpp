#include "ThreadPool.h"

ThreadStatus::ThreadStatus(int id)
{
	this->id = id;
	this->func = nullptr;
	this->funcArgs = nullptr;
	this->isWorking = false;
	this->threadHandle = 0;
}

ThreadStatus::ThreadStatus() {}

ThreadPool::ThreadPool(int maxTasks)
{
	this->threadLimit = maxTasks;
	for (int i = 0; i < maxTasks; i++)
	{
		int newThreadId = (int)this->threadMap.size();
		this->threadMap.emplace(newThreadId, ThreadStatus(newThreadId));
		this->threadMap[newThreadId].WriteLog("|> New thread created, id = " + std::to_string(newThreadId));
	}
};

void ThreadStatus::WriteLog(std::string message)
{
	std::ofstream out("Log.txt", std::ios::app);
	if (out.is_open())
	{
		out << message << std::endl;
	}
	out.close();
}

int ThreadPool::GetFreeThreadId()
{
	Sleep(10);
	for (int i = 0; i < this->threadMap.size(); i++)
	{
		if (!this->threadMap[i].isWorking)
			return i;
	}
	return -1;
}

DWORD WINAPI ThreadPool::funcWrapper(void* args)
{
	auto params = (ThreadStatus*)(args);

	try
	{
		params->func(params->funcArgs);
	}
	catch (std::exception & e)
	{
		params->WriteLog("|> Exception: " + std::string(e.what()) + " , thread id = " + std::to_string(params->id));
	}

	CloseHandle(params->threadHandle);
	params->threadHandle = 0;
	params->isWorking = false;
	params->func = nullptr;

	return 0;
}

void ThreadPool::waitAll()
{
	for (int i = 0; i < this->threadLimit; i++)
	{
		if (this->threadMap[i].isWorking)
		{
			WaitForSingleObjectEx(this->threadMap[i].threadHandle, INFINITE, FALSE);
		}
	}
}

void ThreadPool::run(Procedure proc, void* args)
{
	Sleep(5);
	int freeThreadId = GetFreeThreadId();
	if (freeThreadId == -1)
	{
		this->threadMap[freeThreadId].WriteLog("|> Not found any free thread, task will be destroyed :(");
		return;
	}
	auto params = &this->threadMap[freeThreadId];

	params->func = proc;
	params->funcArgs = args;
	params->isWorking = true;

	params->WriteLog("|> Task was added (Thread id = " + std::to_string(freeThreadId) + ")");

	params->threadHandle = (void*)CreateThread(0, 0, &ThreadPool::funcWrapper, params, 0, 0);
}