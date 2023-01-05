#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

int64 result;

int64 Calculate()
{
	int64 sum = 0;

	for (int32 i = 0; i < 1000000; i++)
	{
		sum += i;
	}

	return sum;
}

void PromiseWorker(std::promise<std::string>&& promise)
{
	promise.set_value("Secret_message");
}

void TaskWorker(std::packaged_task<int64(void)>&& task)
{
	task();
}
int main()
{
	// ����(synchronous) ����
	//int64 sum = Calculate();
	//std::cout << sum << std::endl;

	// std::future
	{
		// 1) deferred => lazy evaluation �����ؼ� �����ϼ���.
		// 2) async => ������ �����带 ���� �����ϼ���.
		// 3) defereed | async => �� �� �˾Ƽ� ����ּ���
		// ������ �̷��� ������� ����ٰž�!
		std::future<int64> future = std::async(std::launch::async, Calculate);
	
		// TODO

		int64 sum = future.get(); // ������� �������� �ʿ��ϴ�!
	
		//Knight knight;
		//std::future<int64> future2 = std::async(std::launch::async, &Knight::GetHp, knight);
	}

	// std::promise
	{
		// �̷�(std::future) �� ������� ��ȯ���ٰŶ� ��� ����-
		std::promise<std::string> promise;
		std::future<std::string> future = promise.get_future();

		std::thread t(PromiseWorker, std::move(promise));

		std::string message = future.get();
		std::cout << message << std::endl;

		t.join();
	}

	// std::packaged_task
	{
		std::packaged_task<int64(void)> task(Calculate);
		std::future<int64> future = task.get_future();

		std::thread t(TaskWorker, std::move(task));

		int64 sum = future.get();
		std::cout << sum << std::endl;

		t.join();
	}

	// ���)
	// mutex, condition_variable ���� ���� �ʰ� �ܼ��� �ֵ��� ó���� �� �ִ�...
	// Ư����, �� �� �߻��ϴ� �̺�Ʈ�� �����ϴ�!
	// ����µ� ����� Į�� �� �ʿ� ����!
}