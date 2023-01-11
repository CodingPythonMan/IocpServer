#include "pch.h"
#include <iostream>

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

    SOCKET clientSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        HandleError("Socket"); 
        return 0;
    }


    // 家南 府家胶 馆券
    ::closesocket(clientSocket);

    // 家南 辆丰
    ::WSACleanup();
}