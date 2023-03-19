#include "TaskQueueProcessor.h"
#include <vector>

TaskQueueProcessor::TaskQueueProcessor(TaskQueue *queue) {
	this->queue = queue;
}

void TaskQueueProcessor::ExecuteTasks(int threadCount) {

	HANDLE* threads = (HANDLE*)malloc(sizeof(HANDLE) * threadCount);
	for (int i = 0; i < threadCount; i++)
	{
		HANDLE t = CreateThread(NULL, 0, &TaskQueueProcessor::Process, (LPVOID)(this->queue), CREATE_SUSPENDED, NULL);
		threads[i] = t;
	}

	for (int i = 0; i < threadCount; i++)
	{
		ResumeThread(threads[i]);
	}
	
	WaitForMultipleObjects(threadCount, threads, TRUE, INFINITE);
}

DWORD WINAPI TaskQueueProcessor::Process(void* vqueue) {
	TaskQueue* queue = (TaskQueue*)vqueue;
	if (!queue->Empty())
	{
		SortTask *task = queue->Get();
		task->func(task->params);
	}

	return 0;
}