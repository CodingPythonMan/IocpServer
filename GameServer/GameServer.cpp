#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>

std::atomic<bool> ready;
int32 value;

void Producer()
{
	value = 10;

	ready.store(true, std::memory_order::memory_order_relaxed);
}

void Consumer()
{
	while (ready.load(std::memory_order::memory_order_relaxed) == false);

	std::cout << value << std::endl;
}

int main()
{
	ready = false;
	value = 0;
	std::thread t1(Producer);
	std::thread t2(Consumer);
	t1.join();
	t2.join();

	// 메모리 정책
	// 1) Sequentially Consistent(seq_cst)
	// 2) Acquire-Release (consume, acquire, release, acq_rel)
	// 3) Relaxed (relaxed)

	// 1) seq_cst (가장 엄격 = 컴파일러 최적화 여지 적음 = 직관적)
	// 가시성 문제 바로 해결! 코드 재배치 바로 해결!
	
	// 2) acquire_release
	
	// 3) relaxed (자유롭다 = 컴파일러 최적화 여지 많음 = 직관적이지 않음)
	// 너무나도 자유롭다!
	// 코드 재배치도 멋대로 가능! 가시성 해결 NO!
	// 가장 기본 조건 (동일 객체에 대한 동일 관전 순서만 보장)
}