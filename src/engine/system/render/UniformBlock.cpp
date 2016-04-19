#include "engine/system/render/UniformBlock.h"

namespace ds_render
{
const std::string &UniformBlock::GetName() const
{
    return m_name;
}

void UniformBlock::SetName(const std::string &name)
{
    m_name = name;
}

const std::vector<Uniform> &UniformBlock::GetUniforms() const
{
    return m_uniforms;
}

void UniformBlock::AddUniform(const Uniform &uniform)
{
    m_uniforms.push_back(uniform);
}
}
