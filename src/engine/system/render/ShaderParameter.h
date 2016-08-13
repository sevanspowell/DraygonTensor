#pragma once

#include "engine/common/StreamBuffer.h"
#include "engine/system/render/RenderCommon.h"

namespace ds_render
{
/**
 * A shader parameter is a shader data binding point. It is used to get data to
 * the shader.
 *
 * A shader parameter is made up of a name, a data type and some data. The
 * sizeof the
 * data type should match the size of the data stored in the shader parameter.
 */
class ShaderParameter
{
public:
    /** Type of the shader parameter data */
    enum class ShaderParameterType
    {
        Float,
        Int,
        Matrix4,
        Vector3,
        Vector4,
    };

    /**
     * ShaderParameter default constructor
     */
    ShaderParameter();

    /**
     * ShaderParameter constructor.
     *
     * @param  name      const std::string &, name of the shader parameter.
     * @param  dataType  ShaderParameterType, data type of the shader parameter.
     * @param  dataSize  size_t, size of the shader parameter data.
     * @param  dataIn    const void *, pointer to shader parameter data.
     */
    ShaderParameter(const std::string &name,
                    ShaderParameterType dataType,
                    size_t dataSize,
                    const void *dataIn);

    /**
     * Get the shader parameter name.
     *
     * @return  const std::string &, shader parameter name.
     */
    const std::string &GetName() const;

    /**
     * Set the shader parameter name.
     *
     * @param  shader parameterName  const std::string &, shader parameter name
     * to set.
     */
    void SetName(const std::string &parameterName);

    /**
     * Get the data type of the shader parameter.
     *
     * @return  ShaderParameterType, data type of the shader parameter.
     */
    ShaderParameterType GetDataType() const;

    /**
     * Set the data type of the shader parameter.
     *
     * @param  ShaderParameterType, data type of the shader parameter
     */
    void SetDataType(ShaderParameterType dataType);

    /**
     * Get the shader parameter's data.
     *
     * @return  const void *, shader parameter data.
     */
    const void *GetData() const;

    /**
     * Set the shader parameter data.
     *
     * @param  dataSize  size_t, size of the passed in data (in bytes).
     * @param  dataIn    const void *, pointer to shader parameter data to set.
     */
    void SetData(size_t dataSize, const void *dataIn);

    /**
     * Get shader parameter data size.
     *
     * @return  size_t, size of the shader parameter data.
     */
    size_t GetDataSize() const;

private:
    /** ShaderParameter name */
    std::string m_name;
    /** ShaderParameter data type */
    ShaderParameterType m_dataType;
    /** ShaderParameter data */
    ds_com::StreamBuffer m_dataBuffer;
};
}
