#include <string>
#include "math/Vector3.h"

namespace ds
{
	class PhysicsHandler
	{
	public:
		PhysicsHandler();
		~PhysicsHandler();
		void ConstructEntity(int entityID, std::string modelPath, bool staticObject);
		void SetEntityPosition(int entityID, ds_math::Vector3 pos);
		ds_math::Vector3 GetEntityPosition(int entityID);
		void ApplyRotationToEntity(int entityID, ds_math::Vector3 axis, float angle);
		void SetWorldGravity(float gravAmount);
		void SetEntityMess(int entityID, float mass);
		void SetEntityGravity(int entityID, ds_math::Vector3 grav);
		void SetEntityAngDampening(int entityId, float damp);
		void SetEntityFriction(int entityID, float fric);
		void RemoveEntity(int entityID);
		bool DoesEntityExist(int entityID);
		void SetPhysicsWorldIntertia(float inert);
		


	private:


	};
}