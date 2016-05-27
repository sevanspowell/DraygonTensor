#pragma once

#include "engine/system/render/Mesh.h"
#include "engine/system/render/Material.h"

namespace ds_render
{
class Skybox
{
public:
    /**
     * Get the skybox's mesh.
     *
     * @return  Mesh, skybox mesh.
     */
    Mesh GetMesh() const;

    /**
     * Set the skybox's mesh.
     *
     * @param  mesh  Mesh, skybox's mesh.
     */
    void SetMesh(Mesh mesh);

private:
    /** Skybox mesh */
    Mesh m_mesh;
};
}
