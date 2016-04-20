#include "engine/message/Message.h"

namespace ds_msg
{
void AppendMessage(MessageStream *msgStream,
                   MessageType msgType,
                   size_t msgSize,
                   const void *msg)
{
    if (msgStream != nullptr)
    {
        // Construct message header
        ds_msg::MessageHeader header;
        header.type = msgType;
        header.size = msgSize;

        // Append message header first
        (*msgStream) << header;

        // Append message second
        msgStream->Insert(msgSize, msg);
    }
}
}
