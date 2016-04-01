#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

namespace ds_com
{
/**
 * The stream buffer is primarily used to store POD (Plain Old Data) structures.
 * When an object is inserted into the stream buffer the object is copied
 * directly, no constructors and destructors are called. For this reason, the
 * stream buffer should only be used to store POD types.
 */
class StreamBuffer
{
public:
    typedef uint8_t Byte_t;

    /**
     * Default constructor.
     */
    StreamBuffer();
    /**
     * Destructor.
     */
    ~StreamBuffer();

    /**
     * Insert data into the stream buffer.
     *
     * @param  size    size_t, size of the data to insert into the buffer.
     * @param  dataIn  const void *const, pointer to data to insert into the
     *                 buffer.
     */
    void Insert(size_t size, const void *const dataIn);
    /**
     * Extract data from the stream buffer.
     *
     * @param  size     size_t, size of the data to extract from the buffer.
     * @param  dataOut  void *const, pointer to where extracted data should be
     *                  stored. Nullptr if you don't want to store the data
     *                  anywhere.
     */
    bool Extract(size_t size, void *const dataOut = nullptr);

    /**
     * Return the number of bytes left in the buffer.
     *
     * @return size_t, number of bytes left in the buffer.
     */
    size_t AvailableBytes() const;

    /**
     * Clear the stream buffer (remove all data).
     */
    void Clear();

    /**
     * Get a pointer to the data contained in this buffer.
     *
     * @return const void *, const pointer to data in this buffer.
     */
    const void *GetDataPtr() const;

private:
    size_t m_readPos;
    std::vector<Byte_t> m_buffer;
};

/**
 * Insert data of the given type into the stream buffer. Convenience method.
 *
 * @param  dataIn const T &, data to insert into the buffer.
 * @return        StreamBuffer &, reference to this StreamBuffer object,
 *                allows you to chain operator calls.
 */
template <typename T>
StreamBuffer &operator<<(StreamBuffer &buffer, const T &dataIn);

/**
 * Extract data from the stream buffer. Convenience method.
 *
 * @param dataOut T &const, where extracted data should be stored.
 */
template <typename T>
StreamBuffer &operator>>(StreamBuffer &buffer, T &dataOut);

/**
 * Append all data in the 'from' buffer onto the end of the 'to' buffer.
 *
 * @param to   StreamBuffer &, StreamBuffer to append data to.
 * @param from const StreamBuffer &, StreamBuffer to get data from.
 */
void AppendStreamBuffer(StreamBuffer &to, const StreamBuffer &from);

#include "engine/common/StreamBuffer.hpp"
}
