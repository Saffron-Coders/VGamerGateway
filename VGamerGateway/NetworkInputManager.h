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

	/// This function extracts the starting byte of a ControlMessage within the
	/// @buff and puts a pointer to it into @extracted_msg along with its @extracted_msg_len.
	/// Return: 0(Success), -1(Failure)
	int extractSingleMessage(const char* buff, size_t len, char* extracted_msg, size_t& extracted_msg_len);

	bool m_Initialized;
	char* m_RecvBuffer;
	static int m_RecvLen;
	static PCSTR NET_IN_COMMAND_PORT, NET_IN_DISCOVER_PORT;

	std::unique_ptr<InputProcessor> m_InputProcessor;
};

extern NetworkInputManager NetInMgr;
