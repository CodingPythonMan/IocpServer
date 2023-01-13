#include "pch.h"
#include <iostream>
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ServerPacketHandler.h"
#include <tchar.h>

#pragma pack(1)
struct PKT_S_TEST
{
	uint32 hp;		// 4
	uint64 id;		// 8
	uint16 attack;	// 2
};
#pragma pack()

int main()
{
	PKT_S_TEST pkt;
	pkt.hp = 1;
	pkt.id = 2;
	pkt.attack = 3;

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

	WCHAR sendData3[1000] = L"가"; // UTF16 = Unicode (한글/로마 2바이트)

	while (true) 
	{
		vector<BuffData> buffs{ BuffData{100, 1.5f}, BuffData{200, 2.3f}, BuffData{300, 0.7f} };
		SendBufferRef sendBuffer = ServerPacketHandler::Make_S_TEST(1001, 100, 10, buffs, L"안녕하세요");
		GSessionManager.BroadCast(sendBuffer);

		this_thread::sleep_for(250ms);
	}

	GThreadManager->Join();
}