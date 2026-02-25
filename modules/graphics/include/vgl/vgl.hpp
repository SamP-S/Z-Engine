#pragma once

// std libs
#include <cstdint>
// internal
#include "vgl/defines.hpp"
#include "vgl/g_structs.hpp"
#include "vgl/encoder.hpp"

/// --- DOCUMENTATION --- ///
// See docs.gl/gl3

namespace vgl {

// debug
void hello();

/// --- LIBRARY OPERATIONS --- ///
// start library
bool init(const Config& _config);
void shutdown();
void window(uint16_t _width, uint16_t _height);
Encoder* begin();
void end(Encoder* _pEncoder);
void frame();
uint32_t render();
const Stats& getPerformanceStats();
const Resolution& getResolution();
const RendererType getRendererType();
const char* getRendererName();

/// --- RESOURCE CREATE/DESTROY --- ///
VertexLayoutHandle createVertexLayout(
	const VertexLayout& _layout
);
void destroy(VertexLayoutHandle _handle);

VertexBufferHandle createVertexBuffer(	
	uint32_t _size,
	VertexLayoutHandle _layoutHandle,
	const void* _pData,
	BufferFlags _flags = VGL_BUFFER_NONE
);
void destroy(VertexBufferHandle _handle);

IndexBufferHandle createIndexBuffer(
	uint32_t _size,
	const void* _pData,
	BufferFlags _flags = VGL_BUFFER_NONE
);
void destroy(IndexBufferHandle _handle);

ShaderHandle createShader(
	ShaderType _shaderType,
	const char* _source
);
void destroy(ShaderHandle _handle);

ProgramHandle createProgram(
	ShaderHandle _vert,
	ShaderHandle _frag
);
void destroy(ProgramHandle _handle);

TextureHandle createTexture2D(
	TexFormat _interalFormat,
	uint16_t _width, uint16_t _height,
	uint8_t _numLayers, bool _useMips,
	TexFormat _format,
	const void* _pData, TexFlags _flags
);
TextureHandle createTexture3D(
	TexFormat _internalFormat,
	uint16_t _width, uint16_t _height, uint16_t _depth,
	bool _useMipMaps,
	TexFormat _format,
	const void* _pData, TexFlags _flags
);
TextureHandle createTextureCube(
	TexFormat _internalFormat,
	uint16_t _width,
	uint8_t _numLayers, bool _useMipMaps,
	TexFormat _format,
	const void* _pData, TexFlags _flags
);
void destroy(TextureHandle _handle);

FrameBufferHandle createFrameBuffer(
	TextureHandle _colourTex,
	TextureHandle _depthStencilTex
);

FrameBufferHandle createFrameBuffer(
	uint8_t _num, const Attachment* _pAttachments
);
void destroy(FrameBufferHandle _handle);

// set pipeline viewport
void setViewRect(
	ViewID _id,
	int16_t _x, 
	int16_t _y, 
	uint16_t _width, 
	uint16_t _height
);

// set pipeline scissor
// disabled: width, height == 0, 0
void setViewScissor(
	ViewID _id,
	uint16_t _x = 0,
	uint16_t _y = 0,
	uint16_t _width = 0,
	uint16_t _height = 0
);

void setViewClear(
	ViewID _id,
	ClearFlags _clearFlags,
	uint32_t _rgba = 0x333333ff,
	float _depth = 1.0f,
	uint8_t _stencil = 0
);

void setViewFrameBuffer(
	ViewID _id,
	FrameBufferHandle _handle
);

void resetView(ViewID _id);

/// TODO:
// - blit
// - Implemente API of <resource>Info structs as debugging layer to cache data
// - Implement MSAA support as capability
// - uniforms as a resource
// - MSAA textures
// - 1D textures
// - update sub region of resources
// - dynamic buffers
// - free handles after destroy
 
} // vgl
