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

private:
    void ProcessEvents(ds_msg::MessageStream *messages);
    ds_msg::MessageStream m_messagesGenerated, m_messagesReceived;

    /**
     * Create a Bullet rigid body from the given parameters.
     *
     * btRigidBody should be freed by user.
     *
     * @param   origin  const ds_math::Vector3 &, origin of rigid body.
     * @param   shape   StringIntern::StringId, interned string that will give
     * string representing collision shape of rigidbody.
     * @param   scale   const ds_math::Vector3 &, scale of rigid body.
     * @param   mass    float, mass of the rigid body. 0 for a static rigid
     * body.
     * @return          btRigidBody *, pointer to rigid body created or nullptr
     * if failed.
     */
    btRigidBody *CreateRigidBody(const ds_math::Vector3 &origin,
                                 StringIntern::StringId shape,
                                 const ds_math::Vector3 &scale,
                                 float mass);

    /**
     * Create a Bullet rigid body for a height map from the given
     * TerrainResource, modify the terrain resource to be scaled by the given
     * heightscale.
     *
     * btRigidBody should be freed by user.
     *
     * @param   origin           const ds_math::Vector3 &, origin of rigid
     *                           body.
     * @param   terrainResource  TerrainResource *, pointer to terrain resource
     * to construct rigidbody from.
     * @param   heightScale      float, factor to scale terrain height by.
     * @return                   btRigidBody *, pointer to rigid body created or
     * nullptr if failed.
     */
    btRigidBody *CreateHeightMapRigidBody(const ds_math::Vector3 &origin,
                                          TerrainResource *terrainResource,
                                          float heightScale);

    /**
     * Create a Bullet ghost object from the given parameters.
     *
     * btGhostObject should be freed by user.
     *
     * @param   origin  const ds_math::Vector3 &, origin of ghost object.
     * @param   shape   StringIntern::StringId, interned string that will give
     * string representing collision shape of ghost object.
     * @param   scale   const ds_math::Vector3 &, scale of ghost object.
     * @return          btRigidBody *, pointer to ghost object
     * created or nullptr if failed.
     */
    btRigidBody *CreateGhostObject(const ds_math::Vector3 &origin,
                                   StringIntern::StringId shape,
                                   const ds_math::Vector3 &scale);

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
};
}
