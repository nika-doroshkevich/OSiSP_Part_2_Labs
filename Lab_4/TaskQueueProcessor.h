#pragma once

#include <Windows.h>
#include <vector>
#include "TaskQueue.h"

class TaskQueueProcessor {
	private:
		TaskQueue *queue;
		static DWORD WINAPI Process(void* vqueue);
	
	public:
		TaskQueueProcessor(TaskQueue *queue);
		void ExecuteTasks(int threadCount);
};

