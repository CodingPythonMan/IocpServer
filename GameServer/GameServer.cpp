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
	// 동기(synchronous) 실행
	//int64 sum = Calculate();
	//std::cout << sum << std::endl;

	// std::future
	{
		// 1) deferred => lazy evaluation 지연해서 실행하세요.
		// 2) async => 별도의 스레드를 만들어서 실행하세요.
		// 3) defereed | async => 둘 중 알아서 골라주세요
		// 언젠가 미래에 결과물을 뱉어줄거야!
		std::future<int64> future = std::async(std::launch::async, Calculate);
	
		// TODO

		int64 sum = future.get(); // 결과물이 이제서야 필요하다!
	
		//Knight knight;
		//std::future<int64> future2 = std::async(std::launch::async, &Knight::GetHp, knight);
	}

	// std::promise
	{
		// 미래(std::future) 에 결과물을 반환해줄거라 약속 해줘-
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

	// 결론)
	// mutex, condition_variable 까지 가지 않고 단순한 애들을 처리할 수 있는...
	// 특히나, 한 번 발생하는 이벤트에 유용하다!
	// 닭잡는데 소잡는 칼을 쓸 필요 없다!
}