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
