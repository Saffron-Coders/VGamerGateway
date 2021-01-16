#include <algorithm>
#include "InputProcessor.h"

#define MSG_TYPE_SEQUENCE       0x01
#define MSG_TYPE_MERGE          0x02

InputProcessor::InputProcessor()
{
    m_Handlers.insert( std::pair<uint16_t, InputHandlerFunc>(
        EventType::EV_SINGLE_SHOT, std::bind(&InputProcessor::singleShot, std::ref(*this), std::placeholders::_1))
    );
}

InputProcessor::~InputProcessor()
{
}

int InputProcessor::process(const char* msg, size_t len)
{
    ControlMessage ctl_msg;
    
    int ret = ctl_msg.deserialize(msg, len);
    if (ret < 0)
        return ret;

    switch (ctl_msg.type) {
    
    case MSG_TYPE_SEQUENCE:
        for (ControlMessage::Event event : ctl_msg.eventList) {
            InputHandlerMap::const_iterator it_handler = m_Handlers.find(event.eventName);
            if (it_handler != m_Handlers.end()) {
                it_handler->second(ctl_msg);
            }
            else { // Warning log
                fprintf(stderr, "Error: Input event %u unrecognized.\n", event.eventName);
            }
        }
        break;

    case MSG_TYPE_MERGE:
        break;
    }
    
    return 0;
}

int InputProcessor::singleShot(const ControlMessage&)
{
    printf("SINGLE SHOT\n");
    return 0;
}

int InputProcessor::sprayShot(const ControlMessage&)
{
    return 0;
}

int InputProcessor::aimdown(const ControlMessage&)
{
    return 0;
}

int InputProcessor::reload(const ControlMessage&)
{
    return 0;
}

int InputProcessor::moveForward(const ControlMessage&)
{
    return 0;
}

int InputProcessor::moveBackward(const ControlMessage&)
{
    return 0;
}

int InputProcessor::strafLeft(const ControlMessage&)
{
    return 0;
}

int InputProcessor::strafRight(const ControlMessage&)
{
    return 0;
}

int InputProcessor::jump(const ControlMessage&)
{
    return 0;
}

int InputProcessor::crouch(const ControlMessage&)
{
    return 0;
}

int InputProcessor::prone(const ControlMessage&)
{
    return 0;
}
