#include <winsock.h>
#include "Utils.h"
#include "ControlMessage.h"

//ControlMessage::ControlMessage()
//{
//}

int ControlMessage::serialize(char* msg, size_t len)
{
	// TODO...
	return 0;
}

int ControlMessage::deserialize(const char* msg, size_t len)
{
	if (len < 3)
		return -1;
	this->type = msg[0];

	char short_buff[2];
	short_buff[0] = msg[1];
	short_buff[1] = msg[2];
	this->nEvents = Utils::bytes2short(short_buff);

	// Each event entry must be 2-bytes so there must be
	// nEvents * 2 bytes of data starting from msg[3].
	if ((len - 3) != (nEvents * 2))
		return -1;

	for (int i = 3; i < 3 + nEvents; i += 2) {
		this->eventList.push_back(Event(msg[i], msg[i + 1]));
	}
	return 0;
}
