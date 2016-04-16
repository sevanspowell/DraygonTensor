#include "engine/common/StringIntern.h"
#include "engine/message/MessageHelper.h"
#include "engine/system/Physics.h"

namespace ds
{
	bool Physics::Initialize(const Config &config)
	{
		ds_msg::SystemInit initMsg;
		initMsg.systemName = "Physics";

		ds_msg::AppendMessage(&m_messagesGenerated, ds_msg::MessageType::SystemInit,
			sizeof(ds_msg::SystemInit), &initMsg);

		return true;
	}

	void Physics::Update(float deltaTime)
	{
		ProcessEvents(&m_messagesGenerated);
	}

	void Physics::Shutdown()
	{

	}

	void Physics::PostMessages(const ds_msg::MessageStream &messages)
	{
		AppendStreamBuffer(m_messagesReceived, messages);
	}

	ds_msg::MessageStream Physics::CollectMessages()
	{
		ds_msg::MessageStream tmp = m_messagesGenerated;
		m_messagesGenerated.Clear();
		return tmp;
	}

	void Physics::ProcessEvents(ds_msg::MessageStream *messages)
	{
		while (messages->AvailableBytes() != 0)
		{
			ds_msg::MessageHeader header;
			(*messages) >> header;

			switch (header.type)
			{
			default:
				messages->Extract(header.size);
				break;
			}
		}
	}


}