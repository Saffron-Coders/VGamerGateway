#pragma once

#include <cstdint>
#include <vector>

/*
 *         1b               2b               2b/4b          2b/4b                   2b/4b                  2b
 *   +-------------+-------------------+---------------+--------------+--------+--------------+----------------------+
 *   |   Type      |  N(no. of events) |   Event[0]    |   Event[1]   |  ....  |  Event[N-1]  | Termination (0xffff) |
 *   +-------------+-------------------+---------------+--------------+--------+--------------+----------------------+
 *
 * - Type(1-byte): Specifies how each events in the event list are to be processed
 * - Values:
 *   > KEY(0x01): Process events sequentially and independently as single key press events starting from Event[0] till Event[N-1].
 *   > MOUSE(0x02): Consider each event as a 4-bytes word where the first half-word(2-byte) represents x-axis value and the 2nd y-axis.
 *
 * - N(2-bytes): Number of events in the event list.
 *
 * - Event[]: A sequence of events according to "Type".
 *   For Type == 0x01,
 *     sizeof(Event[i]) = 2 bytes, therefore,
 *     sizeof(Event[]) = (2*N) bytes.
 *     > 1st byte = Event_Type, 2nd byte = Value.
 * 
 *   For Type == 0x02,
 *     sizeof(Event[i]) = 4 bytes, therefore,
 *     sizeof(Event[]) = (4*N) bytes.
 *     > 1st half-word = (+/-)X-Axis, 2nd half-word = (+/-)Y-Axis.
 * 
 * - Termination(2-bytes): Message termination sequence. Value = 0xffff
 */
struct ControlMessage
{
	typedef enum {
		MSG_TYPE_KEY		= 0x01,
		MSG_TYPE_MOUSE		= 0x02
	} MessageType;

	/************* Event values for the control message **************/
	typedef enum {

		// Low-level event types, i.e., very specific to device type.
		// Eg: Keyboard specific or mouse specific events.

		EV_KEY_PRESS = 1,
		EV_KEY_RELEASE = 2,
		EV_MOUSE_HOLD = 3,
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
		EV_SHOOT = 13, ///> values = 0(stop), 1(single), 2(hold)
		EV_RELOAD = 14,
		EV_AIMDOWN = 15, ///> values = 0(release), 1(aimdown)
		EV_MOVE_FORWARD = 16, ///> values = 0(slow), 1(walk), 2(sprint)
		EV_MOVE_BACKWARD = 17,
		EV_STRAF_LEFT = 18,
		EV_STRAF_RIGHT = 19,
		EV_JUMP = 20,			///> values = 0(release), 1(once), 2(hold)
		EV_CROUCH = 21,
		EV_PRONE = 22,
		EV_RUN = 23,
		EV_WALK = 24,
		EV_SLOW = 25,

	} EventType;

	uint8_t type;
	uint16_t nEvents;
	
	union Event {
		// Type 0x01
		struct KeyEvent {
			uint8_t eventName, eventValue; ///> Event name and value
		} keyEvent ;

		// Type 0x02
		struct MouseEvent {
			short x, y; ///> X/Y axis
		} mouseEvent;
	};

	std::vector<Event> eventList;

	//ControlMessage();

	///  Serialize the message object to string and return to (msg,len)
	/// Used when sending message.
	int serialize(uint8_t* msg, size_t len);

	/// Deserializes string message to this object.
	/// Used when receiving message.
	/// @msg: Buffer containing raw bytes of message.
	/// @len: Length of the whole UDP payload received from a client.
	/// Returns the byte size of the parsed message out of
	/// the whole @len sized chunk.
	int deserialize(const uint8_t* msg, size_t len);
};

