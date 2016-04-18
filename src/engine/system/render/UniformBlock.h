#pragma once

#include "engine/system/render/Uniform.h"

namespace ds_render
{
/**
 * A uniform block corresponds to a uniform block in a shader, a named
 * collection of uniforms.
 */
class UniformBlock
{
public:
    /**
     * Get the uniform block's name.
     *
     * @return  const std::string &, uniform block's name.
     */
    const std::string &GetName() const;

    /**
     * Set the uniform block's name.
     *
     * @param  name  const std::string &, name to set as uniform block's name.
     */
    void SetName(const std::string &name);

    /**
     * Get uniforms in the uniform block.
     *
     * @return  const std::vector<Uniform> &, uniforms to get from uniform
     * block.
     */
    const std::vector<Uniform> &GetUniforms() const;

    /**
     * Add a uniform to the uniform block.
     *
     * @param  uniform  const Uniform &, uniform to add to uniform block.
     */
    void AddUniform(const Uniform &uniform);

private:
    /** Uniform block name */
    std::string m_name;
    /** Uniforms belonging to uniform block */
    std::vector<ds_render::Uniform> m_uniforms;
};
}
