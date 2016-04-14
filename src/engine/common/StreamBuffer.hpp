template <typename T>
StreamBuffer &operator<<(StreamBuffer &buffer, const T &dataIn)
{
    buffer.Insert(sizeof(dataIn),
                  reinterpret_cast<const StreamBuffer::Byte_t *>(&dataIn));
    return buffer;
}

template <typename T>
StreamBuffer &operator>>(StreamBuffer &buffer, T &dataOut)
{
    buffer.Extract(sizeof(dataOut),
                   reinterpret_cast<StreamBuffer::Byte_t *const>(&dataOut));
    return buffer;
}

template <typename T>
bool StreamBuffer::Peek(T *const dataOut) const
{
    bool result = false;

    size_t dataSize = sizeof(*dataOut);

    // Only extract data if there is enough data in the buffer
    if (this->AvailableBytes() >= dataSize)
    {
        if (dataOut != nullptr)
        {
            memcpy(dataOut, &m_buffer[m_readPos], dataSize);
        }

        result = true;
    }

    return result;
}

template <typename T>
void StreamBuffer::Set(const T *const dataIn)
{
    if (dataIn != nullptr)
    {
        size_t dataSize = sizeof(*dataIn); 

        // Clear stream buffer
        Clear();

        // If the data is too big for the buffer
        if ((m_buffer.size() + dataSize) > m_buffer.capacity())
        {
            // Reserve some more memory
            m_buffer.reserve((m_buffer.capacity() + dataSize) * 2);
        }

        // Resize the array before copying data to it (this keeps the vector
        // size up to date)
        m_buffer.resize(m_buffer.size() + dataSize);

        int writePos = m_buffer.size() - dataSize;
        memcpy(&m_buffer[writePos], dataIn, dataSize);
    }
}
