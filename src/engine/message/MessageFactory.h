#pragma once

#include "engine/message/Message.h"

namespace ds_msg
{
class MessageFactory
{
public:
    /**
     * Construct a message (contained in a MessageStream) from a message string.
     *
     * @param   messageString  std::string, string representing message to
     * create.
     * @return                 MessageStream, message stream containing message
     * and MessageHeader.
     */
    static MessageStream ConstructMessage(std::string messageString);

private:
};
}
