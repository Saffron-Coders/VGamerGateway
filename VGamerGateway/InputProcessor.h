#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>
#include "ControlMessage.h"

class InputProcessor
{
public:

	InputProcessor();
	~InputProcessor();

	/// Process each event sequentially.
	/// Returns whatever ControlMessage::deserialize() returns.	
	int process(const uint8_t* msg, size_t len);

	/// Typedef for control message handler function.
	typedef std::function<int(const ControlMessage& ctl_msg, uint8_t ev_value)> InputHandlerFunc;
	typedef std::unordered_map<uint16_t, InputHandlerFunc>	InputHandlerMap;
private:

	InputHandlerMap m_Handlers;

	// -------- All the handler functions ----------
	int mouseMove(const ControlMessage& ctl_msg, short x, short y);
	int shoot(const ControlMessage& ctl_msg, uint8_t ev_value);
	int aimdown(const ControlMessage& ctl_msg, uint8_t ev_value);
	int reload(const ControlMessage& ctl_msg, uint8_t ev_value);
	int moveForward(const ControlMessage& ctl_msg, uint8_t ev_value);
	int moveBackward(const ControlMessage& ctl_msg, uint8_t ev_value);
	int strafLeft(const ControlMessage& ctl_msg, uint8_t ev_value);
	int strafRight(const ControlMessage& ctl_msg, uint8_t ev_value);
	int jump(const ControlMessage& ctl_msg, uint8_t ev_value);
	int crouch(const ControlMessage& ctl_msg, uint8_t ev_value);
	int prone(const ControlMessage& ctl_msg, uint8_t ev_value);
};