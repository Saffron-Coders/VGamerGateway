#pragma once

/*
* A switch-case based event handler. This was preferred instead of having separate classes for
* each event type implementing a common IEvent(say) interface.
*/
class EventMux
{
public:

	/************* Lower 2-byte of the 4-byte message **************/
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
		EV_MOUSE_LEFT_RELEASE = 12,

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


	} EventClass;

	/************* Higher 2-byte of the 4-byte message **************/
	// Each of the values defined here activates the alternative function of a MainEvent.
	// Eg: EV_RUN + EV_MOVE_FORWARD = Run forward
	//     EV_AIMDOWN + EV
	typedef enum {
		EV_RUN = 1,
		EV_WALK = 2,
		EV_SLOW = 3,
		EV_AIMDOWN = 4
	} EventValue;
};

