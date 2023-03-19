#include <Windows.h>
#include "TaskQueue.h"

TaskQueue::TaskQueue() {
	InitializeCriticalSectionAndSpinCount(&(this->lock), 0x00000400);
}

void TaskQueue::Add(SortTask* task) {	
	// thread synchronization 
	EnterCriticalSection(&lock);
	this->tasks.push(task);
	LeaveCriticalSection(&lock);
}

SortTask* TaskQueue::Get() {
	while (tasks.empty()) {}

	// thread synchronization 
	EnterCriticalSection(&lock);
	SortTask *res = this->tasks.front();
	tasks.pop();
	LeaveCriticalSection(&lock);
	return res;
}

bool TaskQueue::Empty() {
	return tasks.empty();
}

TaskQueue::~TaskQueue() {

	while (!this->Empty()) {
		this->Get();
	}

	DeleteCriticalSection(&(this->lock));
}