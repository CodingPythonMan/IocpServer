#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ClientPacketHandler.h"
#include <tchar.h>
#include "Protocol.pb.h"
#include "Job.h"
#include "Room.h"

void HealByValue(uint64 target, int32 value)
{
	cout << target << "ÇÑÅ× Èú" << value << "¸¸Å­ ÁÜ" << endl;
}

class Knight
{
public:
	void HealMe(int32 value)
	{
		cout << "HealMe! " << value << endl;
	}
};

int main()
{
	auto tup = std::tuple<int32, int32>(1, 2);
	auto val0 = std::get<0>(tup);
	auto val1 = std::get<1>(tup);

	auto s = gen_seq<3>();
	// gen_seq<3>
	// : gen_seq<2, 2>
	// : gen_seq<1, 1, 2>
	// : gen_seq<0, 0, 1, 2>
	// : seq<0, 1, 2>

	// TEST JOB
	{
		FuncJob<void, uint64, int32> job(HealByValue, 100, 10);
		job.Execute();
	}
	{
		Knight k1;
		MemberJob job2(&k1, &Knight::HealMe, 10);
		job2.Execute();
	}

	// JOB

	ClientPacketHandler::Init();

	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>,
		100);

	ASSERT_CRASH(service->Start());
	
	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]() 
		{
			while (true)
			{
				service->GetIocpCore()->Dispatch();
			}
		});
	}

	while (true)
	{
		GRoom.FlushJob();
		this_thread::sleep_for(1ms);
	}

	GThreadManager->Join();
}