#include "internal/command_buffer.hpp"

namespace vgl {

// ------------------------------------------------------------------------
// Static utility
// ------------------------------------------------------------------------

uint32_t CommandBuffer::alignUp(uint32_t value, uint32_t alignment) {
    return (value + alignment - 1) & ~(alignment - 1);
}

// ------------------------------------------------------------------------
// Construction / Destruction
// ------------------------------------------------------------------------

CommandBuffer::CommandBuffer()
    : m_pBuffer(nullptr)
    , m_pos(0)
    , m_size(0)
    , m_capacity(0)
    , m_minCapacity(0)
{
    resize();
    finish();
}

CommandBuffer::~CommandBuffer() {
    std::free(m_pBuffer);
}

// ------------------------------------------------------------------------
// Initialization
// ------------------------------------------------------------------------

void CommandBuffer::init(uint32_t minCapacity) {
    m_minCapacity = alignUp(minCapacity, 1024);
    resize();
}

void CommandBuffer::reset() {
    m_pos = 0;
}

void CommandBuffer::start() {
    m_pos = 0;
    m_size = 0;
}

void CommandBuffer::finish() {
    uint8_t cmd = static_cast<uint8_t>(Command::End);
    write(cmd);
    m_size = m_pos;
    m_pos = 0;

    if (m_size < m_minCapacity && m_capacity != m_minCapacity) {
        resize();
    }
}

// ------------------------------------------------------------------------
// Memory management
// ------------------------------------------------------------------------

void CommandBuffer::resize(uint32_t _capacity) {
    uint32_t newCapacity = alignUp(std::max(_capacity, m_minCapacity), 1024);
    m_pBuffer = static_cast<uint8_t*>(std::realloc(m_pBuffer, newCapacity));
    assert(m_pBuffer && "CommandBuffer allocation failed!");
    m_capacity = newCapacity;
}

void CommandBuffer::align(uint32_t alignment) {
    const uint32_t mask = alignment - 1;
    m_pos = (m_pos + mask) & (~mask);
}

// ------------------------------------------------------------------------
// Write / Read / Skip
// ------------------------------------------------------------------------

void CommandBuffer::write(const void* data, uint32_t size) {
    assert(m_size == 0 && "Called write outside start/finish");
    if (m_pos + size > m_capacity) {
        resize(m_capacity + (16 << 10));
    }
    std::memcpy(&m_pBuffer[m_pos], data, size);
    m_pos += size;
}

void CommandBuffer::read(void* data, uint32_t size) {
    assert(m_pos + size <= m_size && "CommandBuffer::read out of bounds");
    std::memcpy(data, &m_pBuffer[m_pos], size);
    m_pos += size;
}

const uint8_t* CommandBuffer::skip(uint32_t size) {
    assert(m_pos + size <= m_size && "CommandBuffer::skip out of bounds");
    const uint8_t* result = &m_pBuffer[m_pos];
    m_pos += size;
    return result;
}

} // namespace vgl
