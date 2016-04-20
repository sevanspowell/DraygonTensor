#include "engine/system/render/Uniform.h"

namespace ds_render
{
Uniform::Uniform()
{
}

Uniform::Uniform(const std::string &name,
                 Uniform::UniformType dataType,
                 size_t dataSize,
                 const void *dataIn)
{
    m_name = name;
    m_dataType = dataType;
    m_dataBuffer.Clear();
    m_dataBuffer.Insert(dataSize, dataIn);
}

const std::string &Uniform::GetName() const
{
    return m_name;
}

void Uniform::SetName(const std::string &uniformName)
{
    m_name = uniformName;
}

Uniform::UniformType Uniform::GetDataType() const
{
    return m_dataType;
}

void Uniform::SetDataType(Uniform::UniformType dataType)
{
    m_dataType = dataType;
}

const void *Uniform::GetUniformData() const
{
    return m_dataBuffer.GetDataPtr();
}

size_t Uniform::GetUniformDataSize() const
{
    return m_dataBuffer.AvailableBytes();
}

void Uniform::SetUniformData(size_t dataSize, const void *dataIn)
{
    // Clear data already in buffer
    m_dataBuffer.Clear();
    // Insert new data
    m_dataBuffer.Insert(dataSize, dataIn);
}
}
