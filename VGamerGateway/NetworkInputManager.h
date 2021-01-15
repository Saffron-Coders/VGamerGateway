#pragma once

class NetworkInputManager
{
public:
	NetworkInputManager();
	int start();
	int startDiscoverMode();

private:
	bool m_Initialized;
	char* m_RecvBuffer;
	static int m_RecvLen;
	static PCSTR NET_IN_COMMAND_PORT, NET_IN_DISCOVER_PORT;
};

extern NetworkInputManager NetInMgr;
