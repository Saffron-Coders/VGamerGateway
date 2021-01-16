#include "ControlMessage.h"

//ControlMessage::ControlMessage()
//{
//}

int ControlMessage::serialize(char* msg, size_t len)
{
	return 0;
}

int ControlMessage::deserialize(const char* msg, size_t len)
{
	if (len < 3)
		return -1;
	this->type = msg[0];
	this->nEvents = (uint16_t)msg[1];
	for(int i = 3; i < nEvents; i += 2)
		this->eventList.push_back((uint16_t)msg[i]);
	return 0;
}
