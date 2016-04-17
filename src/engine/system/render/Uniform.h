#pragma once

#include "engine/common/StreamBuffer.h"
#include "engine/system/render/RenderCommon.h"

namespace ds_render
{
/**
 * A uniform is a shader data binding point. It is used to get data to the
 * shader.
 *
 * A uniform is made up of a name, a data type and some data. The sizeof the
 * data type should match the size of the data stored in the uniform.
 */
class Uniform
{
public:
    /** Type of the uniform data */
    enum class UniformType
    {
        Float,
        Int,
        Vec4,
    };

    /**
     * Uniform default constructor
     */
    Uniform();

    /**
     * Uniform constructor.
     *
     * @param  name      const std::string &, name of the uniform.
     * @param  dataType  UniformType, data type of the uniform.
     * @param  dataSize  size_t, size of the uniform data.
     * @param  dataIn    const void *, pointer to uniform data.
     */
    Uniform(const std::string &name,
            UniformType dataType,
            size_t dataSize,
            const void *dataIn);

    /**
     * Get the uniform name.
     *
     * @return  const std::string &, uniform name.
     */
    const std::string &GetName() const;

    /**
     * Set the uniform name.
     *
     * @param  uniformName  const std::string &, uniform name to set.
     */
    void SetName(const std::string &uniformName);

    /**
     * Get the data type of the uniform.
     *
     * @return  UniformType, data type of the uniform.
     */
    UniformType GetDataType() const;

    /**
     * Set the data type of the uniform.
     *
     * @param  UniformType, data type of the uniform
     */
    void SetDataType(UniformType dataType);

    /**
     * Get the uniform's data.
     *
     * @return  const void *, uniform data.
     */
    const void *GetUniformData() const;

    /**
     * Get uniform data size.
     *
     * @return  size_t, size of the uniform data.
     */
    size_t GetUniformDataSize() const;

    /**
     * Set the uniform data.
     *
     * @param  dataSize  size_t, size of the passed in data (in bytes).
     * @param  dataIn    const void *, pointer to uniform data to set.
     */
    void SetUniformData(size_t dataSize, const void *dataIn);

private:
    /** Uniform name */
    std::string m_name;
    /** Uniform data type */
    UniformType m_dataType;
    /** Uniform data */
    ds_com::StreamBuffer m_dataBuffer;
};
}
