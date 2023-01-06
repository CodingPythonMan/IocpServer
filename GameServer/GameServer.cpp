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

	ready.store(true);
	
	std::atomic_thread_fence(std::memory_order::memory_order_release);
	// ----------- 절취선 --------------
}

void Consumer()
{
	// ------------ 절취선 --------------
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
	// 딱 중간!
	// relase 명령 이전의 메모리 명령들이, 해당 명령 이후로 재배치 되는 걸 금지합니다.
	// 그리고 acquire 모드로 같은 변수를 읽는 쓰레드가 있다면
	// release 이전의 명령들이 -> acquire 하는 순간에 보장된다.(가시성 보장)

	// 3) relaxed (자유롭다 = 컴파일러 최적화 여지 많음 = 직관적이지 않음)
	// 너무나도 자유롭다!
	// 코드 재배치도 멋대로 가능! 가시성 해결 NO!
	// 가장 기본 조건 (동일 객체에 대한 동일 관전 순서만 보장)

	// 인텔, AMD 의 경우 애당초 순차적 일관성을 보장
	// seq_cst 를 써도 별다른 부하가 없음
	// ARM 의 경우 꽤 차이가 있다!
}