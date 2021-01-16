#pragma once

#include <cstdint>
#include <vector>

/*
 *         1b               2b               2b              2b                       2b
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
	uint8_t type;
	uint16_t nEvents;
	std::vector<uint8_t> eventList;

	//ControlMessage();

	///  Serialize the message object to string and return to (msg,len)
	/// Used when sending message.
	int serialize(char *msg, size_t len);

	/// Deserializes string message to this object.
	/// Used when receiving message.
	int deserialize(const char* msg, size_t len);
};

