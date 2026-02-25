#include "internal/noop/noop_renderer_context.hpp"
#include <iostream>

namespace vgl {

namespace noop {

RendererContextNoop::RendererContextNoop() {
	std::cout << "RendererContextNoop::RendererContextNoop\n\n";
}

RendererContextNoop::~RendererContextNoop() {
	std::cout << "RendererContextNoop::~RendererContextNoop\n\n";
}

RendererType RendererContextNoop::getRendererType() const {
	std::cout << "RendererContextNoop::getRendererType\n\n";
	return VGL_RENDERER_NOOP;
}

const char* RendererContextNoop::getRendererName() const {
	std::cout << "RendererContextNoop::getRendererName\n\n";
	return "No-Op";
}

bool RendererContextNoop::init(const Config& _config) {
	(void)_config;
	std::cout << "RendererContextNoop::init\n";
	std::cout << "_config.resolution.width=" << _config.resolution.width << "\n";
	std::cout << "_config.resolution.height=" << _config.resolution.height << "\n\n";
	return true;
}

void RendererContextNoop::shutdown() {
	std::cout << "RendererContextNoop::shutdown\n\n";
}

void RendererContextNoop::window(uint16_t _width, uint16_t _height) {
	(void)_width; (void)_height;
	std::cout << "RendererContextNoop::window\n";
	std::cout << "_width=" << _width << "\n";
	std::cout << "_height=" << _height << "\n\n";
}

void RendererContextNoop::render(Frame* _frame) {
	(void)_frame;
	std::cout << "RendererContextNoop::render\n";
	std::cout << "_frame=" << _frame << "\n";
	std::cout << "_frame.numDrawCalls=" << _frame->numDrawCalls << "\n";
	std::cout << "_frame.views=" << _frame->views << "\n\n";
}

void RendererContextNoop::createVertexLayout(VertexLayoutHandle _handle, const VertexLayout& _layout) {
	(void)_handle; (void)_layout;
	std::cout << "RendererContextNoop::createVertexLayout\n";
	std::cout << "_handle=" << _handle.idx << "\n";
	std::cout << "_layout=...\n\n";
}

void RendererContextNoop::destroyVertexLayout(VertexLayoutHandle _handle) {
	(void)_handle;
	std::cout << "RendererContextNoop::destroyVertexLayout\n";
	std::cout << "_handle=" << _handle.idx << "\n\n";
}

void RendererContextNoop::createVertexBuffer(VertexBufferHandle _handle, uint32_t _size, VertexLayoutHandle _layoutHandle, void* _pData, BufferFlags _flags) {
	(void)_handle; (void)_size; (void)_layoutHandle; (void)_pData; (void)_flags;
	std::cout << "RendererContextNoop::createVertexBuffer\n";
	std::cout << "_handle=" << _handle.idx << "\n";
	std::cout << "_size=" << _size << "\n";
	std::cout << "_layoutHandle=" << _layoutHandle.idx << "\n";
	std::cout << "_pData=" << _pData << "\n";
	std::cout << "_flags=" << static_cast<uint32_t>(_flags) << "\n\n";
}

void RendererContextNoop::destroyVertexBuffer(VertexBufferHandle _handle) {
	(void)_handle;
	std::cout << "RendererContextNoop::destroyVertexBuffer\n";
	std::cout << "_handle=" << _handle.idx << "\n\n";
}

void RendererContextNoop::createIndexBuffer(IndexBufferHandle _handle, uint32_t _size, void* _pData, BufferFlags _flags) {
	(void)_handle; (void)_size; (void)_pData; (void)_flags;
	std::cout << "RendererContextNoop::createIndexBuffer\n";
	std::cout << "_handle=" << _handle.idx << "\n";
	std::cout << "_size=" << _size << "\n";
	std::cout << "_pData=" << _pData << "\n";
	std::cout << "_flags=" << static_cast<uint32_t>(_flags) << "\n\n";
}

void RendererContextNoop::destroyIndexBuffer(IndexBufferHandle _handle) {
	(void)_handle;
	std::cout << "RendererContextNoop::destroyIndexBuffer\n";
	std::cout << "_handle=" << _handle.idx << "\n\n";
}

void RendererContextNoop::createDynamicVertexBuffer(VertexBufferHandle _handle, uint32_t _size, VertexLayoutHandle _layoutHandle, BufferFlags _flags) {
	(void)_handle; (void)_size; (void)_layoutHandle; (void)_flags;
	std::cout << "RendererContextNoop::createDynamicVertexBuffer\n";
	std::cout << "_handle=" << _handle.idx << "\n";
	std::cout << "_size=" << _size << "\n";
	std::cout << "_layoutHandle=" << _layoutHandle.idx << "\n";
	std::cout << "_flags=" << static_cast<uint32_t>(_flags) << "\n\n";
}

void RendererContextNoop::updateDynamicVertexBuffer(VertexBufferHandle _handle, uint32_t _dstOffset, uint32_t _size, void* _pData) {
	(void)_handle; (void)_dstOffset; (void)_size; (void)_pData;
	std::cout << "RendererContextNoop::updateDynamicVertexBuffer\n";
	std::cout << "_handle=" << _handle.idx << "\n";
	std::cout << "_dstOffset=" << _dstOffset << "\n";
	std::cout << "_size=" << _size << "\n";
	std::cout << "_pData=" << _pData << "\n\n";
}

void RendererContextNoop::destroyDynamicVertexBuffer(VertexBufferHandle _handle) {
	(void)_handle;
	std::cout << "RendererContextNoop::destroyDynamicVertexBuffer\n";
	std::cout << "_handle=" << _handle.idx << "\n\n";
}

void RendererContextNoop::createDynamicIndexBuffer(IndexBufferHandle _handle, uint32_t _size, BufferFlags _flags) {
	(void)_handle; (void)_size; (void)_flags;
	std::cout << "RendererContextNoop::createDynamicIndexBuffer\n";
	std::cout << "_handle=" << _handle.idx << "\n";
	std::cout << "_size=" << _size << "\n";
	std::cout << "_flags=" << static_cast<uint32_t>(_flags) << "\n\n";
}

void RendererContextNoop::updateDynamicIndexBuffer(IndexBufferHandle _handle, uint32_t _dstOffset, uint32_t _size, void* _pData) {
	(void)_handle; (void)_dstOffset; (void)_size; (void)_pData;
	std::cout << "RendererContextNoop::updateDynamicIndexBuffer\n";
	std::cout << "_handle=" << _handle.idx << "\n";
	std::cout << "_dstOffset=" << _dstOffset << "\n";
	std::cout << "_size=" << _size << "\n";
	std::cout << "_pData=" << _pData << "\n\n";
}

void RendererContextNoop::destroyDynamicIndexBuffer(IndexBufferHandle _handle) {
	(void)_handle;
	std::cout << "RendererContextNoop::destroyDynamicIndexBuffer\n";
	std::cout << "_handle=" << _handle.idx << "\n\n";
}

void RendererContextNoop::createShader(ShaderHandle _handle, ShaderType _shaderType, const char* _source) {
	(void)_handle; (void)_shaderType; (void)_source;
	std::cout << "RendererContextNoop::createShader\n";
	std::cout << "_handle=" << _handle.idx << "\n";
	std::cout << "_shaderType=" << static_cast<uint32_t>(_shaderType) << "\n";
	std::cout << "_source=" << "\n" << (_source ? _source : "nullptr") << "\n\n";
}

void RendererContextNoop::destroyShader(ShaderHandle _handle) {
	(void)_handle;
	std::cout << "RendererContextNoop::destroyShader\n";
	std::cout << "_handle=" << _handle.idx << "\n\n";
}

void RendererContextNoop::createProgram(ProgramHandle _handle, ShaderHandle _vert, ShaderHandle _frag) {
	(void)_handle; (void)_vert; (void)_frag;
	std::cout << "RendererContextNoop::createProgram\n";
	std::cout << "_handle=" << _handle.idx << "\n";
	std::cout << "_vert=" << _vert.idx << "\n";
	std::cout << "_frag=" << _frag.idx << "\n\n";
}

void RendererContextNoop::destroyProgram(ProgramHandle _handle) {
	(void)_handle;
	std::cout << "RendererContextNoop::destroyProgram\n";
	std::cout << "_handle=" << _handle.idx << "\n\n";
}

void RendererContextNoop::createTexture(
	TextureHandle _handle,
	TexTarget _target, TexFormat _internalFormat,
	uint16_t _width, uint16_t _height, uint16_t _depth,
	uint8_t _numLayers, bool _useMips,
	TexFormat _format,
	void* _pData, TexFlags _flags
) {
	(void)_handle; (void)_target; (void)_internalFormat;
	(void)_width; (void)_height; (void)_depth;
	(void)_numLayers; (void)_useMips;
	(void)_format; (void)_pData; (void)_flags;
	std::cout << "RendererContextNoop::createTexture\n";
	std::cout << "_handle=" << _handle.idx << "\n";
	std::cout << "_target=" << static_cast<uint32_t>(_target) << "\n";
	std::cout << "_internalFormat=" << static_cast<uint32_t>(_internalFormat) << "\n";
	std::cout << "_width=" << _width << "\n";
	std::cout << "_height=" << _height << "\n";
	std::cout << "_depth=" << _depth << "\n";
	std::cout << "_numLayers=" << static_cast<uint32_t>(_numLayers) << "\n";
	std::cout << "_useMips=" << _useMips << "\n";
	std::cout << "_format=" << static_cast<uint32_t>(_format) << "\n";
	std::cout << "_pData=" << _pData << "\n";
	std::cout << "_flags=" << static_cast<uint64_t>(_flags) << "\n\n";
}

void RendererContextNoop::updateTexture(
	TextureHandle _handle,
	uint16_t _width, uint16_t _height, uint16_t _depth,
	uint8_t _layer, uint8_t _face,
	void* _pData
) {
	(void)_handle; (void)_width; (void)_height; (void)_depth;
	(void)_layer; (void)_face; (void)_pData;
	std::cout << "RendererContextNoop::updateTexture\n";
	std::cout << "_handle=" << _handle.idx << "\n";
	std::cout << "_width=" << _width << "\n";
	std::cout << "_height=" << _height << "\n";
	std::cout << "_depth=" << _depth << "\n";
	std::cout << "_layer=" << static_cast<uint32_t>(_layer) << "\n";
	std::cout << "_face=" << static_cast<uint32_t>(_face) << "\n";
	std::cout << "_pData=" << _pData << "\n\n";
}

void RendererContextNoop::readTexture(
	TextureHandle _handle,
	uint8_t _layer, uint8_t _face,
	void* _data
) {
	(void)_handle; (void)_data; (void)_layer; (void)_face;
	std::cout << "RendererContextNoop::readTexture\n";
	std::cout << "_handle=" << _handle.idx << "\n";
	std::cout << "_layer=" << static_cast<uint32_t>(_layer) << "\n";
	std::cout << "_face=" << static_cast<uint32_t>(_face) << "\n";
	std::cout << "_data=" << _data << "\n\n";
}

void RendererContextNoop::destroyTexture(TextureHandle _handle) {
	(void)_handle;
	std::cout << "RendererContextNoop::destroyTexture\n";
	std::cout << "_handle=" << _handle.idx << "\n\n";
}

// void RendererContextNoop::createFrameBuffer(FrameBufferHandle _handle, uint16_t _width, uint16_t _height, TexFormat _format, TexFormat _depthFormat) {
//     (void)_handle; (void)_width; (void)_height; (void)_format; (void)_depthFormat;
// }

void RendererContextNoop::createFrameBuffer(FrameBufferHandle _handle, uint8_t _num, const Attachment* _pAttachments) {
	(void)_handle; (void)_num; (void)_pAttachments;
	std::cout << "RendererContextNoop::createFrameBuffer\n";
	std::cout << "_handle=" << _handle.idx << "\n";
	std::cout << "_num=" << static_cast<uint32_t>(_num) << "\n";
	for (int i = 0; i < _num; i++) {
		std::cout << "_pAttachments[" << i << "].attachmentSlot=" << static_cast<uint32_t>(_pAttachments[i].attachmentSlot) << "\n";
		std::cout << "_pAttachments[" << i << "].texHandle=" << _pAttachments[i].texHandle.idx << "\n";
	}
	std::cout << "\n";
}

void RendererContextNoop::destroyFrameBuffer(FrameBufferHandle _handle) {
	(void)_handle;
	std::cout << "RendererContextNoop::destroyFrameBuffer\n";
	std::cout << "_handle=" << _handle.idx << "\n\n";
}

} // namespace noop

} // namespace vgl

