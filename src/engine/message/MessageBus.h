#pragma once

#include <memory>
#include <vector>

#include "engine/system/ISystem.h"

namespace ds
{
/**
 * The role of the message bus is to distribute messages across the Engine and
 * it's systems.
 */
class MessageBus
{
public:
    /**
     * Add a system to the message bus. If added successfully, the message bus
     * will collect messages from the system, distribute them to other systems
     * and post other systems messages to this system.
     *
     * The message bus does not have ownership of the system, it may be freed at
     * any time.
     *
     * Method returns TRUE if the system was added successfully and FALSE
     * otherwise.
     *
     * @param   system  std::weak_ptr<ISystem>, system to add to the message
     *                  bus.
     * @return          bool, TRUE if the system was added to the bus
     *                  successfully, FALSE otherwise.
     */
    bool AddSystem(std::weak_ptr<ISystem> system);

    /**
     * Collect all messages from the systems the message bus manages.
     *
     * @pre  Systems have been updated, otherwise they won't have generated any
     *       messages to collect.
     */
    void CollectAllMessages();

    /**
     * Broadcast all messages collected by the message bus to the systems
     * managed by the message bus.
     *
     * Purges all messages when finished.
     */
    void BroadcastAllMessages();

    /**
     * Post messages to the Message Bus.
     *
     * These messages will be broadcast across all systems. The intended usage
     * for this method is to allow the owning class (the Engine) to also
     * participate in the distribution of messages.
     *
     * @param  messages  const ds_msg::MessageStream &messages, messages to send
     * to the Message Bus.
     */
    void PostMessages(const ds_msg::MessageStream &messages);

    /**
     * Collect messages from the Message Bus.
     *
     * The intended usage for this method is to allow the owning class (the
     * Engine) to participate in the processing of messages.
     *
     * @return  ds_msg::MessageStream, stream of messages generated by this
     * system.
     */
    ds_msg::MessageStream CollectMessages();

private:
    std::vector<std::weak_ptr<ISystem>> m_systems;

    ds_msg::MessageStream m_messageStoreTemp;
};
}