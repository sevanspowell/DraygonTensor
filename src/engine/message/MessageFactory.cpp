#include "engine/message/MessageFactory.h"

namespace ds_msg
{
MessageStream MessageFactory::ConstructMessage(std::string messageString)
{
    MessageStream stream;

    if (messageString == "console_toggle")
    {
        MessageHeader header;
        header.type = ds_msg::MessageType::ConsoleToggle;
        header.size = sizeof(ds_msg::ConsoleToggle);

        ConsoleToggle consoleToggle;

        stream << header << consoleToggle;
    }
    // else if (messageString == "move_forward")
    // {
    //     MessageHeader header;
    //     header.type = ds_msg::MessageType::MoveForward;
    //     header.size = sizeof(ds_msg::MoveForward);

    //     MoveForward moveForward;

    //     stream << header << moveForward;
    // }
    // else if (messageString == "move_backward")
    // {
    //     MessageHeader header;
    //     header.type = ds_msg::MessageType::MoveBackward;
    //     header.size = sizeof(ds_msg::MoveBackward);

    //     MoveBackward moveBackward;

    //     stream << header << moveBackward;
    // }
    // else if (messageString == "strafe_left")
    // {
    //     MessageHeader header;
    //     header.type = ds_msg::MessageType::StrafeLeft;
    //     header.size = sizeof(ds_msg::StrafeLeft);

    //     StrafeLeft strafeLeft;

    //     stream << header << strafeLeft;
    // }
    // else if (messageString == "strafe_right")
    // {
    //     MessageHeader header;
    //     header.type = ds_msg::MessageType::StrafeRight;
    //     header.size = sizeof(ds_msg::StrafeRight);

    //     StrafeRight strafeRight;

    //     stream << header << strafeRight;
    // }
    else if (messageString == "quit")
    {
        MessageHeader header;
        header.type = ds_msg::MessageType::QuitEvent;
        header.size = sizeof(ds_msg::QuitEvent);

        QuitEvent quitEvent;

        stream << header << quitEvent;
    }

    return stream;
}
}
