#include "vgl/vgl.hpp"

// std lib
#include <iostream>
// internal
#include "vgl/defines.hpp"
#include "internal/context.hpp"

#define VGL_ASSERT_CTX()		VGL_ASSERT(s_ctx != nullptr, "VGL was not init successfully, context does not exist!")
#define VGL_ASSERT_NO_CTX()		VGL_ASSERT(s_ctx == nullptr, "VGL already init, context shouldn't exist!")

namespace vgl {

	void hello() {
		std::cout << "Hello world." << std::endl;
	}
	
	namespace {
		// anonymous namespace for context
		static Context* s_ctx = nullptr;
	}

	/// --- LIBRARY OPERATIONS --- ///
	bool init(const Config& _config) {
		VGL_ASSERT_NO_CTX()
		Context* ctx = new Context();
		// validate init
		if (!ctx->init(_config)) {
			ctx->shutdown();
			return false;
		}
		// save created context
		s_ctx = ctx;
		return true;
	}
	
	void shutdown() {
		VGL_ASSERT_CTX()
		s_ctx->shutdown();
		s_ctx = nullptr;
	}

	void window(uint16_t _width, uint16_t _height) {
		VGL_ASSERT_CTX()
		s_ctx->window(_width, _height);
	}

	Encoder* begin() {
		VGL_ASSERT_CTX()
		return s_ctx->begin();
	}

	void end(Encoder* _pEncoder) {
		VGL_ASSERT_CTX()
		s_ctx->end(_pEncoder);
	}
	
	void frame() {
		VGL_ASSERT_CTX()
		s_ctx->frame();
	}

	uint32_t render() {
		VGL_ASSERT_CTX()
		return s_ctx->render();
	}

	const Stats& getPerformanceStats() {
		VGL_ASSERT_CTX()
		return s_ctx->getPerformanceStats();
	}

	const Resolution& getResolution() {
		VGL_ASSERT_CTX()
		return s_ctx->getResolution();
	}

	const RendererType getRendererType() {
		VGL_ASSERT_CTX()
		return s_ctx->getRendererType();
	}

	const char* getRendererName() {
		VGL_ASSERT_CTX()
		return s_ctx->getRendererName();
	}


	/// --- RESOURCE CREATE/DESTROY --- ///
	VertexLayoutHandle createVertexLayout(const VertexLayout& _layout) {
		VGL_ASSERT_CTX()
		return s_ctx->createVertexLayout(_layout);
	}
	void destroy(VertexLayoutHandle _handle) {
		VGL_ASSERT_CTX()
		s_ctx->destroy(_handle);
	}

	VertexBufferHandle createVertexBuffer(uint32_t _size, VertexLayoutHandle _layoutHandle, const void* _pData, BufferFlags _flags) {
		VGL_ASSERT_CTX()
		return s_ctx->createVertexBuffer(_size, _layoutHandle, _pData, _flags);
	}
	void destroy(VertexBufferHandle _handle) {
		VGL_ASSERT_CTX()
		s_ctx->destroy(_handle);
	}

	IndexBufferHandle createIndexBuffer(uint32_t _size, const void* _pData, BufferFlags _flags) {
		VGL_ASSERT_CTX()
		return s_ctx->createIndexBuffer(_size, _pData, _flags);
	}
	void destroy(IndexBufferHandle _handle) {
		VGL_ASSERT_CTX()
		s_ctx->destroy(_handle);
	}

	ShaderHandle createShader(ShaderType _shaderType, const char* _source) {
		VGL_ASSERT_CTX()
		return s_ctx->createShader(_shaderType, _source);
	}
	void destroy(ShaderHandle _handle) {
		VGL_ASSERT_CTX()
		s_ctx->destroy(_handle);
	}

	ProgramHandle createProgram(ShaderHandle _vert, ShaderHandle _frag) {
		VGL_ASSERT_CTX()
		return s_ctx->createProgram(_vert, _frag);
	}
	void destroy(ProgramHandle _handle) {
		VGL_ASSERT_CTX()
		s_ctx->destroy(_handle);
	}

	TextureHandle createTexture2D(
		TexFormat _internalFormat,
		uint16_t _width, uint16_t _height,
		uint8_t _numLayers, bool _useMips,
		TexFormat _format,
		const void* _pData, TexFlags _flags
	) {
		VGL_ASSERT_CTX()
		return s_ctx->createTexture2D(_internalFormat, _width, _height, _numLayers, _useMips, _format, _pData, _flags);
	}
	TextureHandle createTexture3D(
		TexFormat _internalFormat,
		uint16_t _width, uint16_t _height, uint16_t _depth,
		bool _useMipMaps,
		TexFormat _format,
		const void* _pData, TexFlags _flags
	) {
		VGL_ASSERT_CTX()
		return s_ctx->createTexture3D(_internalFormat, _width, _height, _depth, _useMipMaps, _format, _pData, _flags);
	}
	TextureHandle createTextureCube(
		TexFormat _internalFormat,
		uint16_t _width,
		uint8_t _numLayers, bool _useMipMaps,
		TexFormat _format,
		const void* _pData, TexFlags _flags
	) {
		VGL_ASSERT_CTX()
		return s_ctx->createTextureCube(_internalFormat, _width, _numLayers, _useMipMaps, _format, _pData, _flags);
	}
	void destroy(TextureHandle _handle) {
		VGL_ASSERT_CTX()
		s_ctx->destroy(_handle);
	}

	FrameBufferHandle createFrameBuffer(TextureHandle _colourTex, TextureHandle _depthStencilTex) {
		VGL_ASSERT_CTX()
		return s_ctx->createFrameBuffer(_colourTex, _depthStencilTex);
	}
	FrameBufferHandle createFrameBuffer(uint8_t _num, const Attachment* _pAttachments) {
		VGL_ASSERT_CTX()
		return s_ctx->createFrameBuffer(_num, _pAttachments);
	}
	void destroy(FrameBufferHandle _handle) {
		VGL_ASSERT_CTX()
		s_ctx->destroy(_handle);
	}

	/// --- VIEW PIPELINE --- ///
	void setViewRect(ViewID _id, int16_t _x, int16_t _y, uint16_t _width, uint16_t _height) {
		VGL_ASSERT_CTX()
		s_ctx->setViewRect(_id, _x, _y, _width, _height);
	}
	void setViewScissor(ViewID _id, uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height) {
		VGL_ASSERT_CTX()
		s_ctx->setViewScissor(_id, _x, _y, _width, _height);
	}
	void setViewClear(ViewID _id, ClearFlags _clearFlags, uint32_t _rgba, float _depth, uint8_t _stencil) {
		VGL_ASSERT_CTX()
		s_ctx->setViewClear(_id, _clearFlags, _rgba, _depth, _stencil);
	}
	void setViewFrameBuffer(ViewID _id, FrameBufferHandle _handle) {
		VGL_ASSERT_CTX()
		s_ctx->setViewFrameBuffer(_id, _handle);
	}
	void resetView(ViewID _id) {
		VGL_ASSERT_CTX()
		s_ctx->resetView(_id);
	}
	
} // namespace vgl
