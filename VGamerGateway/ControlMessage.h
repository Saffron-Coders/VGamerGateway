#pragma once

#include <cstdint>
#include <vector>

/*
 *         8b               16b               16b            16b                     16b
 *   +-------------+-------------------+---------------+--------------+--------+--------------+
 *   |   Type      |  N(no. of events) |   Event[0]    |   Event[1]   |  ....  |  Event[N-1]  |
 *   +-------------+-------------------+---------------+--------------+--------+--------------+
 *
 * - Type(1-byte): Specifies how each events in the event list are to be processed
 * - Values:
 *   > SEQUENCE(0x01): Process events sequentially and independently starting from Event[0] till Event[N-1].
 *   > MERGE(0x02): Create a merged event out of the all events in event list. Event[0] represent the main event type.
 *     Reset of the Events defines the behaviour.
 *
 * - N(2-byte): Number of events in the event list.
 *
 * - Event[] ((2*N)-bytes): A sequence of events. Each of size 2-byte.
 *   1st byte represents event type and 2nd byte represents value.
 */
struct ControlMessage
{
	typedef enum {
		MSG_TYPE_SEQUENCE	= 0x01,
		MSG_TYPE_MERGE		= 0x02
	} MessageType;

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
	struct Event {
		uint8_t eventName, eventValue; ///> Event name and value
		Event(uint8_t ev_name, uint8_t ev_value) :
			eventName(ev_name), eventValue(ev_value)
		{}
	};
	std::vector<Event> eventList;

	//ControlMessage();

	///  Serialize the message object to string and return to (msg,len)
	/// Used when sending message.
	int serialize(uint8_t* msg, size_t len);

	/// Deserializes string message to this object.
	/// Used when receiving message.
	int deserialize(const uint8_t* msg, size_t len);
};

