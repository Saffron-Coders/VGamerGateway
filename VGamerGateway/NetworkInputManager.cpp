#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "NetworkInputManager.h"

PCSTR NetworkInputManager::NET_IN_COMMAND_PORT = "15000";
PCSTR NetworkInputManager::NET_IN_DISCOVER_PORT = "15010";
int NetworkInputManager::m_RecvLen = 512;

NetworkInputManager::NetworkInputManager() :
	m_Initialized(false), m_RecvBuffer(nullptr)
{
	WSAData wsa_data;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (ret != 0) {
		fprintf(stderr, "Error(%d): Winsock init failed.\n", ret);
		return;
	}
	m_RecvBuffer = new char[m_RecvLen];
	if (!m_RecvBuffer) {
		fprintf(stderr, "Error(-1): Buffer allocation failed.\n");
		WSACleanup();
		return;
	}

	m_Initialized = true;
}

static DWORD WINAPI disc_thread_fn(LPVOID lpParam)
{
	return NetInMgr.startDiscoverMode();
}

int NetworkInputManager::start()
{
	struct addrinfo* result = NULL, * ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	int ret = getaddrinfo(NULL, NET_IN_COMMAND_PORT, &hints, &result);
	if (ret != 0) {
		printf("getaddrinfo failed: %d\n", ret);
		WSACleanup();
		return -1;
	}

	// Create socket object for socket that will receive commands.
	SOCKET cmd_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (cmd_sock == INVALID_SOCKET) {
		fprintf(stderr, "Error(%ld): socket()\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return -1;
	}

	// Setup the UDP listening socket
	ret = bind(cmd_sock, result->ai_addr, (int)result->ai_addrlen);
	if (ret == SOCKET_ERROR) {
		fprintf(stderr, "Error(%d): bind() failed.\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(cmd_sock);
		WSACleanup();
		return -1;
	}

#if 0
	// Start discovery thread before going to infinite receive loop.
	HANDLE disc_thread;
	disc_thread = CreateThread(NULL, 0, disc_thread_fn, NULL, 0, NULL);
	if (!disc_thread) {
		fprintf(stderr, "Error(%d): CreateThread() failed.\n", GetLastError());
		freeaddrinfo(result);
		closesocket(cmd_sock);
		WSACleanup();
		return -1;
	}
#endif

	// Forever RECEIVE............... Unless crashed.

	sockaddr sender_addr;
	int sender_addr_len;
	do {
		sender_addr_len = sizeof(sender_addr);
		ret = recvfrom(cmd_sock, m_RecvBuffer, m_RecvLen, 0, (SOCKADDR*)&sender_addr, &sender_addr_len);
		m_RecvBuffer[ret] = '\0';
		if (ret > 0) {
			printf("[C]> %s\t...(%d)\n", m_RecvBuffer, ret);
			// TODO... Decode command and sendKey()
		}
		else {
			fprintf(stderr, "Error(%d): recv() failed\n", WSAGetLastError());
			closesocket(cmd_sock);
			WSACleanup();
			return -1;
		}
	} while (true);

	return 0;
}

// FIXME... Refactor usage of duplicate code.

int NetworkInputManager::startDiscoverMode()
{
	struct addrinfo* result = NULL, * ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	int ret = getaddrinfo(NULL, NET_IN_DISCOVER_PORT, &hints, &result);
	if (ret != 0) {
		printf("getaddrinfo failed: %d\n", ret);
		WSACleanup();
		return -1;
	}

	// Create socket object for socket that will receive commands.
	SOCKET disc_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (disc_sock == INVALID_SOCKET) {
		fprintf(stderr, "Error(%ld): socket()\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return -1;
	}

	// Setup the UDP listening socket
	ret = bind(disc_sock, result->ai_addr, (int)result->ai_addrlen);
	if (ret == SOCKET_ERROR) {
		fprintf(stderr, "Error(%d): bind() failed.\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(disc_sock);
		WSACleanup();
		return -1;
	}

	// Add to multicast group...

	struct ip_mreq imr;

	ret = InetPton(AF_INET, L"239.1.2.3", &imr.imr_multiaddr.s_addr);
	ret |= InetPton(AF_INET, L"0.0.0.0", &imr.imr_interface.s_addr) << 1;
	if (ret != 0x3) { // XXX... Just a clever error check.
		fprintf(stderr, "Error: InetPton() failed.\n");
		return -1;
	}

	ret = setsockopt(disc_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&imr, sizeof(imr));
	if (ret != 0) {
		fprintf(stderr, "Error(%d): bind() failed.\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(disc_sock);
		WSACleanup();
		return -1;
	}

	// Forever RECEIVE............... Unless crashed.

	char recv_buff[64];
	sockaddr sender_addr;
	int sender_addr_len;
	do {
		sender_addr_len = sizeof(sender_addr);
		ret = recvfrom(disc_sock, recv_buff, 64, 0, (SOCKADDR*)&sender_addr, &sender_addr_len);
		if (ret < 64) { // Protection from buffer overrun past recv_buff[63]
			recv_buff[ret] = '\0';
			if (ret > 0) {
				printf("[D]> %s\t...(%d)\n", recv_buff, ret);
				// TODO... Decode command and sendKey()
			}
			else {
				fprintf(stderr, "Error(%d): recv() failed\n", WSAGetLastError());
				closesocket(disc_sock);
				WSACleanup();
				return -1;
			}
		}
	} while (true);

	return 0;
}

NetworkInputManager NetInMgr;