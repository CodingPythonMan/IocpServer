#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"
#include "RefCounting.h"

class Knight
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}

	~Knight()
	{
		cout << "~Knight()" << endl;
	}
};

int main()
{
	// 1. �̹� ������� Ŭ���� ������� ��� �Ұ�
	// 2. ��ȯ ����

	// shared_ptr
	// weak_ptr
	
	// [Knight | RefCountingBlock][ngBlock]
	
	// [T*][RefCountBlock*]

	// RefCountBlock(useCount(shared), weakCount)
	shared_ptr<Knight> spr = make_shared<Knight>();
	weak_ptr<Knight> wpr = spr;

	bool expired = wpr.expired();
	shared_ptr<Knight> spr2 = wpr.lock();
}