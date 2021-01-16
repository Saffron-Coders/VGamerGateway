#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>
#include "ControlMessage.h"

/************* Event values for the control message **************/
typedef enum {

	// Low-level event types, i.e., very specific to device type.
	// Eg: Keyboard specific or mouse specific events.
	EV_KEY_PRESS = 1,
	EV_KEY_RELEASE = 2,
	EV_KEY_HOLD = 3,
	EV_MOUSE_LEFT_PRESS = 4,
	EV_MOUSE_LEFT_RELEASE = 5,
	EV_MOUSE_LEFT_HOLD = 6,
	EV_MOUSE_RIGHT_PRESS = 7,
	EV_MOUSE_RIGHT_RELEASE = 8,
	EV_MOUSE_RIGHT_HOLD = 9,
	EV_MOUSE_SCROLL_UP = 10,
	EV_MOUSE_SCROLL_DOWN = 11,

	// High-level events that may directly resemble game specific actions directly.
	// The client-side controller application will mostly send these commands over the network.
	EV_SINGLE_SHOT = 13,
	EV_SPRAY_SHOT = 14,
	EV_RELOAD = 15,
	EV_AIMDOWN = 16,
	EV_MOVE_FORWARD = 17,
	EV_MOVE_BACKWARD = 18,
	EV_STRAF_LEFT = 19,
	EV_STRAF_RIGHT = 20,
	EV_JUMP = 21,
	EV_CROUCH = 22,
	EV_PRONE = 23,
	EV_RUN = 24,
	EV_WALK = 25,
	EV_SLOW = 26,

} EventType;

class InputProcessor
{
public:

	InputProcessor();
	~InputProcessor();

	int process(const char* msg, size_t len);

	/// Typedef for control message handler function.
	typedef std::function<int(const ControlMessage&)> InputHandlerFunc;
	typedef std::unordered_map<uint16_t, InputHandlerFunc>	InputHandlerMap;
private:

	InputHandlerMap m_Handlers;

	// -------- All the handler functions ----------

	int singleShot(const ControlMessage&);
	int sprayShot(const ControlMessage&);
	int aimdown(const ControlMessage&);
	int reload(const ControlMessage&);
	int moveForward(const ControlMessage&);
	int moveBackward(const ControlMessage&);
	int strafLeft(const ControlMessage&);
	int strafRight(const ControlMessage&);
	int jump(const ControlMessage&);
	int crouch(const ControlMessage&);
	int prone(const ControlMessage&);
};