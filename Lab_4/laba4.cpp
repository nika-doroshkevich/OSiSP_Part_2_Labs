#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

#include "TaskQueue.h"
#include "TaskQueueProcessor.h"

#define FILEPATH "input.txt"
#define THREAD_COUNT 8

using namespace std;

vector<string> MergeArrays(vector<string> v1, vector<string> v2);
vector<string> ReadFile(string filePath);
void WriteFile(vector<string> sortedData);
void Sort(void* content);
vector<SortTask*> InitTasks(vector<string> text);

vector<vector<string>*> slices;

int main() {
    // read text from file
    auto text = ReadFile(FILEPATH);
    auto tasks = InitTasks(text);

    TaskQueue taskQueue;
    for (SortTask* task : tasks) {
        taskQueue.Add(task);
    }

    TaskQueueProcessor processor(&taskQueue);
    processor.ExecuteTasks(THREAD_COUNT);

    vector<string> result((slices[0])->begin(), (slices[0])->end());
    for (int i = 1; i < slices.size(); i++)
    {
        result = MergeArrays(result, *(slices[i]));
    }

    WriteFile(result);
    return 0;
}

void WriteFile(vector<string> sortedData) {
    std::ofstream file("output.txt");
    for (string line : sortedData) {
        file << line << "\n";
    }
}

vector<string> ReadFile(string filePath) {
    ifstream file(filePath);
    string word;
    vector<string> res;
    while (file >> word)
    {
        res.push_back(word);
    }
    return res;
}

void Sort(void* content) {
    vector<string>* obj = (vector<string>*)content;
    sort(obj->begin(), obj->end());
}

vector<SortTask*> InitTasks(vector<string> text) {

    int wordsPerThread = ceil((double)text.size() / THREAD_COUNT);
    vector<SortTask*> tasks;

    for (int i = 0; i < THREAD_COUNT; ++i) {
        vector<string>* slice = new vector<string>(text.begin() + i * wordsPerThread,
            text.size() < (i + 1) * wordsPerThread ?
            text.end() : text.begin() + (i + 1) * wordsPerThread);

        SortTask* t = new SortTask(Sort, (void*)slice);
        slices.push_back(slice);

        tasks.push_back(t);
    }

    return tasks;
}

vector<string> MergeArrays(vector<string> v1, vector<string> v2) {
    int i = 0, j = 0;
    vector<string> res{};
    
    while (i < v1.size() && j < v2.size()) {
        if (v1[i] < v2[j])
            res.push_back(v1[i++]);
        else
            res.push_back(v2[j++]);
    }

    while (i < v1.size())
        res.push_back(v1[i++]);

    while (j < v2.size())
        res.push_back(v2[j++]);

    return res;
}