#include "engine/system/render/ShaderParameter.h"

namespace ds_render
{
ShaderParameter::ShaderParameter()
{
}

ShaderParameter::ShaderParameter(const std::string &name,
                 ShaderParameter::ShaderParameterType dataType,
                 size_t dataSize,
                 const void *dataIn)
{
    m_name = name;
    m_dataType = dataType;
    m_dataBuffer.Clear();
    m_dataBuffer.Insert(dataSize, dataIn);
}

const std::string &ShaderParameter::GetName() const
{
    return m_name;
}

void ShaderParameter::SetName(const std::string &uniformName)
{
    m_name = uniformName;
}

ShaderParameter::ShaderParameterType ShaderParameter::GetDataType() const
{
    return m_dataType;
}

void ShaderParameter::SetDataType(ShaderParameter::ShaderParameterType dataType)
{
    m_dataType = dataType;
}

const void *ShaderParameter::GetData() const
{
    return m_dataBuffer.GetDataPtr();
}

size_t ShaderParameter::GetDataSize() const
{
    return m_dataBuffer.AvailableBytes();
}

void ShaderParameter::SetData(size_t dataSize, const void *dataIn)
{
    // Clear data already in buffer
    m_dataBuffer.Clear();
    // Insert new data
    m_dataBuffer.Insert(dataSize, dataIn);
}
}
