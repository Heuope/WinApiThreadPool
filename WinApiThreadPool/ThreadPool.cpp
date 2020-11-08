#include "ThreadPool.h"

#define MAX_THREADS 255

bool ThreadPool::exitFlag = false;
int ThreadPool::threadLimit = 0;
std::mutex ThreadPool::mutex;
std::list<Task> ThreadPool::tasks;
int ThreadPool::currentNumOfThreads = 0;
HANDLE ThreadsArray[MAX_THREADS];

void ThreadPool::WriteLog(std::string message)
{
	std::ofstream out("Log.txt", std::ios::app);
	if (out.is_open())
	{
		out << message << std::endl;
	}
	out.close();
}

ThreadPool::ThreadPool(int maxThreads)
{
	DWORD threadId[MAX_THREADS];
	threadLimit = maxThreads;

	for (int i = 0; i < maxThreads; i++)
	{
		ThreadsArray[i] = CreateThread(NULL, 0, ThreadPool::FuncWrapper, 0, 0, &threadId[i]);
		WriteLog("|> New thread created, id = " + std::to_string(threadId[i]));
	}
};

DWORD WINAPI ThreadPool::FuncWrapper(void* args)
{
	Task task;

	while (!exitFlag)
	{
		if (!tasks.empty())
		{
			mutex.lock();
			if (!tasks.empty())
			{
				task = tasks.front();
				tasks.pop_front();
			}
			else
			{
				mutex.unlock();
				continue;
			}
			mutex.unlock();

			try
			{
				task.proc(task.args);
			}
			catch (const std::exception &e)
			{
				WriteLog(e.what());
			}
			currentNumOfThreads--;
		}
	}
	return 0;
}

void ThreadPool::Run(Procedure proc, void* args)
{
	Task task;
	task.proc = proc;
	task.args = args;

	if (currentNumOfThreads < threadLimit)
	{
		tasks.push_back(task);
		currentNumOfThreads++;
		WriteLog("Added new task");
	}
	else
	{
		WriteLog("|> Not found any free thread, task will be destroyed :(");
	}
}

void ThreadPool::WaitAll()
{
	exitFlag = true;
	WaitForMultipleObjects(threadLimit, ThreadsArray, TRUE, INFINITE);

	for (int i = 0; i < threadLimit; i++)
	{
		CloseHandle(ThreadsArray[i]);
	}
}