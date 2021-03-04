#pragma once
#include "framework.h"

void __stdcall ReadingThread(LPVOID param)
{
	SOCKET s = reinterpret_cast<SOCKET>(param);
	char Buffer[512];

	while (true) {
		const int iResult = recv(s, Buffer, 512, 0);
		
		if (iResult <= 0) break;
		
		std::cout << "[SERVER] " << Buffer << std::endl;
	}
}

void ConnectServer() {

	WSADATA Winsockdata;

	const auto iWsaStartup = WSAStartup(MAKEWORD(2, 2), &Winsockdata);
	if (iWsaStartup != 0)
	{
		std::cout << "iWsaStartup Failed";
	}

	const auto TCPClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in TCPServerAdd;
	TCPServerAdd.sin_family = AF_INET;
	TCPServerAdd.sin_addr.s_addr = inet_addr("3.14.182.203");
	TCPServerAdd.sin_port = htons(16087);

	connect(TCPClientSocket, reinterpret_cast<SOCKADDR*>(&TCPServerAdd), sizeof(TCPServerAdd));
	printf("[CLIENT] Connected to the Server.");

	CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(&ReadingThread), reinterpret_cast<void*>(TCPClientSocket), 0, nullptr);

	char SenderBuffer[512];
	
	wcstombs(SenderBuffer, L"Fortnite Connected to the Server.", 12);

	send(TCPClientSocket, SenderBuffer, strlen(SenderBuffer) + 1, 0);
}