#include <iostream>

#include "engine/system/render/ConstantBufferDescription.h"

namespace ds_render
{
ConstantBufferDescription::ConstantBufferDescription(size_t dataSize)
{
    m_data.resize(dataSize);
}

void ConstantBufferDescription::AddMember(const std::string &memberName)
{
    m_offsetMap[memberName] = -1;
}

void ConstantBufferDescription::SetMemberOffset(const std::string &memberName,
                                                size_t offset)
{
    // Try to entry for member
    std::map<std::string, size_t>::iterator it =
        m_offsetMap.find(memberName);

    // If found
    if (it != m_offsetMap.end())
    {
        it->second = offset;
    }
    else
    {
        std::cerr << "ConstantBufferDescription::SetMemberOffset: No member "
                     "with name '"
                  << memberName << "'." << std::endl;
    }
}

void ConstantBufferDescription::InsertMemberData(const std::string &memberName,
                                                 size_t dataSize,
                                                 const void *data)
{
    // Try to find offset for member
    std::map<std::string, size_t>::const_iterator it =
        m_offsetMap.find(memberName);

    // If found
    if (it != m_offsetMap.end())
    {
        // Insert data at offset
        memcpy((&m_data[it->second]), data, dataSize);
    }
    else
    {
        std::cerr << "ConstantBufferDescription::InsertMemberData: No member "
                     "with name '"
                  << memberName << "'." << std::endl;
    }
}

std::vector<std::string> ConstantBufferDescription::GetMemberNames() const
{
    std::vector<std::string> memberNames;

    for (auto &nameOffset : m_offsetMap)
    {
        memberNames.push_back(nameOffset.first);
    }

    return memberNames;
}

size_t ConstantBufferDescription::GetNumberOfMembers() const
{
    return m_offsetMap.size();
}

size_t ConstantBufferDescription::GetBufferSize() const
{
    return m_data.size();
}

const void *ConstantBufferDescription::GetDataPtr() const
{
    return &m_data[0];
}
}
