#include "engine/message/Message.h"

namespace ds_msg
{
/**
 * Append a message to the given message stream (helper function);
 *
 * @param  msgStream  MessageStream *, message stream to append message to.
 * @param  msgType    MessageType, type of the message you wish to append.
 * @param  msgSize    size_t, size of the message to append.
 * @param  msg        const void *, pointer to message payload.
 */
void AppendMessage(MessageStream *msgStream,
                   MessageType msgType,
                   size_t msgSize,
                   const void *msg);
}
