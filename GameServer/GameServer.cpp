#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>

std::mutex m;
std::queue<int32> q;
HANDLE handle;

void Producer()
{
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(m);
			q.push(100);
		}

		::SetEvent(handle);

		std::this_thread::sleep_for(std::chrono::microseconds(10000));
	}
}

void Consumer()
{
	while (true)
	{
		::WaitForSingleObject(handle, INFINITE);
		//::ResetEvent(handle);
		// Non-Signal

		std::unique_lock<std::mutex> lock(m);
		if (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();
			std::cout << data << std::endl;
		}
	}
}

int main()
{
	// Ŀ�� ������Ʈ
	// Usage Count
	// Signal (�Ķ���) / Non-Signal (������) << bool
	// Auto / Manual
	handle = ::CreateEvent(NULL/*���� �Ӽ�*/, FALSE/*bManualReset*/, 
		FALSE/*bInitialState*/,NULL);

	std::thread t1(Producer);
	std::thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);
}