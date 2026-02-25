#pragma once

// internal
#include "vgl/defines.hpp"
#include "internal/i_renderer_context.hpp"

namespace vgl {

namespace noop {

class RendererContextNoop : public IRendererContext {
public:
	RendererContextNoop();
    ~RendererContextNoop() override;

    RendererType getRendererType() const override;
    const char* getRendererName() const override;

    bool init(const Config& _config) override;
    void shutdown() override;
	void window(uint16_t _width, uint16_t _height) override;
    
    void render(Frame* _frame) override;

    void createVertexLayout(VertexLayoutHandle _handle, const VertexLayout& _layout) override;
    void destroyVertexLayout(VertexLayoutHandle _handle) override;

    void createVertexBuffer(VertexBufferHandle _handle, uint32_t _size, VertexLayoutHandle _layoutHandle, void* _pData, BufferFlags _flags = VGL_BUFFER_NONE) override;
    void destroyVertexBuffer(VertexBufferHandle _handle) override;

    void createIndexBuffer(IndexBufferHandle _handle, uint32_t _size, void* _pData, BufferFlags _flags = VGL_BUFFER_NONE) override;
    void destroyIndexBuffer(IndexBufferHandle _handle) override;

    void createDynamicVertexBuffer(VertexBufferHandle _handle, uint32_t _size, VertexLayoutHandle _layoutHandle, BufferFlags _flags = VGL_BUFFER_NONE) override;
    void updateDynamicVertexBuffer(VertexBufferHandle _handle, uint32_t _dstOffset, uint32_t _size, void* _pData) override;
    void destroyDynamicVertexBuffer(VertexBufferHandle _handle) override;

    void createDynamicIndexBuffer(IndexBufferHandle _handle, uint32_t _size, BufferFlags _flags = VGL_BUFFER_NONE) override;
    void updateDynamicIndexBuffer(IndexBufferHandle _handle, uint32_t _dstOffset, uint32_t _size, void* _pData) override;
    void destroyDynamicIndexBuffer(IndexBufferHandle _handle) override;

    void createShader(ShaderHandle _handle, ShaderType _shaderType, const char* _source) override;
    void destroyShader(ShaderHandle _handle) override;

    void createProgram(ProgramHandle _handle, ShaderHandle _vert, ShaderHandle _frag) override;
    void destroyProgram(ProgramHandle _handle) override;

    void createTexture(
        TextureHandle _handle,
        TexTarget _target, TexFormat _internalFormat,
        uint16_t _width, uint16_t _height, uint16_t _depth,
        uint8_t _numLayers, bool _useMips,
        TexFormat _format, 
		void* _pData, TexFlags _flags
    ) override;
    void updateTexture(
        TextureHandle _handle,
        uint16_t _width, uint16_t _height, uint16_t _depth,
        uint8_t _layer, uint8_t _face,
		void* _pData
    ) override;
    void readTexture(
        TextureHandle _handle,
        uint8_t _layer, uint8_t _face,
		void* _data
    ) override;
    void destroyTexture(TextureHandle _handle) override;

    // void createFrameBuffer(FrameBufferHandle _handle, uint16_t _width, uint16_t _height, TexFormat _format, TexFormat _depthFormat) override;
    void createFrameBuffer(FrameBufferHandle _handle, uint8_t _num, const Attachment* _pAttachments) override;
    void destroyFrameBuffer(FrameBufferHandle _handle) override;
};

} // namespace noop

} // namespace vgl
