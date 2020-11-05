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