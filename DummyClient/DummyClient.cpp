#include "pch.h"
#include <iostream>

#include <WinSock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
    // Winsock �ʱ�ȭ (ws2_32 ���̺귯�� �ʱ�ȭ)
    // ���� ������ wsaData �� ä����
    WSAData wsaData;
    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return 0;

    // ad : Address Family (AF_INET = IPv4, AF_INET6 = IPv6)
    // type : TCP(SOCK_STREAM) vs UDP(SOCK_DGRAM)
    // protocol : 0
    // return : descriptor
    SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        int32 errCode = ::WSAGetLastError();
        cout << "Socket ErrorCode : " << errCode << endl;
        return 0;
    }

    // ������ ��������? (IP�ּ� + Port) -> XX ����Ʈ YY ȣ
    SOCKADDR_IN serverAddr; // IPv4
    ::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    //serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1"); << deprecated
    ::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    serverAddr.sin_port = ::htons(7777);

    // host to network short
    // Little-Endian vs Big-Endian
    // ex) 0x12345678 4����Ʈ ����
    // low [0x78][0x56][0x34][0x12] high < little
    // low [0x12][0x34][0x56][0x78] high < big = network

	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Connect ErrorCode : " << errCode << endl;
		return 0;
	}
    
    // ----------------------------------------
    // ���� ���� : �������� ������ �ۼ��� ����!
    
    cout << "Connected To Server!" << endl;

    while (true)
    {
        this_thread::sleep_for(1s);
    }

    // -----------------------------------------

    // ���� ���ҽ� ��ȯ
    ::closesocket(clientSocket);

    // ���� ����
    ::WSACleanup();
}