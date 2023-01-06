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

	// �޸� ��å
	// 1) Sequentially Consistent(seq_cst)
	// 2) Acquire-Release (consume, acquire, release, acq_rel)
	// 3) Relaxed (relaxed)

	// 1) seq_cst (���� ���� = �����Ϸ� ����ȭ ���� ���� = ������)
	// ���ü� ���� �ٷ� �ذ�! �ڵ� ���ġ �ٷ� �ذ�!
	
	// 2) acquire_release
	
	// 3) relaxed (�����Ӵ� = �����Ϸ� ����ȭ ���� ���� = ���������� ����)
	// �ʹ����� �����Ӵ�!
	// �ڵ� ���ġ�� �ڴ�� ����! ���ü� �ذ� NO!
	// ���� �⺻ ���� (���� ��ü�� ���� ���� ���� ������ ����)
}