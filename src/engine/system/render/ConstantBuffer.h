#pragma once

#include "engine/common/StreamBuffer.h"

namespace ds_render
{
/**
 * A constant buffer essentially corresponds to a struct in a shader. It is an
 * aggregate of uniform data that is used by the shader.
 *
 * A constant buffer might include the view and projection matrices for a camera
 * for example.
 */
class ConstantBuffer
{
public:
    /**
     * Set the data of the constant buffer. This is the collection of all
     * uniform data that makes up this constant buffer.
     *
     * @param  dataSize  size_t, size of the data to set.
     * @param  data      const void *const, pointer to data to copy to constant
     * buffer.
     */
    void SetData(size_t dataSize, const void *const data);

    /**
     * Get constant buffer data (the combined data of all uniforms).
     */
    const void *GetData() const;

    /**
     * Describe a uniform in the constant buffer: It's name (which will be used
     * to bind to the shader), it's offset into the constant buffer and the size
     * of it's data.
     *
     * @param  name    std::string, name of the buffer member.
     * @param  offset  size_t, buffer member's offset into the constant buffer.
     * @param  size    size_t, size of the buffer member's data.
     */
    void DescribeBufferMember(std::string name, size_t offset, size_t size);

    /**
     * Get the number of members in the constant buffer (number of uniforms).
     *
     * @return  size_t, number of members in the constant buffer.
     */
    size_t GetNumMembers() const;

    /**
     * Get the name of the member at the given index.
     *
     * @pre  Given index is greater than or equal to 0 and less than the number
     * of members in the constant buffer (use GetNumMembers to check).
     *
     * @param   index  size_t, index of member to get name of.
     * @return         const std::string &, name of member.
     */
    const std::string &GetMemberName(size_t index) const;

    /**
     * Get the offset of the member at the given index.
     *
     * @pre  Given index is greater than or equal to 0 and less than the number
     * of members in the constant buffer (use GetNumMembers to check).
     *
     * @param   index  size_t, index of member to get offset of.
     * @return         size_t, offset of member in constant buffer.
     */
    size_t GetMemberOffset(size_t index) const;

    /**
     * Get the size of the member at the given index.
     *
     * @pre  Given index is greater than or equal to 0 and less than the number
     * of members in the constant buffer (use GetNumMembers to check).
     *
     * @param   index  size_t, index of member to get size of.
     * @return         size_t, size of member.
     */
    size_t GetMemberSize(size_t index) const;

private:
    // Parallel arrays
    /** Name of each uniform in shader */
    std::vector<std::string> m_memberName;
    /** Uniform data's offset into data store */
    std::vector<size_t> m_memberOffset;
    /** Uniform data's size */
    std::vector<size_t> m_memberSize;

    /** Constant buffer data */
    ds_com::StreamBuffer m_data;
};
}
