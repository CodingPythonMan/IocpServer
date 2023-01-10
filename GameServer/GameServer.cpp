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
#include "Memory.h"
#include "Allocator.h"

class Knight
{
public:
	int32 _hp = rand() % 1000;
};

class Monster
{
public:
	int64 _id = 0;
};

int main()
{
	Knight* knights[100];

	for (int32 i = 0; i < 100; i++)
		knights[i] = ObjectPool<Knight>::Pop();

	for (int i = 0; i < 100; i++)
	{
		ObjectPool<Knight>::Push(knights[i]);
		knights[i] = nullptr;
	}

	shared_ptr<Knight> sptr = ObjectPool<Knight>::MakeShared();
	shared_ptr<Knight> sptr2 = MakeShared<Knight>();

	for (int32 i = 0; i < 3; i++)
	{
		GThreadManager->Launch([]()
		{
			while (true)
			{
				Knight* knight = xnew<Knight>();

				cout << knight->_hp << endl;

				this_thread::sleep_for(10ms);

				xdelete(knight);
			}
		});
	}

	GThreadManager->Join();
}