#include <algorithm>
#include <Windows.h>
#include "InputProcessor.h"

InputProcessor::InputProcessor()
{
    m_Handlers.insert( std::pair<uint16_t, InputHandlerFunc>(
        ControlMessage::EventType::EV_SHOOT, std::bind(&InputProcessor::shoot, std::ref(*this), std::placeholders::_1, std::placeholders::_2))
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
    
    case ControlMessage::MessageType::MSG_TYPE_SEQUENCE:
        for (ControlMessage::Event event : ctl_msg.eventList) {
            InputHandlerMap::const_iterator it_handler = m_Handlers.find(event.eventName);
            if (it_handler != m_Handlers.end()) {
                it_handler->second(ctl_msg, event.eventValue);
            }
            else { // Warning log
                fprintf(stderr, "Error: Input event %u unrecognized.\n", event.eventName);
            }
        }
        break;

    case ControlMessage::MessageType::MSG_TYPE_MERGE:
        break;
    }
    
    return 0;
}

int InputProcessor::shoot(const ControlMessage& ctl_msg, uint8_t ev_value)
{
    printf("SHOOT -> ");
    
    UINT ret;
    INPUT inputs[2];
    ZeroMemory(inputs, 2);
    
    switch (ev_value) {
    
    case 0: // Stop
        // Just release left-click.
        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.mouseData = XBUTTON1;
        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
        ret = SendInput(1, inputs, sizeof(INPUT));
        printf("STOP\n");
        break;
    
    case 1: // Single shot
        // Press left-click
        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.mouseData = XBUTTON1;
        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        // Release left-click
        inputs[1].type = INPUT_MOUSE;
        inputs[1].mi.mouseData = XBUTTON1;
        inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
        ret = SendInput(2, inputs, sizeof(INPUT));
        printf("SINGLE\n");
        break;
    
    case 2: // Hold/spray
        // Press left-click and dont release.
        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.mouseData = XBUTTON1;
        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        ret = SendInput(1, inputs, sizeof(INPUT));
        printf("SPRAY\n");
        break;
    
    default:
        printf("??\n");
        return -1;
    }
    return 0;
}

int InputProcessor::aimdown(const ControlMessage& ctl_msg, uint8_t ev_value)
{
    return 0;
}

int InputProcessor::reload(const ControlMessage& ctl_msg, uint8_t ev_value)
{
    return 0;
}

int InputProcessor::moveForward(const ControlMessage& ctl_msg, uint8_t ev_value)
{
    return 0;
}

int InputProcessor::moveBackward(const ControlMessage& ctl_msg, uint8_t ev_value)
{
    return 0;
}

int InputProcessor::strafLeft(const ControlMessage& ctl_msg, uint8_t ev_value)
{
    return 0;
}

int InputProcessor::strafRight(const ControlMessage& ctl_msg, uint8_t ev_value)
{
    return 0;
}

int InputProcessor::jump(const ControlMessage& ctl_msg, uint8_t ev_value)
{
    return 0;
}

int InputProcessor::crouch(const ControlMessage& ctl_msg, uint8_t ev_value)
{
    return 0;
}

int InputProcessor::prone(const ControlMessage& ctl_msg, uint8_t ev_value)
{
    return 0;
}
