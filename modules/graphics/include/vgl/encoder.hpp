#pragma once

// internal
#include "vgl/defines.hpp"
#include "vgl/g_structs.hpp"

namespace vgl {

// encoder encodes all the information for a single draw call
class Encoder {
public:
	Encoder() = default;
	virtual ~Encoder() = default;

	// pipeline
	virtual void setState(StateFlags _state) = 0;
	virtual void setStencil(StencilFlags _fStencil, StencilFlags _bStencil) = 0;
	virtual void setScissor(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height) = 0;

	// resources
	virtual void setVertexBuffer(
		uint8_t _stream,
		VertexBufferHandle _handle,
		uint32_t _startVertex,
		uint32_t _numVertices
	) = 0;
	virtual void setVertexBuffer(
		uint8_t _stream,
		VertexBufferHandle _handle
	) = 0;
	virtual void setIndexBuffer(
		IndexBufferHandle _handle,
		uint32_t _firstIndex,
		uint32_t _numIndices
	) = 0;
	virtual void setIndexBuffer(
		IndexBufferHandle _handle
	) = 0;
	virtual void setTextureUnit(
		uint8_t _unit,
		const char* _name,
		TextureHandle _handle
	) = 0;
	virtual void setUniform(
		const char* _name,
		UniformType _type,
		const void* _pData,
		uint16_t _numElements = 1,
		bool _transpose = false
	) = 0;

	// operations
	virtual void touch(ViewID _id) = 0;
	virtual void submit(ViewID _id, ProgramHandle _handle, DiscardFlags _flags) = 0;
	virtual void discard(DiscardFlags _flags) = 0;
};

} // namespace vgl
