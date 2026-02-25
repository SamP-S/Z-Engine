#pragma once

// external
#include "vgl/encoder.hpp"
#include "internal/pipeline_structs.hpp"

namespace vgl {

class EncoderImpl : public Encoder {
private:
	// encoding state
	Frame* m_pFrame;
	DrawCall m_draw;
	bool m_discard;

	// stats
	uint32_t m_numSubmitted;
	uint32_t m_numDropped;

	static const std::unordered_map<UniformType, size_t> s_kUniformTypeSize;

public:
	EncoderImpl();
	~EncoderImpl();

	// **INTERNAL USAGE ONLY**
	void _begin(Frame* _frame);
	void _end();

	// pipeline
	void setState(StateFlags _state) override;
	void setStencil(StencilFlags _fStencil, StencilFlags _bStencil) override;
	void setScissor(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height) override;

	/// TODO:
	// - consolidate vertex count & indice count using resource properties rather than setting on draw call
	// - allow for vertex count instead of vertex buffer

	// resources
	void setVertexBuffer(
		uint8_t _stream,
		VertexBufferHandle _handle,
		uint32_t _startVertex,
		uint32_t _numVertices
	) override;
	void setVertexBuffer(
		uint8_t _stream,
		VertexBufferHandle _handle
	) override;
	void setIndexBuffer(
		IndexBufferHandle _handle,
		uint32_t _firstIndex,
		uint32_t _numIndices
	) override;
	void setIndexBuffer(
		IndexBufferHandle _handle
	) override;
	void setTextureUnit(
		uint8_t _unit,
		const char* _name,
		TextureHandle _handle
	) override;
	void setUniform(
		const char* _name,
		UniformType _type,
		const void* _pData,
		uint16_t _numElements,
		bool _transpose
	) override;

	// operations
	void touch(ViewID _id) override;
	void submit(ViewID _id, ProgramHandle _handle, DiscardFlags _flags) override;
	void discard(DiscardFlags _flags) override;
};

} // namespace vgl
