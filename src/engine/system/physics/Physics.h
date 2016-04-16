#pragma once
#include "engine/system/ISystem.h"

namespace ds
{
	class Physics : public ISystem
	{

	public:
		
		virtual bool Initialize(const Config &config);
		virtual void Update(float deltaTime);
		virtual void Shutdown();
		virtual void PostMessages(const ds_msg::MessageStream &messages);
		virtual ds_msg::MessageStream CollectMessages();

	private:

		void ProcessEvents(ds_msg::MessageStream *messages);
		ds_msg::MessageStream m_messagesGenerated, m_messagesReceived;
	};
}