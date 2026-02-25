#pragma once

// internal
#include "vgl/defines.hpp"
#include "internal/pipeline_structs.hpp"

namespace vgl {

// immediate mode interface with renderer API
class IRendererContext {
public:
	IRendererContext() = default;
	virtual ~IRendererContext() = 0;
	virtual RendererType getRendererType() const = 0;
	virtual const char* getRendererName() const = 0;

	// constr/destr
	virtual bool init(const Config& _config) = 0;
	virtual void shutdown() = 0;
	virtual void window(uint16_t _width, uint16_t _height) = 0;

	// operations
	virtual void render(Frame* _render) = 0;

	// layouts
	virtual void createVertexLayout(VertexLayoutHandle _handle, const VertexLayout& _layout) = 0;
	virtual void destroyVertexLayout(VertexLayoutHandle _handle) = 0;
	
	// buffers
	virtual void createVertexBuffer(VertexBufferHandle _handle, uint32_t _size, VertexLayoutHandle _layoutHandle, void* _pData, BufferFlags _flags = VGL_BUFFER_NONE) = 0;
	virtual void destroyVertexBuffer(VertexBufferHandle _handle) = 0;
	virtual void createIndexBuffer(IndexBufferHandle _handle, uint32_t _size, void* _pData, BufferFlags _flags = VGL_BUFFER_NONE) = 0;
	virtual void destroyIndexBuffer(IndexBufferHandle _handle) = 0;

	// dynamic buffers
	virtual void createDynamicVertexBuffer(VertexBufferHandle _handle, uint32_t _size, VertexLayoutHandle _layoutHandle, BufferFlags _flags = VGL_BUFFER_NONE) = 0;
	virtual void updateDynamicVertexBuffer(VertexBufferHandle _handle, uint32_t _dstOffset, uint32_t _size, void* _pData) = 0;
	virtual void destroyDynamicVertexBuffer(VertexBufferHandle _handle) = 0;
	virtual void createDynamicIndexBuffer(IndexBufferHandle _handle, uint32_t _size, BufferFlags _flags = VGL_BUFFER_NONE) = 0;
	virtual void updateDynamicIndexBuffer(IndexBufferHandle _handle, uint32_t _dstOffset, uint32_t _size,  void* _pData) = 0;
	virtual void destroyDynamicIndexBuffer(IndexBufferHandle _handle) = 0;
	
	// shaders
	virtual void createShader(ShaderHandle _handle, ShaderType _shaderType, const char* _source) = 0;
	virtual void destroyShader(ShaderHandle _handle) = 0;
	virtual void createProgram(ProgramHandle _handle, ShaderHandle _vert, ShaderHandle _frag) = 0;
	virtual void destroyProgram(ProgramHandle _handle) = 0;

	// TODO: figure out better mips handling for generation & manual
	// create texture from type and params
	virtual void createTexture(
		TextureHandle _handle,
		TexTarget _target, TexFormat _internalFormat,
		uint16_t _width, uint16_t _height, uint16_t _depth, 
		uint8_t _numLayers, bool _useMips,
		TexFormat _format, 
		void* _pData, TexFlags _flags
	) = 0;
	// update whole single 2D/3D texture // TODO: allow sub texture updates
	virtual void updateTexture(
		TextureHandle _handle,									// source data
		uint16_t _width, uint16_t _height, uint16_t _depth,		// destination area
		uint8_t _layer, uint8_t _face,							// destination location
		void* _pData
	) = 0;
	// reads whole single 2D/3D texture // TODO: allow sub texture reads
	virtual void readTexture(
		TextureHandle _handle,					
		uint8_t _layer, uint8_t _face,			// target 2D/3D texture
		void* _data								// preallocated destination data ptr
	) = 0;
	virtual void destroyTexture(TextureHandle _handle) = 0;
	
	// virtual void createFrameBuffer(FrameBufferHandle _handle, uint16_t _width, uint16_t _height, TexFormat _format, TexFormat _depthFormat) = 0;
	virtual void createFrameBuffer(FrameBufferHandle _handle, uint8_t _num, const Attachment* _pAttachments) = 0;
	virtual void destroyFrameBuffer(FrameBufferHandle _handle) = 0;

	// // debug && renderdoc
	// virtual void setDebugMarker(const char* _name, uint16_t _len) = 0;
	// virtual void setDebugName(Handle _handle, const char* _name, uint16_t _len) = 0;
	// virtual void setViewName(ViewId _id, const char* _name, uint16_t _len) = 0;

};

// pure virtual destructor must have definition
inline IRendererContext::~IRendererContext() = default;

} // namespace vgl
