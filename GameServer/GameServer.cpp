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

// ����) cv �� User-Level Object (Ŀ�� ������Ʈx)
std::condition_variable cv;

void Producer()
{
	while (true)
	{
		// 1. Lock�� ���
		// 2. ���� ���� ���� ����
		// 3. Lock�� Ǯ��
		// 4. ���Ǻ��� ���� �ٸ� �����忡�� ����
		{
			std::unique_lock<std::mutex> lock(m);
			q.push(100);
		}

		cv.notify_one(); // wait ���� �����尡 ������ �� 1���� �����.

		//std::this_thread::sleep_for(std::chrono::microseconds(10000));
	}
}

void Consumer()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(m);
		cv.wait(lock, []() {return q.empty() == false; });
		// 1. lock �� ���
		// 2. ���� Ȯ��
		// - ����0 => �������ͼ� �̾ �ڵ带 ����
		// - ����x => Lock �� Ǯ���ְ� ��� ����

		// �׷��� notify_one �� ������ �׻� ���ǽ��� �����ϴ� �� �ƴұ�?
		// Spurious Makeup (��¥ ���?)
		// notify_one �� �� lock �� ��� �ִ� ���� �ƴϱ� ����

		//if (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();
			std::cout << q.size() << std::endl;
		}
	}
}

int main()
{
	std::thread t1(Producer);
	std::thread t2(Consumer);

	t1.join();
	t2.join();
}