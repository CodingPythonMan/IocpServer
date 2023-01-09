#pragma once
#include <stack>
#include <map>
#include <vector>

/*-------------------------
	DeadLock Profiler
--------------------------*/

class DeadLockProfiler
{
public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();

private:
	unordered_map<const char*, int32> _nameToId;
	unordered_map<int32, const char*> _idToName;
	stack<int32> _lockStack;
	map<int32, set<int32>> _lockHistory;

	Mutex _lock;

	vector<int32> _discoveredOrder; // 노드가 발견된 순서를 기록하는 배열
	int32 _discoveredCount = 0; // 노드가 발견된 순서
	vector<bool> _finished;	// Dfs(i) 가 종료되었는지 여부
	vector<int32> _parent;

	void Dfs(int32 index);
};