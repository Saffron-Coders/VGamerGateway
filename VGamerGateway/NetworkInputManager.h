#pragma once

#include <memory>
#include "InputProcessor.h"

class NetworkInputManager
{
public:
	NetworkInputManager();
	~NetworkInputManager();
	int start();
	int startDiscoverMode();

private:
	bool m_Initialized;
	char* m_RecvBuffer;
	static int m_RecvLen;
	static PCSTR NET_IN_COMMAND_PORT, NET_IN_DISCOVER_PORT;

	std::unique_ptr<InputProcessor> m_InputDecoder;
};

extern NetworkInputManager NetInMgr;
