#include <winsock.h>
#include "Utils.h"
#include "ControlMessage.h"

//ControlMessage::ControlMessage()
//{
//}

int ControlMessage::serialize(uint8_t* msg, size_t len)
{
	// TODO...
	return 0;
}

int ControlMessage::deserialize(const uint8_t* msg, size_t len)
{
	int ret = 0;

	if (!msg || len < 3)
		return -1;
	
	// Type
	this->type = msg[0];
	if ((this->type != ControlMessage::MessageType::MSG_TYPE_KEY) &&
		(this->type != ControlMessage::MessageType::MSG_TYPE_MOUSE)) {
		return -1;
	}

	// Event count.
	this->nEvents = Utils::bytes2short(msg[1], msg[2]);

	int i;
	switch (this->type) {
	
	case MessageType::MSG_TYPE_KEY:
		
		// Each event entry must be 2-bytes so there must be
		// ((nEvents * 2) + 2) bytes of data starting from msg[3].
		// The additional 2-bytes for 0xffff.
		if ((len - 3) != ((nEvents * 2) + 2))
			return -1;

		for (i = 3; i < 3 + nEvents; i += 2) {
			Event event;
			event.keyEvent.eventName = msg[i];
			event.keyEvent.eventValue = msg[i + 1];
			this->eventList.push_back(event);
		}
		ret = i + 2; // Including 0xffff
		break;
	
	case MessageType::MSG_TYPE_MOUSE:
		
		// Each event entry must be 4-bytes so there must be
		// ((nEvents * 4) + 2) bytes of data starting from msg[3].
		// The additional 2-bytes for 0xffff.
		if ((len - 3) != ((nEvents * 4) + 2))
			return -1;

		for (i = 3; i < 3 + nEvents; i += 4) {
			Event event;
			event.mouseEvent.x = (short)Utils::bytes2short(msg[i], msg[i + 1]);
			event.mouseEvent.y = (short)Utils::bytes2short(msg[i + 2], msg[i + 3]);
			this->eventList.push_back(event);
		}
		ret = i + 2; // Including 0xffff

		break;
	}

	return ret;
}
