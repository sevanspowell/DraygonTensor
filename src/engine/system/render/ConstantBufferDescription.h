#pragma once

#include <map>
#include <string>
#include <vector>

namespace ds_render
{
/**
 * The way the shader stores constant buffer data may not match C/C++. Use this
 * class as a bridge.
 *
 * A data store is created by the renderer and you fill it's data up.
 */
class ConstantBufferDescription
{
public:
    /**
     * Create a constant buffer data store of the given size;
     *
     * @param  dataSize  size_t, create a data store of this size.
     */
    ConstantBufferDescription(size_t dataSize = 0);

    /**
     * Add a member to the data store.
     *
     * @param  memberName  const std::string &, name of the data store member.
     */
    void AddMember(const std::string &memberName);

    /**
     * Set the offset of the member in the data store.
     *
     * @param  memberName  const std::string &, name of the data store member.
     * @param  offset      size_t, offset into data store of member.
     */
    void SetMemberOffset(const std::string &memberName, size_t offset);

    /**
     * Insert data into a member of the ConstantBufferDescription.
     *
     * @param  memberName  const std::string &, name of member to insert data
     * into.
     * @param  dataSize    size_t, size of the data to insert.
     * @param  data        const void *, data to insert.
     */
    void InsertMemberData(const std::string &memberName,
                          size_t dataSize,
                          const void *data);

    /**
     * Get a list of the names of all members in the constant buffer.
     *
     * @return  std::vector<std::string>, list of names of all members in the
     * constant buffer.
     */
    std::vector<std::string> GetMemberNames() const;

    /**
     * Get number of members in the constant buffer.
     *
     * @return  size_t, number of members in the constant buffer.
     */
    size_t GetNumberOfMembers() const;

    /**
     * Get the size (in bytes) of the constant buffer.
     *
     * @return  size_t, size (in bytes) of the constant buffer.
     */
    size_t GetBufferSize() const;

    /**
     * Get a pointer to the constant buffer data.
     *
     * @return  const void *, pointer to constant buffer data.
     */
    const void *GetDataPtr() const;

private:
    /** Constant buffer data */
    std::vector<char> m_data;

    /** Map member name to offset */
    std::map<std::string, size_t> m_offsetMap;
};
}
