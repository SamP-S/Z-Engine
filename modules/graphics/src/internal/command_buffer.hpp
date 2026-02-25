#pragma once

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cassert>

namespace vgl {

/// Enumeration of all commands that can be encoded in the CommandBuffer.
enum Command : uint8_t
{
    RendererInit,
    RendererShutdownBegin,
    CreateVertexLayout,
	CreateVertexBuffer,
    CreateIndexBuffer,
	CreateDynamicVertexBuffer,
    UpdateDynamicVertexBuffer,
    CreateDynamicIndexBuffer,
    UpdateDynamicIndexBuffer,
    CreateShader,
    CreateProgram,
	CreateTexture,
	UpdateTexture,
	ResizeTexture,
    CreateFrameBuffer,
    							// CreateUniform,
    							// UpdateViewName,
    							// InvalidateOcclusionQuery,
    							// SetName,
    End,
    RendererShutdownEnd,
    DestroyVertexLayout,
    DestroyVertexBuffer,
	DestroyIndexBuffer,
	DestroyDynamicVertexBuffer,
    DestroyDynamicIndexBuffer,
    DestroyShader,
    DestroyProgram,
    DestroyTexture,
    DestroyFrameBuffer,
    							// DestroyUniform,
    							// ReadTexture,
    							// RequestScreenShot
};

/// CommandBuffer is a linear memory buffer for recording commands and their arguments
/// for a rendering backend.
class CommandBuffer {
private:
    uint8_t* m_pBuffer;    ///< Pointer to the linear memory buffer
    uint32_t m_pos;        ///< Current read/write position
    uint32_t m_size;       ///< Size of recorded commands
    uint32_t m_capacity;   ///< Current allocated capacity of buffer
    uint32_t m_minCapacity;///< Minimum capacity to maintain

    /// Align `_value` up to the next multiple of `_alignment`.
    static uint32_t alignUp(uint32_t value, uint32_t alignment);

public:
    // ------------------------------------------------------------------------
    // Construction / Destruction
    // ------------------------------------------------------------------------
    
    CommandBuffer();
    ~CommandBuffer();

    /// Initialize the buffer with a minimum capacity.
    void init(uint32_t minCapacity);

    /// Reset the buffer read/write position to the start.
    void reset();

    /// Begin recording commands. Clears previous frame's data.
    void start();

    /// Finish recording commands. Appends an End command and finalizes buffer size.
    void finish();

    // ------------------------------------------------------------------------
    // Memory management
    // ------------------------------------------------------------------------

    /// Resize the internal buffer to at least `_capacity` bytes.
    void resize(uint32_t _capacity = 0);

    /// Align the write/read position to `_alignment` bytes.
    void align(uint32_t _alignment);

    // ------------------------------------------------------------------------
    // Write / Read / Skip operations
    // ------------------------------------------------------------------------

    /// Write raw bytes into the buffer.
    void write(const void* data, uint32_t size);

    /// Read raw bytes from the buffer.
    void read(void* data, uint32_t size);

    /// Skip `_size` bytes in the buffer, returning a pointer to the skipped region.
    const uint8_t* skip(uint32_t size);

    // ------------------------------------------------------------------------
    // Template versions (must be in header)
    // ------------------------------------------------------------------------

    /// Write a typed value into the buffer, automatically aligning as needed.
    template<typename T>
    void write(const T& value) {
        align(alignof(T));
        write(reinterpret_cast<const uint8_t*>(&value), sizeof(T));
    }

    /// Read a typed value from the buffer, automatically aligning as needed.
    template<typename T>
    void read(T& value) {
        align(alignof(T));
        read(reinterpret_cast<uint8_t*>(&value), sizeof(T));
    }

    /// Skip a typed value in the buffer, automatically aligning as needed.
    template<typename T>
    void skip() {
        align(alignof(T));
        skip(sizeof(T));
    }

};

} // namespace vgl
