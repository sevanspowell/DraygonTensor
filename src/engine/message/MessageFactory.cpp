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
