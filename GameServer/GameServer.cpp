#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

// 소수 구하기

// 1 과 자기 자신으로만 나뉘면 그것을 소수라고 함.
bool IsPrime(int number)
{
	if (number <= 1)
		return false;
	if (number == 2 || number == 3)
		return true;

	for (int i = 2; i < number; i++)
	{
		if ((number % i) == 0)
			return false;
	}

	return true;
}

int CountPrime(int start, int end)
{
	int count = 0;
	
	for (int number = start; number <= end; number++)
	{
		if (IsPrime(number))
			count++;
	}

	return count;
}

int main()
{
	const int MAX_NUMBER = 1000000;
	// 1-MAX_NUMBER 까지의 소수 개수

	vector<thread> threads;

	int coreCount = thread::hardware_concurrency();
	int jobCount = (MAX_NUMBER / coreCount) + 1;

	atomic<int> primeCount = 0;

	for (int i = 0; i < coreCount; i++)
	{
		int start = (i * jobCount) + 1;
		int end = min(MAX_NUMBER, ((i + 1) * jobCount));
	
		threads.push_back(thread([start, end, &primeCount]() {
			primeCount += CountPrime(start, end);
		}));
	}

	for (thread& t: threads)
		t.join();

	cout << primeCount << endl;
}