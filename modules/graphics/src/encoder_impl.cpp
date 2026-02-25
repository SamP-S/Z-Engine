#include "internal/encoder_impl.hpp"

// std lib
#include <iostream>
#include <cstring>
#include <algorithm>
// external
#include "spdlog/spdlog.h"

namespace vgl {

	const std::unordered_map<UniformType, size_t> EncoderImpl::s_kUniformTypeSize = {
		{VGL_UNIFORM_TYPE_INT32,                  		4},
		{VGL_UNIFORM_TYPE_UINT32,                 		4},
		{VGL_UNIFORM_TYPE_FLOAT,                  		4},
		{VGL_UNIFORM_TYPE_DOUBLE,                 		8},
		{VGL_UNIFORM_TYPE_IVEC2,                  		8},
		{VGL_UNIFORM_TYPE_IVEC3,                  		12},
		{VGL_UNIFORM_TYPE_IVEC4,                  		16},
		{VGL_UNIFORM_TYPE_UVEC2,                  		8},
		{VGL_UNIFORM_TYPE_UVEC3,                  		12},
		{VGL_UNIFORM_TYPE_UVEC4,                  		16},
		{VGL_UNIFORM_TYPE_VEC2,                   		8},
		{VGL_UNIFORM_TYPE_VEC3,                   		12},
		{VGL_UNIFORM_TYPE_VEC4,                   		16},
		{VGL_UNIFORM_TYPE_DVEC2,                  		16},
		{VGL_UNIFORM_TYPE_DVEC3,                  		24},
		{VGL_UNIFORM_TYPE_DVEC4,                  		32},
		{VGL_UNIFORM_TYPE_MAT2,                   		16},
		{VGL_UNIFORM_TYPE_MAT2X3,                 		24},
		{VGL_UNIFORM_TYPE_MAT2X4,                 		32},
		{VGL_UNIFORM_TYPE_MAT3X2,                 		24},
		{VGL_UNIFORM_TYPE_MAT3,                   		36},
		{VGL_UNIFORM_TYPE_MAT3X4,                 		48},
		{VGL_UNIFORM_TYPE_MAT4X2,                 		32},
		{VGL_UNIFORM_TYPE_MAT4X3,                 		48},
		{VGL_UNIFORM_TYPE_MAT4,                   		64},
		{VGL_UNIFORM_TYPE_SAMPLER_1D,             		4},
		{VGL_UNIFORM_TYPE_SAMPLER_2D,             		4},
		{VGL_UNIFORM_TYPE_SAMPLER_3D,             		4},
		{VGL_UNIFORM_TYPE_SAMPLER_CUBE,           		4},
		{VGL_UNIFORM_TYPE_SAMPLER_2D_RECT,        		4},
		{VGL_UNIFORM_TYPE_SAMPLER_1D_ARRAY,       		4},
		{VGL_UNIFORM_TYPE_SAMPLER_2D_ARRAY,       		4},
		{VGL_UNIFORM_TYPE_SAMPLER_CUBE_ARRAY,     		4},
		{VGL_UNIFORM_TYPE_SAMPLER_BUFFER,         		4},
		{VGL_UNIFORM_TYPE_SAMPLER_1D_SHADOW,      		4},
		{VGL_UNIFORM_TYPE_SAMPLER_2D_SHADOW,      		4},
		{VGL_UNIFORM_TYPE_SAMPLER_CUBE_SHADOW,    		4},
		{VGL_UNIFORM_TYPE_SAMPLER_2D_RECT_SHADOW, 		4},
		{VGL_UNIFORM_TYPE_SAMPLER_1D_ARRAY_SHADOW, 		4},
		{VGL_UNIFORM_TYPE_SAMPLER_2D_ARRAY_SHADOW, 		4},
		{VGL_UNIFORM_TYPE_SAMPLER_CUBE_ARRAY_SHADOW, 	4}
	};

	EncoderImpl::EncoderImpl() {
		this->discard(VGL_DISCARD_ALL);
	}
	EncoderImpl::~EncoderImpl() {

	}
	
	void EncoderImpl::_begin(Frame* _frame) {
		VGL_ASSERT(_frame != nullptr, "Can't encode to NULL frame.")
		m_pFrame = _frame;

		m_numDropped = 0;
		m_numSubmitted = 0;
	}

	void EncoderImpl::_end() {
		this->discard(VGL_DISCARD_ALL);
	}

	void EncoderImpl::setState(StateFlags _state) {
		m_draw.state = _state;
	}

	void EncoderImpl::setStencil(StencilFlags _fStencil, StencilFlags _bStencil) {
		m_draw.frontStencil = _fStencil;
		m_draw.backStencil = _bStencil;
	}
	void EncoderImpl::setScissor(uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height) {
		Rect& scissor = m_draw.scissor;
		scissor.x = _x;
		scissor.y = _y;
		scissor.width = _width;
		scissor.height = _height;
	}

	void EncoderImpl::setVertexBuffer(
		uint8_t _stream,
		VertexBufferHandle _handle,
		uint32_t _startVertex,
		uint32_t _numVertices
	) {
		VGL_ASSERT(_stream < VGL_MAX_VERTEX_STREAMS, "Invalid stream index.")
		VGL_ASSERT(isValid(_handle), "Invalid vertex buffer handle.")

		if (m_draw.vertexStreamsSet.test(_stream)) {
			std::cout << "WARNING: overwriting vertex stream " << _stream << std::endl;
		}

		m_draw.vertexStreamsSet.set(_stream, true);
		VertexStream& vStream = m_draw.vertexStreams[_stream];
		vStream.vertexBufferHandle = _handle;
		vStream.startVertex = _startVertex;
		vStream.numVertices = _numVertices;
		
		// update num vertices
		m_draw.numVertices = std::min(m_draw.numVertices, _numVertices);
	}

	void EncoderImpl::setVertexBuffer(
		uint8_t _stream,
		VertexBufferHandle _handle
	) {
		VGL_ASSERT(_stream < VGL_MAX_VERTEX_STREAMS, "Invalid stream index.")
		VGL_ASSERT(isValid(_handle), "Invalid vertex buffer handle.")

		if (m_draw.vertexStreamsSet.test(_stream)) {
			std::cout << "WARNING: overwriting vertex stream " << _stream << std::endl;
		}

		m_draw.vertexStreamsSet.set(_stream, true);
		VertexStream& vStream = m_draw.vertexStreams[_stream];
		vStream.vertexBufferHandle = _handle;
		vStream.startVertex = 0;
		vStream.numVertices = 0;
	}

	void EncoderImpl::setIndexBuffer(
		IndexBufferHandle _handle,
		uint32_t _startIndex,
		uint32_t _numIndices
	) {
		VGL_ASSERT(isValid(_handle), "Invalid index buffer handle.")
		IndexStream iStream;
		iStream.indexBufferHandle = _handle;
		iStream.startIndex = _startIndex;
		iStream.numIndices = _numIndices;
		m_draw.indexStream = iStream;
	}

	void EncoderImpl::setIndexBuffer(
		IndexBufferHandle _handle
	) {
		VGL_ASSERT(isValid(_handle), "Invalid index buffer handle.")
		IndexStream iStream;
		iStream.indexBufferHandle = _handle;
		iStream.startIndex = 0;
		iStream.numIndices = 0;
		m_draw.indexStream = iStream;
	}

	void EncoderImpl::setTextureUnit(
		uint8_t _unit,
		const char* _name,
		TextureHandle _handle
	) {
		VGL_ASSERT(_unit < VGL_MAX_TEXTURE_UNITS, "Invalid texture unit.")
		VGL_ASSERT(isValid(_handle), "Invalid texture handle.")
		size_t len = strlen(_name);
		VGL_ASSERT(len < VGL_MAX_UNIFORM_NAME_LENGTH, "Invalid name too long.")
		TextureUnit& unit = m_draw.textureUnits[_unit];
		strcpy(unit.name, _name);
		unit.handle = _handle;
		m_draw.textureUnitsSet.set(_unit);
		spdlog::trace("[EncoderImpl] Set texture unit {} to sampler '{}' with handle {}", _unit, _name, _handle.idx);
	}

	void EncoderImpl::setUniform(
		const char* _name,
		UniformType _type,
		const void* _pData,
		uint16_t _numElements,
		bool _transpose
	) {
		VGL_ASSERT(_pData != nullptr, "Invalid data is nullptr.")
		VGL_ASSERT(_type < VGL_UNIFORM_TYPE_COUNT, "Invalid uniform type.")
		size_t len = strlen(_name);
		_numElements = std::max(uint16_t(1), _numElements);
		size_t size = s_kUniformTypeSize.at(_type) * _numElements;
		VGL_ASSERT(len < VGL_MAX_UNIFORM_NAME_LENGTH, "Invalid name too long.")
		
		// copy user data
		void* pCopy = malloc(size);
		memcpy(pCopy, _pData, size);

		// log
		spdlog::trace("[EncoderImpl] Set uniform '{}' of type {} with {} elements (size: {})",
			_name, static_cast<uint32_t>(_type), _numElements, size);
		spdlog::trace("[EncoderImpl] data = {}", reinterpret_cast<const uint8_t*>(_pData)[0]);

		// write to draw uniform
		Uniform& uniform = m_draw.uniforms[m_draw.numUniforms];
		strcpy(uniform.name, _name);
		uniform.type = _type;
		uniform.pData = pCopy;
		uniform.numElements = _numElements;
		uniform.transpose = _transpose;
		m_draw.numUniforms++;
	}

	void EncoderImpl::touch(ViewID _id) {
		submit(_id, VGL_INVALID_HANDLE, VGL_DISCARD_ALL);
	}
	
	void EncoderImpl::submit(ViewID _id, ProgramHandle _handle, DiscardFlags _flags) {
		// set draw view pipeline
		m_draw.viewID = _id;

		// discard if no v/i streams or v/i counts
		if ((UINT32_MAX == m_draw.numVertices) && (0 == m_draw.indexStream.numIndices)) {
			discard(_flags);
			++m_numDropped;
			return;
		}

		// discard if reached max draw calls
		if (m_pFrame->numDrawCalls.load() >= VGL_MAX_DRAW_CALLS) {
			discard(_flags);
			++m_numDropped;
			return;
		}

		// write draw call to frame at next available index
		m_draw.programHandle = _handle;
		const uint32_t idx = m_pFrame->numDrawCalls.fetch_add(1);
		m_pFrame->drawCalls[idx] = m_draw;
		spdlog::trace("[EncoderImpl] Submit set draw {}", idx);
		m_numSubmitted++;	

		// discard requested for next
		discard(_flags);
	}

	void EncoderImpl::discard(DiscardFlags _flags) {
		m_draw.discard(_flags);
	}

}