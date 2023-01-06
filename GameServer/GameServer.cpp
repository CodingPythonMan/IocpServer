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
	// ----------- ���뼱 --------------
}

void Consumer()
{
	// ------------ ���뼱 --------------
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
	// �� �߰�!
	// relase ��� ������ �޸� ��ɵ���, �ش� ��� ���ķ� ���ġ �Ǵ� �� �����մϴ�.
	// �׸��� acquire ���� ���� ������ �д� �����尡 �ִٸ�
	// release ������ ��ɵ��� -> acquire �ϴ� ������ ����ȴ�.(���ü� ����)

	// 3) relaxed (�����Ӵ� = �����Ϸ� ����ȭ ���� ���� = ���������� ����)
	// �ʹ����� �����Ӵ�!
	// �ڵ� ���ġ�� �ڴ�� ����! ���ü� �ذ� NO!
	// ���� �⺻ ���� (���� ��ü�� ���� ���� ���� ������ ����)

	// ����, AMD �� ��� �ִ��� ������ �ϰ����� ����
	// seq_cst �� �ᵵ ���ٸ� ���ϰ� ����
	// ARM �� ��� �� ���̰� �ִ�!
}