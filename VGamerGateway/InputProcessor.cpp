#include <algorithm>
#include <Windows.h>
#include "ScanCode.h"
#include "InputProcessor.h"

InputProcessor::InputProcessor()
{
    m_Handlers.insert( std::pair<uint16_t, InputHandlerFunc>(
        ControlMessage::EventType::EV_SHOOT, std::bind(&InputProcessor::shoot, std::ref(*this), std::placeholders::_1, std::placeholders::_2))
    );

    m_Handlers.insert(std::pair<uint16_t, InputHandlerFunc>(
        ControlMessage::EventType::EV_AIMDOWN, std::bind(&InputProcessor::aimdown, std::ref(*this), std::placeholders::_1, std::placeholders::_2))
    );

    m_Handlers.insert(std::pair<uint16_t, InputHandlerFunc>(
        ControlMessage::EventType::EV_MOVE_FORWARD, std::bind(&InputProcessor::moveForward, std::ref(*this), std::placeholders::_1, std::placeholders::_2))
    );

    m_Handlers.insert(std::pair<uint16_t, InputHandlerFunc>(
        ControlMessage::EventType::EV_JUMP, std::bind(&InputProcessor::jump, std::ref(*this), std::placeholders::_1, std::placeholders::_2))
    );
}

InputProcessor::~InputProcessor()
{
}

int InputProcessor::process(const uint8_t* msg, size_t len)
{
    ControlMessage ctl_msg;
    
    int ret = ctl_msg.deserialize(msg, len);
    if (ret <= 0)
        return ret;
    
    // Sequentially process events.
	for (ControlMessage::Event event : ctl_msg.eventList) {

        switch (ctl_msg.type) {
        case ControlMessage::MessageType::MSG_TYPE_KEY:
        {
            InputHandlerMap::const_iterator it_handler = m_Handlers.find(event.keyEvent.eventName);
            if (it_handler != m_Handlers.end()) {
                it_handler->second(ctl_msg, event.keyEvent.eventValue);
            }
            else { // Warning log
                fprintf(stderr, "Error: Input event %u unrecognized.\n", event.keyEvent.eventName);
            }
            break;
        }
        case ControlMessage::MessageType::MSG_TYPE_MOUSE:
        {
            mouseMove(ctl_msg, event.mouseEvent.x, event.mouseEvent.y);
            break;
        }
        }
	}

    return ret;
}

int InputProcessor::mouseMove(const ControlMessage& ctl_msg, short x, short y)
{
    printf("MOUSE_MOVE -> %d, %d\n", x, y);
    
    UINT ret;
    INPUT input;
    ZeroMemory(&input, sizeof(INPUT));

    // Relatively move mouse position.
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.mouseData = 0;
    input.mi.dx = x; // (long)(x * (65536.0f / GetSystemMetrics(SM_CXSCREEN)));
    input.mi.dy = y; // (long)(y * (65536.0f / GetSystemMetrics(SM_CYSCREEN)));
    ret = SendInput(1, &input, sizeof(INPUT));
    if (ret <= 0) {
        fprintf(stderr, "Error(%d): SendInput() returned %d\n", GetLastError(), ret);
    }

    return 0;
}

int InputProcessor::shoot(const ControlMessage& ctl_msg, uint8_t ev_value)
{
    printf("SHOOT -> ");
    
    UINT ret;
    INPUT inputs[2];
    static INPUT prev_input = { 0 };
    ZeroMemory(inputs, 2);
    
    switch (ev_value) {
    
    case 0: // Stop
        if (prev_input.type == INPUT_MOUSE) {
            if (prev_input.mi.dwFlags == MOUSEEVENTF_LEFTDOWN)
                prev_input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            if (prev_input.mi.dwFlags == MOUSEEVENTF_RIGHTDOWN)
                prev_input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            ret = SendInput(1, &prev_input, sizeof(INPUT));
            if (ret <= 0) {
                fprintf(stderr, "Error(%d): SendInput() returned %d\n", GetLastError(), ret);
            }
        }
        printf("STOP\n");
        ZeroMemory(&prev_input, sizeof(INPUT));
        break;
    
    case 1: // Single shot
        // Press left-click
        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.mouseData = XBUTTON1;
        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        ret = SendInput(1, inputs, sizeof(INPUT));
        if (ret <= 0) {
            fprintf(stderr, "Error(%d): SendInput() returned %d\n", GetLastError(), ret);
        }
        Sleep(50);
        // Release left-click
        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.mouseData = XBUTTON1;
        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTUP;
        ret = SendInput(1, inputs, sizeof(INPUT));
        if (ret <= 0) {
            fprintf(stderr, "Error(%d): SendInput() returned %d\n", GetLastError(), ret);
        }
        printf("SINGLE\n");
        break;
    
    case 2: // Hold/spray
        // Press left-click and dont release.
        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.mouseData = XBUTTON1;
        inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
        ret = SendInput(1, inputs, sizeof(INPUT));
        if (ret <= 0) {
            fprintf(stderr, "Error(%d): SendInput() returned %d\n", GetLastError(), ret);
        }
        printf("SPRAY\n");
        prev_input = inputs[0];
        break;
    
    default:
        printf("??\n");
        return -1;
    }
    return 0;
}

int InputProcessor::aimdown(const ControlMessage& ctl_msg, uint8_t ev_value)
{
    printf("AIMDOWN -> ");
    return 0;
}

int InputProcessor::reload(const ControlMessage& ctl_msg, uint8_t ev_value)
{
    return 0;
}

int InputProcessor::moveForward(const ControlMessage& ctl_msg, uint8_t ev_value)
{
    printf("MOVE_FORWARD -> ");

    UINT ret;
    INPUT inputs[2]; // Holds previous command too.
    static INPUT prev_input = { 0 };
    ZeroMemory(inputs, 2);

    switch (ev_value) {

    case 0: // Stop
        if (prev_input.type == INPUT_KEYBOARD) {
            prev_input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
            ret = SendInput(1, &prev_input, sizeof(INPUT));
            if (ret <= 0) {
                fprintf(stderr, "Error(%d): SendInput() returned %d\n", GetLastError(), ret);
            }
        }
        printf("STOP\n");
        break;
    case 1: // Slow
        break;
    case 2: // Walk/Normal
        // Press left-click
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE; // Keydown
        inputs[0].ki.wScan = ScanCode::XSet3::KEY_W;
        ret = SendInput(1, inputs, sizeof(INPUT));
        if (ret <= 0) {
            fprintf(stderr, "Error(%d): SendInput() returned %d\n", GetLastError(), ret);
        }
        printf("NORMAL\n");
        prev_input = inputs[0];
        break;

    case 3: // Sprint
        break;

    default:
        printf("??\n");
        return -1;
    }

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
    printf("JUMP -> ");

    UINT ret;
    INPUT inputs[2];
    ZeroMemory(inputs, 2);

    switch (ev_value) {

    case 0: // Release
        break;
    case 1: // Once
        // Press left-click
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE; // Keydown
        inputs[0].ki.wScan = ScanCode::XSet3::KEY_SPACE;
        ret = SendInput(1, inputs, sizeof(INPUT));
        if (ret <= 0) {
            fprintf(stderr, "Error(%d): SendInput() returned %d\n", GetLastError(), ret);
        }
        Sleep(200);
        inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP; // Keyup
        ret = SendInput(1, inputs, sizeof(INPUT));
        if (ret <= 0) {
            fprintf(stderr, "Error(%d): SendInput() returned %d\n", GetLastError(), ret);
        }
        printf("ONCE\n");
        break;

    case 2: // Hold
        break;

    default:
        printf("??\n");
        return -1;
    }

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
