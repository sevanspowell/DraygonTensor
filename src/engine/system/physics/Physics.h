#pragma once

#include <vector>

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

#include "engine/common/HandleManager.h"
#include "engine/resource/ResourceFactory.h"
#include "engine/resource/TerrainResource.h"
#include "engine/system/ISystem.h"
#include "engine/system/physics/PhysicsComponentManager.h"
#include "engine/system/scene/TransformComponentManager.h"
#include "engine/system/render/TerrainComponentManager.h"

namespace ds
{
class Physics : public ISystem
{

public:
    struct Raycast
    {
        ds_math::Vector3 start;
        ds_math::Vector3 end;
        ds_math::Vector3 normal;
        bool hasHit;
    };

    virtual bool Initialize(const Config &config);
    virtual void Update(float deltaTime);
    virtual void Shutdown();
    virtual void PostMessages(const ds_msg::MessageStream &messages);
    virtual ds_msg::MessageStream CollectMessages();

    /**
     * @copydoc ISystem::GetName()
     */
    virtual const char *GetName() const;

    /**
     * Return required script bindings.
     *
     * @return  ScriptBindingSet, the script bindings the physics system wants
     * to register with the Script system.
     */
    virtual ScriptBindingSet GetScriptBindings() const;

    /**
     * Get the height of the given terrain entity at the given position.
     *
     * @param   entity    Entity, the terrain entity to query the height of.
     * @param   position  const ds_math::Vector3 &, position to get terrain
     *                    height at.
     * @return            float, terrain height at the given position.
     */
    float GetTerrainHeight(Entity entity, const ds_math::Vector3 &position);

    /**
     * Perform a raycast into the physics world.
     *
     * If the raycast hits something (raycast.hasHit == true) then the raycast
     * end position will be where the ray hit something and the raycast normal
     * is a world-space normal of the surface hit. If the ray cast doesn't hit
     * anything (raycast.hasHit == false) then the end position will be the
     * rayEnd and the normal will be a zero vector.
     *
     * @param   rayStart  const ds_math::Vector3 &, start of the ray.
     * @param   rayEnd    const ds_math::Vector3 &, end of the ray.
     * @return            Raycast, struct containing raycast hit information.
     */
    Raycast PerformRaycast(const ds_math::Vector3 &rayStart,
                           const ds_math::Vector3 &rayEnd);
    /**
     * Set the linear velocity of the rigidbody associated with the given entity
     * (if any).
     *
     * @param  entity    Entity, entity to set the linear velocity of.
     * @param  velocity  const ds_math::Vector3 &, linear velocity to set.
     */
    void SetLinearVelocity(Entity entity, const ds_math::Vector3 &velocity);

    /**
     * Get the linear velocity of the rigidbody associated with the given entity
     * (if any).
     *
     * @param   entity  Entity, entity to get the linear velocity of.
     * @return          ds_math::Vector3, the linear velocity of the
     *                  entity.
     */
    ds_math::Vector3 GetLinearVelocity(Entity entity) const;

private:
    /**
     * Process messages in the given message stream.
     *
     * @param  ds_msg::MessageStream *, messages to process.
     */
    void ProcessEvents(ds_msg::MessageStream *messages);

    /**
     * Create a Bullet rigid body from the given parameters and add it to the
     * Physics system.
     *
     * @param   phys    Instance, physics instance to add rigid body to.
     * @param   origin  const ds_math::Vector3 &, origin of rigid body.
     * @param   shape   StringIntern::StringId, interned string that will give
     * string representing collision shape of rigidbody.
     * @param   scale   const ds_math::Vector3 &, scale of rigid body.
     * @param   mass    float, mass of the rigid body. 0 for a static rigid
     * body.
     */
    void CreateRigidBody(Instance phys,
                         const ds_math::Vector3 &origin,
                         StringIntern::StringId shape,
                         const ds_math::Vector3 &scale,
                         float mass);

    /**
     * Create a Bullet rigid body for a height map from the given
     * TerrainResource, modify the terrain resource to be scaled by the
     * given
     * heightscale.
     *
     * @param   phys    Instance, physics instance to add rigid body to.
     * @param   origin           const ds_math::Vector3 &, origin of rigid
     *                           body.
     * @param   terrainResource  TerrainResource *, pointer to terrain
     * resource
     * to construct rigidbody from.
     * @param   heightScale      float, factor to scale terrain height by.
     * @return                   btRigidBody *, pointer to rigidbody created.
     */
    void CreateHeightMapRigidBody(Instance phys,
                                  const ds_math::Vector3 &origin,
                                  TerrainResource *terrainResource,
                                  float heightScale);

    /**
     * Create a Bullet ghost object from the given parameters and add it to the
     * Physics system.
     *
     * @param   phys    Instance, component instance to add ghost object to.
     * @param   origin  const ds_math::Vector3 &, origin of ghost object.
     * @param   shape   StringIntern::StringId, interned string that will give
     * string representing collision shape of ghost object.
     * @param   scale   const ds_math::Vector3 &, scale of ghost object.
     * @return          btRigidBody *, pointer to ghost object
     * created or nullptr if failed.
     */
    void CreateGhostObject(Instance phys,
                           const ds_math::Vector3 &origin,
                           StringIntern::StringId shape,
                           const ds_math::Vector3 &scale);

    /**
     * Create a Bullet dynamic character controller.
     *
     * btRigidBody returned should be freed by the caller.
     *
     * @param   phys    Instance, component instance to add player capsule to.
     * @param   origin  const ds_math::Vector3 &, origin of rigid body.
     * @param   radius  float, radius of the character capsule.
     * @param   height  float, height of the character capsule.
     * @param   mass    float, mass of the character shape.
     */
    void CreatePlayerCapsule(Instance phys,
                             const ds_math::Vector3 &origin,
                             float radius,
                             float height,
                             float mass);

    btDiscreteDynamicsWorld *m_dynamicsWorld;
    btSequentialImpulseConstraintSolver *m_solver;
    btBroadphaseInterface *m_overlappingPairCache;
    btCollisionDispatcher *m_dispatcher;
    btDefaultCollisionConfiguration *m_collisionConfiguration;
    btAlignedObjectArray<btCollisionShape *> m_collisionShapes;

    /** Transform component manager */
    TransformComponentManager m_transformComponentManager;
    /** Physics component manager */
    PhysicsComponentManager m_physicsComponentManager;
    /** Terrain component manager */
    ds_render::TerrainComponentManager m_terrainComponentManager;

    /** Resource factory */
    ResourceFactory m_factory;

    /** Heightmap data */
    std::vector<std::vector<float>> m_heightmapData;

    /** Manage storage of terrain resources */
    ds::HandleManager m_handleManager;

    ds_msg::MessageStream m_messagesGenerated, m_messagesReceived;

    /** TRUE if paused, FALSE otherwise */
    bool m_isPaused;
};
}
