#pragma once

#include <mutex>

template<typename T>
class LockStack
{
public:
	LockStack() {}

	LockStack(const LockStack&) = delete;
	LockStack& operator=(const LockStack&) = delete;

	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value));
		_condVar.notify_one();
	}

	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		if (_stack.empty())
			return false;

		// empty -> top -> pop
		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] {return _stack.empty() == false; });
		value = std::move(_stack.top());
		_stack.pop();
	}

private:
	stack<T> _stack;
	mutex _mutex;
	condition_variable _condVar;
};


template<typename T>
class LockFreeStack
{
	struct Node
	{
		Node(const T& value) : data(value)
		{

		}

		T data;
		Node* next;
	};

public:

	// 1. 새 노드를 만들고
	// 2. 새 노드의 next = head
	// 3. head = 새 노드

	// [ ][ ][ ]
	// [head]
	void Push(const T& value)
	{
		Node* node = new Node(value);
		node->next = _head;

		/*if (_head == node->next)
		{
			_head = node;
			return true;
		}
		else
		{
			node->next = _head;
			return false;
		}*/

		while (_head.compare_exchange_weak(node->next, node) == false)
		{
			// node-> next = _head;
		}

		// 이 사이에 새치기 당하면?
		//_head = node;
	}

	// 1) head 읽기
	// 2) head->next 읽기
	// 3) head = head->next
	// 4) data 추출해서 반환
	// 5) 추출한 노드를 삭제

	// [ ][ ][ ]
	// [head]
	bool TryPop(T& value)
	{
		Node* oldHead = _head;

		/*if (_head == oldHead)
		{
			_head = oldHead->next;
			return true;
		}
		else
		{
			oldHead = _head;
			return false;
		}*/

		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false)
		{

		}

		if (oldHead == nullptr)
			return false;

		// Exception X
		value = oldHead->data;

		// 잠시 삭제 보류
		//delete oldHead;
		
		// C#, Java 같이 GC가 있으면 신경쓰지 않아도 되지만...
		
		return true;
	}

private:
	// [ ][ ][ ]
	// [head]
	atomic<Node*> _head;
};