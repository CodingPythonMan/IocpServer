#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
#include "ThreadManager.h"

#include <WinSock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

void HandleError(const char* cause)
{
	int32 errCode = ::WSAGetLastError();
	cout << cause << " ErrorCode : " << errCode << endl;
}

int main()
{
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	SOCKET serverSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		HandleError("Socket");
		return 0;
	}

	// 옵션을 해석하고 처리할 주체?
	// 소켓 코드 -> SOL_SOCKET
	// IPv4 -> IPPROTO_IP
	// TCP 프로토콜 -> IPPROTO_TCP

	// SO_KEEPALIVE = 주기적으로 연결 상태 확인 여부 (TCP Only)
	// 상대방이 소리소문 없이 연결을 끊는다면?
	// 주기적으로 TCP 프로토콜 연결 상태 확인 -> 끊어진 연결 감지
	
	bool enable = true;
	::setsockopt(serverSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&enable, sizeof(enable));
	
	// SO_LINGER = 지연하다.
	// 송신 버퍼에 있는 데이터를 보낼 것인가? 날릴 것인가?
	// onoff = 0 이면 closesocket()이 바로 리턴, 아니면 linger 초만큼 대기 (default 0)
	// linger : 대기시간
	LINGER linger;
	linger.l_onoff = 1;
	linger.l_linger = 5;
	::setsockopt(serverSocket, SOL_SOCKET, SO_LINGER, (char*)&linger, sizeof(linger));

	// Half-Close
	// SD_SEND : send 막는다
	// SD_RECEIVE : recv 막는다
	// SD_BOTH : 둘다 막는다
	//::shutdown(serverSocket, SD_SEND);
	// 소켓 리소스 반환
	// send -> closesocket
	//::closesocket(serverSocket);

	// SO_SNDBUF = 송신 버퍼 크기
	// SO_RCVBUF = 수신 버퍼 크기

	int32 sendBufferSize;
	int32 optionLen = sizeof(sendBufferSize);
	::getsockopt(serverSocket, SOL_SOCKET, SO_SNDBUF, (char*)&sendBufferSize, &optionLen);
	cout << "송신 버퍼 크기 : " << sendBufferSize << endl;

	int32 recvBufferSize;
	optionLen = sizeof(recvBufferSize);
	::getsockopt(serverSocket, SOL_SOCKET, SO_RCVBUF, (char*)&recvBufferSize, &optionLen);
	cout << "수신 버퍼 크기 : " << recvBufferSize << endl;

	// SO_REUSEADDR
	// IP 주소 및 port 재사용
	{
		bool enable = true;
		::setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&enable, sizeof(enable));
	}

	// IPPROTO_TCP
	// TCP_NODELAY = Nagle 네이글 알고리즘 작동 여부
	// 데이터가 충분히 크면 보내고, 그렇지 않으면 데이터가 충분히 쌓일 때까지 대기!
	// 장점 : 작은 패킷이 불필요하게 많이 생성되는 일을 방지
	// 단점 : 반응 시간 손해
	{
		bool enable = true;
		::setsockopt(serverSocket, IPPROTO_TCP , TCP_NODELAY, (char*)&enable, sizeof(enable));
	}

	// 소켓 종료
	::WSACleanup();
}