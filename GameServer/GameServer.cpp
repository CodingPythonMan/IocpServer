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

// 참고) cv 는 User-Level Object (커널 오브젝트x)
std::condition_variable cv;

void Producer()
{
	while (true)
	{
		// 1. Lock을 잡고
		// 2. 공유 변수 값을 수정
		// 3. Lock을 풀고
		// 4. 조건변수 통해 다른 스레드에게 통지
		{
			std::unique_lock<std::mutex> lock(m);
			q.push(100);
		}

		cv.notify_one(); // wait 중인 쓰레드가 있으면 딱 1개를 깨운다.

		//std::this_thread::sleep_for(std::chrono::microseconds(10000));
	}
}

void Consumer()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(m);
		cv.wait(lock, []() {return q.empty() == false; });
		// 1. lock 을 잡고
		// 2. 조건 확인
		// - 만족0 => 빠져나와서 이어서 코드를 진행
		// - 만족x => Lock 을 풀어주고 대기 상태

		// 그런데 notify_one 을 했으면 항상 조건식을 만족하는 거 아닐까?
		// Spurious Makeup (가짜 기상?)
		// notify_one 할 때 lock 을 잡고 있는 것이 아니기 때문

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