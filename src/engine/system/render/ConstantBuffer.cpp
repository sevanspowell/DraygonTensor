#include <cassert>

#include "engine/system/render/ConstantBuffer.h"

namespace ds_render
{
void ConstantBuffer::SetData(size_t dataSize, const void *const data)
{
    m_data.Clear();
    m_data.Insert(dataSize, data);
}

const void *ConstantBuffer::GetData() const
{
    return m_data.GetDataPtr();
}

void ConstantBuffer::DescribeBufferMember(std::string name,
                                          size_t offset,
                                          size_t size)
{
    m_memberName.push_back(name);
    m_memberOffset.push_back(offset);
    m_memberSize.push_back(size);
}

size_t ConstantBuffer::GetNumMembers() const
{
    // Parallel arrays, size of one is size of all
    return m_memberName.size();
}

const std::string &ConstantBuffer::GetMemberName(size_t index) const
{
    assert(index >= 0 && index < GetNumMembers() &&
           "Tried to access invalid index.");

    return m_memberName[index];
}

size_t ConstantBuffer::GetMemberOffset(size_t index) const
{
    assert(index >= 0 && index < GetNumMembers() &&
           "Tried to access invalid index.");

    return m_memberOffset[index];
}

size_t ConstantBuffer::GetMemberSize(size_t index) const
{
    assert(index >= 0 && index < GetNumMembers() &&
           "Tried to access invalid index.");

    return m_memberSize[index];
}
}
