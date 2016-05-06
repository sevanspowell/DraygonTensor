#include "engine/common/StreamBuffer.h"

namespace ds_com
{
StreamBuffer::StreamBuffer()
{
    m_readPos = 0;
}

StreamBuffer::~StreamBuffer()
{
}

void StreamBuffer::Insert(size_t size, const void *const dataIn)
{
    if (dataIn != nullptr)
    {
        // If the data is too big for the buffer
        if ((m_buffer.size() + size) > m_buffer.capacity())
        {
            // Reserve some more memory
            m_buffer.reserve((m_buffer.capacity() + size) * 2);
        }

        // Resize the array before copying data to it (this keeps the vector
        // size up
        // to date)
        m_buffer.resize(m_buffer.size() + size);

        int writePos = m_buffer.size() - size;
        memcpy(&m_buffer[writePos], dataIn, size);
    }
}

bool StreamBuffer::Extract(size_t size, void *const dataOut)
{
    bool result = false;

    // Only extract data if there is enough data in the buffer
    if (this->AvailableBytes() >= size)
    {
        if (dataOut != nullptr)
        {
            memcpy(dataOut, &m_buffer[m_readPos], size);
        }

        m_readPos += size;

        result = true;
    }

    return result;
}

size_t StreamBuffer::AvailableBytes() const
{
    return m_buffer.size() - m_readPos;
}

void StreamBuffer::Clear()
{
    m_buffer.clear();
    m_readPos = 0;
}

const void *StreamBuffer::GetDataPtr() const
{
    const void *ptr = nullptr;

    // Only get data ptr if buffer size is greater than 0 and less than size of buffer
    if (m_buffer.size() > 0 && m_readPos < m_buffer.size())
    {
        ptr = &m_buffer[m_readPos];
    }

    return ptr;
}

void AppendStreamBuffer(StreamBuffer *to, const StreamBuffer &from)
{
    to->Insert(from.AvailableBytes(), from.GetDataPtr());
}
}
