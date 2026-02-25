#include "internal/context.hpp"

// std lib
#include <iostream>
#include <cstring>
#include <algorithm>
// internal
#include "internal/gl/gl_renderer_context.hpp"
#include "internal/noop/noop_renderer_context.hpp"
// external
#include "spdlog/spdlog.h"

namespace vgl {

const std::unordered_map<TexFormat, TexFormatInfo> Context::s_kFormatInfoMap = {
	{VGL_TEXTURE_FORMAT_R8U, 		{1, VGL_TEXTURE_LAYOUT_R, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_R8I, 		{1, VGL_TEXTURE_LAYOUT_R, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_R16U, 		{2, VGL_TEXTURE_LAYOUT_R, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_R16I, 		{2, VGL_TEXTURE_LAYOUT_R, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_R16F, 		{2, VGL_TEXTURE_LAYOUT_R, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_R32U, 		{4, VGL_TEXTURE_LAYOUT_R, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_R32I, 		{4, VGL_TEXTURE_LAYOUT_R, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_R32F, 		{4, VGL_TEXTURE_LAYOUT_R, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RG8U, 		{2, VGL_TEXTURE_LAYOUT_RG, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RG8I, 		{2, VGL_TEXTURE_LAYOUT_RG, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RG16U, 		{4, VGL_TEXTURE_LAYOUT_RG, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RG16I, 		{4, VGL_TEXTURE_LAYOUT_RG, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RG16F, 		{4, VGL_TEXTURE_LAYOUT_RG, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RG32U, 		{8, VGL_TEXTURE_LAYOUT_RG, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RG32I, 		{8, VGL_TEXTURE_LAYOUT_RG, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RG32F, 		{8, VGL_TEXTURE_LAYOUT_RG, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RGB8U, 		{3, VGL_TEXTURE_LAYOUT_RGB, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RGB8I, 		{3, VGL_TEXTURE_LAYOUT_RGB, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RGB16U, 	{6, VGL_TEXTURE_LAYOUT_RGB, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RGB16I, 	{6, VGL_TEXTURE_LAYOUT_RGB, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RGB16F, 	{6, VGL_TEXTURE_LAYOUT_RGB, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RGB32U, 	{12, VGL_TEXTURE_LAYOUT_RGB, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RGB32I, 	{12, VGL_TEXTURE_LAYOUT_RGB, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RGB32F, 	{12, VGL_TEXTURE_LAYOUT_RGB, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RGBA8U, 	{4, VGL_TEXTURE_LAYOUT_RGBA, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RGBA8I, 	{4, VGL_TEXTURE_LAYOUT_RGBA, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RGBA16U, 	{8, VGL_TEXTURE_LAYOUT_RGBA, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RGBA16I, 	{8, VGL_TEXTURE_LAYOUT_RGBA, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RGBA16F, 	{8, VGL_TEXTURE_LAYOUT_RGBA, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RGBA32U, 	{16, VGL_TEXTURE_LAYOUT_RGBA, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RGBA32I, 	{16, VGL_TEXTURE_LAYOUT_RGBA, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_RGBA32F, 	{16, VGL_TEXTURE_LAYOUT_RGBA, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_BGR8U, 		{3, VGL_TEXTURE_LAYOUT_BGR, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_BGR8I, 		{3, VGL_TEXTURE_LAYOUT_BGR, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_BGR16U, 	{6, VGL_TEXTURE_LAYOUT_BGR, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_BGR16I, 	{6, VGL_TEXTURE_LAYOUT_BGR, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_BGR16F, 	{6, VGL_TEXTURE_LAYOUT_BGR, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_BGR32U, 	{12, VGL_TEXTURE_LAYOUT_BGR, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_BGR32I, 	{12, VGL_TEXTURE_LAYOUT_BGR, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_BGR32F, 	{12, VGL_TEXTURE_LAYOUT_BGR, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_BGRA8U, 	{4, VGL_TEXTURE_LAYOUT_BGRA, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_BGRA8I, 	{4, VGL_TEXTURE_LAYOUT_BGRA, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_BGRA16U, 	{8, VGL_TEXTURE_LAYOUT_BGRA, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_BGRA16I, 	{8, VGL_TEXTURE_LAYOUT_BGRA, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_BGRA16F, 	{8, VGL_TEXTURE_LAYOUT_BGRA, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_BGRA32U, 	{16, VGL_TEXTURE_LAYOUT_BGRA, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_BGRA32I, 	{16, VGL_TEXTURE_LAYOUT_BGRA, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_BGRA32F, 	{16, VGL_TEXTURE_LAYOUT_BGRA, VGL_ATTACHMENT_TYPE_COLOUR}},
	{VGL_TEXTURE_FORMAT_D16F, 		{2, VGL_TEXTURE_LAYOUT_DEPTH, VGL_ATTACHMENT_TYPE_DEPTH}},
	{VGL_TEXTURE_FORMAT_D24F, 		{3, VGL_TEXTURE_LAYOUT_DEPTH, VGL_ATTACHMENT_TYPE_DEPTH}},
	{VGL_TEXTURE_FORMAT_D32F, 		{4, VGL_TEXTURE_LAYOUT_DEPTH, VGL_ATTACHMENT_TYPE_DEPTH}},
	{VGL_TEXTURE_FORMAT_D24FS8U, 	{4, VGL_TEXTURE_LAYOUT_DEPTH_STENCIL, VGL_ATTACHMENT_TYPE_DEPTH_STENCIL}},
	{VGL_TEXTURE_FORMAT_D32FS8U, 	{5, VGL_TEXTURE_LAYOUT_DEPTH_STENCIL, VGL_ATTACHMENT_TYPE_DEPTH_STENCIL}},	// ?? could be 8 i.e. 5 pad 3
	{VGL_TEXTURE_FORMAT_S8U, 		{1, VGL_TEXTURE_LAYOUT_STENCIL, VGL_ATTACHMENT_TYPE_STENCIL}},
	{VGL_TEXTURE_FORMAT_S16U, 		{2, VGL_TEXTURE_LAYOUT_STENCIL, VGL_ATTACHMENT_TYPE_STENCIL}}
};

const std::unordered_map<AttachmentSlot, AttachmentType> Context::s_kAttachmentSlotTypeMap = {
	{VGL_ATTACHMENT_COLOUR0, 		VGL_ATTACHMENT_TYPE_COLOUR},
	{VGL_ATTACHMENT_COLOUR1, 		VGL_ATTACHMENT_TYPE_COLOUR},
	{VGL_ATTACHMENT_COLOUR2, 		VGL_ATTACHMENT_TYPE_COLOUR},
	{VGL_ATTACHMENT_COLOUR3, 		VGL_ATTACHMENT_TYPE_COLOUR},
	{VGL_ATTACHMENT_COLOUR4, 		VGL_ATTACHMENT_TYPE_COLOUR},
	{VGL_ATTACHMENT_COLOUR5, 		VGL_ATTACHMENT_TYPE_COLOUR},
	{VGL_ATTACHMENT_COLOUR6, 		VGL_ATTACHMENT_TYPE_COLOUR},
	{VGL_ATTACHMENT_COLOUR7, 		VGL_ATTACHMENT_TYPE_COLOUR},
	{VGL_ATTACHMENT_DEPTH, 			VGL_ATTACHMENT_TYPE_DEPTH},		
	{VGL_ATTACHMENT_STENCIL, 		VGL_ATTACHMENT_TYPE_STENCIL},
	{VGL_ATTACHMENT_DEPTH_STENCIL, 	VGL_ATTACHMENT_TYPE_DEPTH_STENCIL}
};



CommandBuffer& Context::getCommandBuffer(Command _cmdEnum) {
	if (_cmdEnum < Command::End) {
		m_pSubmit->cmdPre.write<Command>(_cmdEnum);
		return m_pSubmit->cmdPre;
	} else {
		m_pSubmit->cmdPost.write<Command>(_cmdEnum);
		return m_pSubmit->cmdPost;
	}
}

Context::Context() 
	: m_numFrames(0) {
	// set frame pointers
	m_pSubmit = &m_aFrames[0];
	m_pRender = &m_aFrames[1];

	/// TODO: set start frame time
	// m_lastFrameTimestamp = 0;

	// set debug level
	spdlog::set_level(spdlog::level::info);
	// spdlog::set_level(spdlog::level::trace);
}

Context::~Context() {
	// todo
}

bool Context::init(const Config& _config) {
	// validate render type and renderer config data provided
	switch (_config.rendererType) {
		case VGL_RENDERER_NOOP:
			std::cout << "INFO: No Operation renderer selected." << std::endl;
			m_rendererCtx = new noop::RendererContextNoop();
			break;
		case VGL_RENDERER_OPENGL:
			std::cout << "INFO: OpenGL renderer selected." << std::endl;
			m_rendererCtx = new gl::RendererContextGL();
			break;
		case VGL_RENDERER_VULKAN:
			std::cout << "ERROR: Vulkan renderer not implemented." << std::endl;
			return false;
		default:
			std::cout << "ERROR: Cannot init VGL, render type invalid." << std::endl;
			return false;
	}

	// start frames for first use
	m_pSubmit->start();
	m_pRender->start();
	
	// valid config provided
	m_config = _config;
	return m_rendererCtx->init(m_config);
}

void Context::shutdown() {
	// free renderer
	if (m_rendererCtx != nullptr) {
		m_rendererCtx->shutdown();
		delete m_rendererCtx;
		m_rendererCtx = nullptr;
	}
}

void Context::window(uint16_t _width, uint16_t _height) {
	m_config.resolution = {_width, _height};
	m_rendererCtx->window(_width, _height);
}

/// TODO: modify encoder alloc/dealloc to allow for reuse within frame ticks
Encoder* Context::begin() {
	std::unique_lock<std::mutex> lock(m_encoderLock);
	uint16_t idx = m_encoderAllocator.alloc();
	if (idx == kInvalidHandle) {
		std::cout << "WARNING: No encoders available, max encoders allocated." << std::endl;
		return nullptr;
	}
	EncoderImpl* encoder = &m_encoders[idx];
	encoder->_begin(m_pSubmit);
	return reinterpret_cast<Encoder*>(encoder);
}

void Context::end(Encoder* _pEncoder) {
	if (_pEncoder == nullptr) {
		std::cout << "WARNING: Can't end null encoder pointer." << std::endl;
		return;
	}
	EncoderImpl* encoder = static_cast<EncoderImpl*>(_pEncoder);
	encoder->_end();
}

void Context::frame() {
	// finish encoders
	uint16_t numEncoders = m_encoderAllocator.getNumAllocated();
	for (uint16_t i = 0; i < numEncoders; i++) {
		m_encoders[i]._end();
	}
	m_encoderAllocator.reset();

	/// TODO:
	// - reset free handles?
	// - update frame resolution & properties from init/pushed from user

	// rendering
	m_pSubmit->finish();

	// swap frame encodings
	Frame* pTmp = m_pSubmit;
	m_pSubmit = m_pRender;
	m_pRender = pTmp;

	// start next frame
	m_pSubmit->start();

	/// TODO:
	// - stats
}

// Renders given frame flushing pre & post command buffers
// returns 0 if failed otherwise frame index (1:)
uint32_t Context::render() {
	if (m_rendererCtx == nullptr) {
		std::cout << "ERROR: Cannot render, renderer is null." << std::endl;
		return 0;
	}
	execCommands(m_pRender->cmdPre);
	m_rendererCtx->render(m_pRender);
	execCommands(m_pRender->cmdPost);
	return ++m_numFrames;
}

void Context::execCommands(CommandBuffer& _cmdBuf) {
	_cmdBuf.reset();
	bool end = false;
	do {
		Command command;
		_cmdBuf.read<Command>(command);
		
		switch(command) {
			case Command::RendererInit: {
				std::cout << "Command::RendererInit: not implemented." << std::endl;
				break;
			}
			case Command::RendererShutdownBegin: {
				std::cout << "Command::RendererShutdownBegin: not implemented." << std::endl;
				break;
			}
			case Command::CreateVertexLayout: {
				VertexLayoutHandle handle;
				_cmdBuf.read<VertexLayoutHandle>(handle);
				VertexLayout layout;
				_cmdBuf.read<VertexLayout>(layout);
				m_rendererCtx->createVertexLayout(handle, layout);
				break;
			}
			case Command::CreateVertexBuffer: {
				VertexBufferHandle handle;
				_cmdBuf.read<VertexBufferHandle>(handle);
				uint32_t size;
				_cmdBuf.read<uint32_t>(size);
				VertexLayoutHandle layoutHandle;
				_cmdBuf.read<VertexLayoutHandle>(layoutHandle);
				void* pData;
				_cmdBuf.read<void*>(pData);
				BufferFlags flags;
				_cmdBuf.read<BufferFlags>(flags);
				m_rendererCtx->createVertexBuffer(handle, size, layoutHandle, pData, flags);
				if (pData != nullptr) {
					free(pData);
				}
				break;
			}
			case Command::CreateIndexBuffer: {
				IndexBufferHandle handle;
				_cmdBuf.read<IndexBufferHandle>(handle);
				uint32_t size;
				_cmdBuf.read<uint32_t>(size);
				void* pData;
				_cmdBuf.read<void*>(pData);
				BufferFlags flags;
				_cmdBuf.read<BufferFlags>(flags);
				m_rendererCtx->createIndexBuffer(handle, size, pData, flags);
				if (pData != nullptr) {
					free(pData);
				}
				break;
			}
			case Command::CreateDynamicVertexBuffer: {
				std::cout << "Command::CreateDynamicVertexBuffer: not implemented." << std::endl;
				break;
			}
			case Command::UpdateDynamicVertexBuffer: {
				std::cout << "Command::UpdateDynamicVertexBuffer: not implemented." << std::endl;
				break;
			}
			case Command::CreateDynamicIndexBuffer: {
				std::cout << "Command::CreateDynamicIndexBuffer: not implemented." << std::endl;
				break;
			}
			case Command::UpdateDynamicIndexBuffer: {
				std::cout << "Command::UpdateDynamicIndexBuffer: not implemented." << std::endl;
				break;
			}
			case Command::CreateShader: {
				ShaderHandle handle;
				_cmdBuf.read<ShaderHandle>(handle);
				ShaderType type;
				_cmdBuf.read<ShaderType>(type);
				char* pSource;
				_cmdBuf.read<char*>(pSource);
				m_rendererCtx->createShader(handle, type, pSource);
				if (pSource != nullptr) {
					free(pSource);
				}
				break;
			}
			case Command::CreateProgram: {
				ProgramHandle handle;
				_cmdBuf.read<ProgramHandle>(handle);
				ShaderHandle vertHandle;
				_cmdBuf.read<ShaderHandle>(vertHandle);
				ShaderHandle fragHandle;
				_cmdBuf.read<ShaderHandle>(fragHandle);
				m_rendererCtx->createProgram(handle, vertHandle, fragHandle);
				break;
			}
			case Command::CreateTexture: {
				TextureHandle handle;
				_cmdBuf.read<TextureHandle>(handle);
				TexTarget target;
				_cmdBuf.read<TexTarget>(target);
				TexFormat internalFormat;
				_cmdBuf.read<TexFormat>(internalFormat);
				uint16_t width;
				_cmdBuf.read<uint16_t>(width);
				uint16_t height;
				_cmdBuf.read<uint16_t>(height);
				uint16_t depth;
				_cmdBuf.read<uint16_t>(depth);
				uint8_t numLayers;
				_cmdBuf.read<uint8_t>(numLayers);
				bool useMips;
				_cmdBuf.read<bool>(useMips);
				TexFormat format;
				_cmdBuf.read<TexFormat>(format);
				void* pData;
				_cmdBuf.read<void*>(pData);
				TexFlags flags;
				_cmdBuf.read<TexFlags>(flags);	/// TODO: fix error?
				m_rendererCtx->createTexture(
					handle, target,
					internalFormat,
					width, height, depth,
					numLayers, useMips,
					format,
					pData, flags
				);
				if (pData != nullptr) {
					free(pData);
				}
				break;
			}
			case Command::UpdateTexture: {
				std::cout << "Command::UpdateTexture: not implemented." << std::endl;
				break;
			}
			case Command::ResizeTexture: {
				std::cout << "Command::ResizeTexture: not implemented." << std::endl;
				break;
			}
			case Command::CreateFrameBuffer: {
				FrameBufferHandle handle;
				_cmdBuf.read<FrameBufferHandle>(handle);
				uint8_t num;
				_cmdBuf.read<uint8_t>(num);
				Attachment* pAttachments = (Attachment*)malloc(sizeof(Attachment) * num);
				for (int i = 0; i < num; i++) {
					_cmdBuf.read<Attachment>(pAttachments[i]);
				}
				m_rendererCtx->createFrameBuffer(handle, num, pAttachments);
				if (pAttachments != nullptr) {
					free(pAttachments);
				}
				break;
			}
			case Command::End: {
				end = true;
				break;
			}
			case Command::RendererShutdownEnd: {
				std::cout << "Command::RendererShutdownEnd: not implemented." << std::endl;
				break;
			}
			case Command::DestroyVertexLayout: {
				VertexLayoutHandle handle;
				_cmdBuf.read<VertexLayoutHandle>(handle);
				m_rendererCtx->destroyVertexLayout(handle);
				break;
			}
			case Command::DestroyVertexBuffer: {
				VertexBufferHandle handle;
				_cmdBuf.read<VertexBufferHandle>(handle);
				m_rendererCtx->destroyVertexBuffer(handle);
				break;
			}
			case Command::DestroyIndexBuffer: {
				IndexBufferHandle handle;
				_cmdBuf.read<IndexBufferHandle>(handle);
				m_rendererCtx->destroyIndexBuffer(handle);
				break;
			}
			case Command::DestroyDynamicVertexBuffer: {
				std::cout << "Command::DestroyDynamicVertexBuffer: not implemented." << std::endl;
				break;
			}
			case Command::DestroyDynamicIndexBuffer: {
				std::cout << "Command::DestroyDynamicIndexBuffer: not implemented." << std::endl;
				break;
			}
			case Command::DestroyShader: {
				ShaderHandle handle;
				_cmdBuf.read<ShaderHandle>(handle);
				m_rendererCtx->destroyShader(handle);
				break;
			}
			case Command::DestroyProgram: {
				ProgramHandle handle;
				_cmdBuf.read<ProgramHandle>(handle);
				m_rendererCtx->destroyProgram(handle);
				break;
			}
			case Command::DestroyTexture: {
				TextureHandle handle;
				_cmdBuf.read<TextureHandle>(handle);
				m_rendererCtx->destroyTexture(handle);
				break;
			}
			case Command::DestroyFrameBuffer: {
				FrameBufferHandle handle;
				_cmdBuf.read<FrameBufferHandle>(handle);
				m_rendererCtx->destroyFrameBuffer(handle);
				break;
			}
			default: {
				std::cout << "Unknown command: not implemented." << std::endl;
				break;
			}
		}
	} while (!end);
}

const Stats& Context::getPerformanceStats() {
	return m_performanceStats;
}

const Resolution& Context::getResolution() {
	return m_config.resolution;
}

const RendererType Context::getRendererType() {
	return m_rendererCtx->getRendererType();
}

const char* Context::getRendererName() {
	return m_rendererCtx->getRendererName();
}

VertexLayoutHandle Context::createVertexLayout(const VertexLayout& _layout) {
	// generate handle & validate
	VertexLayoutHandle handle = { m_vertexLayoutAllocator.alloc() };
	if (!isValid(handle)) {
		std::cout << "WARNING: Cannot create vertex layout, max allocation reached." << std::endl;
		return handle;
	}

	// update ref
	VertexLayoutRef& ref = m_vertexLayouts[handle.idx];
	ref.layout = _layout;
	ref.refCount = 1;

	// write to command buffer
	CommandBuffer& cmdBuf = getCommandBuffer(Command::CreateVertexLayout);
	cmdBuf.write<VertexLayoutHandle>(handle);
	cmdBuf.write<VertexLayout>(_layout);

	return handle;
}

void Context::destroy(VertexLayoutHandle _handle) {
	if (!isValid(_handle)) {
		std::cout << "WARNING: Cannot destroy vertex buffer of invalid handle." << std::endl;
		return;
	}

	// decr ref & destroy if no more refs
	vertexLayoutDecrRef(_handle);
}

VertexBufferHandle Context::createVertexBuffer(uint32_t _size, VertexLayoutHandle& _layoutHandle, const void* _pData, BufferFlags _flags) {
	// generate handle & validate
	VertexBufferHandle handle = { m_vertexBufferAllocator.alloc() };
	if (!isValid(handle)) {
		std::cout << "WARNING: Cannot create vertex buffer, max allocation reached." << std::endl;
		return handle;
	}

	// copy data if necessary
	void* pCopy = nullptr;
	if (_pData != nullptr && _size > 0) {
		pCopy = malloc(_size);
		memcpy(pCopy, _pData, _size);
	}

	// update ref
	VertexBufferRef& ref = m_vertexBuffers[handle.idx];
	ref.size = _size;
	ref.layoutHandle = _layoutHandle;
	ref.flags = _flags;
	vertexLayoutIncrRef(_layoutHandle);

	// write to command buffer
	CommandBuffer& cmdBuf = getCommandBuffer(Command::CreateVertexBuffer);
	cmdBuf.write<VertexBufferHandle>(handle);
	cmdBuf.write<uint32_t>(_size);
	cmdBuf.write<VertexLayoutHandle>(_layoutHandle);
	cmdBuf.write<void*>(pCopy);
	cmdBuf.write<BufferFlags>(_flags);

	return handle;
}

void Context::destroy(VertexBufferHandle _handle) {
	if (!isValid(_handle)) {
		std::cout << "WARNING: Cannot destroy vertex buffer of invalid handle." << std::endl;
		return;
	}

	VertexBufferRef& ref = m_vertexBuffers[_handle.idx];

	// write to command buffer
	CommandBuffer& cmdBuf = getCommandBuffer(Command::DestroyVertexBuffer);
	cmdBuf.write<VertexBufferHandle>(_handle);

	vertexLayoutDecrRef(ref.layoutHandle);
}

IndexBufferHandle Context::createIndexBuffer(uint32_t _size, const void* _pData, BufferFlags _flags) {
	// generate handle & validate
	IndexBufferHandle handle = { m_indexBufferAllocator.alloc() };
	if (!isValid(handle)) {
		std::cout << "WARNING: Cannot create index buffer, max allocation reached." << std::endl;
		return handle;
	}

	// copy data if necessary
	void* pCopy = nullptr;
	if (_pData != nullptr && _size > 0) {
		pCopy = malloc(_size);
		memcpy(pCopy, _pData, _size);
	}

	// write to command buffer
	CommandBuffer& cmdBuf = getCommandBuffer(Command::CreateIndexBuffer);
	cmdBuf.write<IndexBufferHandle>(handle);
	cmdBuf.write<uint32_t>(_size);
	cmdBuf.write<void*>(pCopy);
	cmdBuf.write<BufferFlags>(_flags);

	return handle;
}

void Context::destroy(IndexBufferHandle _handle) {
	if (!isValid(_handle)) {
		std::cout << "WARNING: Cannot destroy index buffer, invalid handle." << std::endl;
		return;
	}

	// write to command buffer
	CommandBuffer& cmdBuf = getCommandBuffer(Command::DestroyIndexBuffer);
	cmdBuf.write<IndexBufferHandle>(_handle);
}

ShaderHandle Context::createShader(ShaderType _shaderType, const char* _source) {
    // generate handle & validate
    ShaderHandle handle = { m_shaderAllocator.alloc() };
    if (!isValid(handle)) {
        std::cout << "WARNING: Cannot create shader, max allocation reached." << std::endl;
        return handle;
    }

    // copy source string
    size_t srcLen = strlen(_source) + 1;
    char* pCopy = (char*)malloc(srcLen);
    memcpy(pCopy, _source, srcLen);

	// update refs
	ShaderRef& ref = m_shaders[handle.idx];
	ref.type = _shaderType;
	ref.refCount = 1;

    // write command to command buffer
    CommandBuffer& cmdBuf = getCommandBuffer(Command::CreateShader);
    cmdBuf.write<ShaderHandle>(handle);
	cmdBuf.write<ShaderType>(_shaderType);
    cmdBuf.write<char*>(pCopy);

    return handle;
}

void Context::destroy(ShaderHandle _handle) {
    if (!isValid(_handle)) {
        std::cout << "WARNING: Cannot destroy shader, invalid handle." << std::endl;
        return;
    }

	// decr ref & destroy if no more refs
	shaderDecrRef(_handle);
}

ProgramHandle Context::createProgram(ShaderHandle _vert, ShaderHandle _frag) {
    ProgramHandle handle = { m_programAllocator.alloc() };
    if (!isValid(handle)) {
        std::cout << "WARNING: Cannot create program, max allocation reached." << std::endl;
        return handle;
    }

	// update ref
	ProgramRef& ref = m_programs[handle.idx];
	ref.vertexHandle = _vert;
	ref.fragmentHandle = _frag;
	shaderIncrRef(_vert);
	shaderIncrRef(_frag);

    // write command to command buffer
    CommandBuffer& cmdBuf = getCommandBuffer(Command::CreateProgram);
    cmdBuf.write<ProgramHandle>(handle);
    cmdBuf.write<ShaderHandle>(_vert);
    cmdBuf.write<ShaderHandle>(_frag);

    return handle;
}

void Context::destroy(ProgramHandle _handle) {
    if (!isValid(_handle)) {
        std::cout << "WARNING: Cannot destroy program, invalid handle." << std::endl;
        return;
    }

	// update ref
	ProgramRef& ref = m_programs[_handle.idx];
	shaderDecrRef(ref.vertexHandle);
	shaderDecrRef(ref.fragmentHandle);

	// write to command buffer
    CommandBuffer& cmdBuf = getCommandBuffer(Command::DestroyProgram);
    cmdBuf.write<ProgramHandle>(_handle);
}

TextureHandle Context::createTexture2D(
		TexFormat _internalFormat,
		uint16_t _width, uint16_t _height,
		uint8_t _numLayers, bool _useMips, 
		TexFormat _format,
		const void* _pData, TexFlags _flags
	) {
	// set target
	_numLayers = std::max(uint8_t(1), _numLayers);	
	TexTarget target = (_numLayers == 1) ? VGL_TEXTURE_TARGET_2D : VGL_TEXTURE_TARGET_2D_ARRAY;

	return this->createTexture(
		target, _internalFormat,
		_width, _height, 1,
		_numLayers, _useMips,
		_format,
		_pData, _flags
	);
}

TextureHandle Context::createTexture3D(
	TexFormat _internalFormat,
	uint16_t _width, uint16_t _height, uint16_t _depth,
	bool _useMips,
	TexFormat _format,
	const void* _pData, TexFlags _flags
) {
	// set target
	TexTarget target = VGL_TEXTURE_TARGET_3D;

	return this->createTexture(
		target, _internalFormat,
		_width, _height, _depth,
		1, _useMips,
		_format,
		_pData, _flags
	);
}

TextureHandle Context::createTextureCube(
	TexFormat _internalFormat,
	uint16_t _width,
	uint8_t _numLayers, bool _useMips,
	TexFormat _format,
	const void* _pData, TexFlags _flags
) {
	// set target
	_numLayers = std::max(uint8_t(1), _numLayers);
	TexTarget target = (_numLayers == 1) ? VGL_TEXTURE_TARGET_CUBE : VGL_TEXTURE_TARGET_CUBE_ARRAY;
	
	return this->createTexture(
		target, _internalFormat,
		_width, _width, 1,
		_numLayers, _useMips,
		_format,
		_pData, _flags
	);
}

TextureHandle Context::createTexture(
	TexTarget _target, TexFormat _internalFormat,
	uint16_t _width, uint16_t _height, uint16_t _depth,
	uint8_t _numLayers, bool _useMips,
	TexFormat _format,
	const void* _pData, TexFlags _flags
) {
	// calc properties
	TexFormatInfo fmtInfo = s_kFormatInfoMap.at(_format);
	uint32_t size = _width * _height * _depth * _numLayers * fmtInfo.size;

	// validate arguments
	if (_internalFormat >= VGL_TEXTURE_FORMAT_COUNT) {
		std::cout << "WARNING: Cannot create texture, internal format invalid." << std::endl;
		return TextureHandle::invalid();
	} else if (_format >= VGL_TEXTURE_FORMAT_COUNT) {
		std::cout << "WARNING: Cannot create texture, user format invalid." << std::endl;
		return TextureHandle::invalid();
	} else if (!isTextureFormatsValid(_internalFormat, _format)) {
		std::cout << "WARNING: Cannot create texture, invalid format combination." << std::endl;
		return TextureHandle::invalid();
	} else if (size == 0) {
		std::cout << "WARNING: Cannot create texture, size == 0." << std::endl;
		return TextureHandle::invalid();
	}

	/// TODO: add more validation:
	// - internal cannot be BGR(A)
	// - depth/stencil/depthstencil cannot have mipmaps
	// - depth/stencil/depthstencil only supports 2D targets
	// - validate internal & external dont cross types i.e. RGBA_integer matches RGBA_integer

	// allocate handle
	TextureHandle handle = { m_textureAllocator.alloc() };
	if (!isValid(handle)) {
		std::cout << "WARNING: Cannot create texture, max allocation reached." << std::endl;
		return handle;
	}

	// copy data if available
	void* pCopy = nullptr;
	if (_pData != nullptr) {
		pCopy = malloc(size);
		memcpy(pCopy, _pData, size);
	}

	// update ref
	TextureRef& ref = m_textures[handle.idx];
	ref.target = _target;
	ref.internalFormat = _internalFormat;
	ref.width = _width;
	ref.height = _height;
	ref.depth = _depth;
	ref.numLayers = 1;
	ref.useMips = _useMips;
	ref.format = _format;
	ref.flags = _flags;
	ref.refCount = 1;

	// write to command buffer
	CommandBuffer& cmdBuf = getCommandBuffer(Command::CreateTexture);
	cmdBuf.write<TextureHandle>(handle);		// allocated handle
	cmdBuf.write<TexTarget>(_target);			// texture target
	cmdBuf.write<TexFormat>(_internalFormat);	// internal format
	cmdBuf.write<uint16_t>(_width);				// width
	cmdBuf.write<uint16_t>(_height);			// height
	cmdBuf.write<uint16_t>(_depth);				// depth
	cmdBuf.write<uint8_t>(1);					// num layers
	cmdBuf.write<bool>(_useMips);				// generate mip maps
	cmdBuf.write<TexFormat>(_format);			// user data format
	cmdBuf.write<void*>(pCopy);					// copy of user data
	cmdBuf.write<TexFlags>(_flags);				// texture parameters as flags

	return handle;
}

void Context::destroy(TextureHandle _handle) {
    if (!isValid(_handle)) {
        std::cout << "WARNING: Cannot destroy texture, invalid handle." << std::endl;
        return;
    }

	// decr ref & destroy if no more refs
	textureDecrRef(_handle);
}

FrameBufferHandle Context::createFrameBuffer(
	TextureHandle _colourTex, TextureHandle _depthStencilTex
) {
	Attachment attachments[2];
	attachments[0].attachmentSlot = VGL_ATTACHMENT_COLOUR0;
	attachments[0].texHandle = _colourTex;
	attachments[1].attachmentSlot = VGL_ATTACHMENT_DEPTH_STENCIL;
	attachments[1].texHandle = _depthStencilTex;

	return createFrameBuffer(2, attachments);
}

FrameBufferHandle Context::createFrameBuffer(
	uint8_t _num, const Attachment* _attachments
) {
	if (_num == 0 || _num >= VGL_MAX_COLOUR_ATTACHMENTS+2) {
		std::cout << "WARNING: Cannot create framebuffer, invalid number of attachments (" << _num << ")." << std::endl;
	}

	bool hasDepth = false;
	bool hasStencil = false;
	for (int i = 0; i < _num; i++) {
		const Attachment& a = _attachments[i];
		// check attachment is valid
		if (!isAttachmentValid(a)) {
			std::cout << "WARNING: Cannot create framebuffer, invalid attachment." << std::endl;
			return FrameBufferHandle::invalid();
		}
		TextureRef& ref = m_textures[a.texHandle.idx];
		// check attachment slot usage
		switch (a.attachmentSlot) {
			case VGL_ATTACHMENT_DEPTH:
				if (hasDepth) {
					std::cout << "WARNING: Cannot create framebuffer, multiple depth attachments." << std::endl;
					return FrameBufferHandle::invalid();
				}
				hasDepth = true;
				break;
			case VGL_ATTACHMENT_STENCIL:
				if (hasStencil) {
					std::cout << "WARNING: Cannot create framebuffer, multiple stencil attachments." << std::endl;
					return FrameBufferHandle::invalid();
				}
				hasStencil = true;
				break;
			case VGL_ATTACHMENT_DEPTH_STENCIL:
				if (hasDepth || hasStencil) {
					std::cout << "WARNING: Cannot create framebuffer, multiple depth or stencil attachments." << std::endl;
					return FrameBufferHandle::invalid();
				}
			default:
				break;
		}
	}

	FrameBufferHandle handle = { m_frameBufferAllocator.alloc() };
	if (!isValid(handle)) {
		std::cout << "WARNING: Cannot create frame buffer, max allocation reached." << std::endl;
		return handle;
	}

	// update ref
	FrameBufferRef& ref = m_frameBuffers[handle.idx];
	ref.numAttachments = _num;
	for (int i = 0; i < _num; i++) {
		ref.attachments[i] = _attachments[i];
		textureIncrRef(_attachments[i].texHandle);
	}

	// write to command buffer
	CommandBuffer& cmdBuf = getCommandBuffer(Command::CreateFrameBuffer);
	cmdBuf.write<FrameBufferHandle>(handle);
	cmdBuf.write<uint8_t>(_num);
	for (int i = 0; i < _num; i++) {
		cmdBuf.write<Attachment>(_attachments[i]);
	}

	return handle;
}

void Context::destroy(FrameBufferHandle _handle) {
	if (!isValid(_handle)) {
        std::cout << "WARNING: Cannot destroy framebuffer, invalid handle." << std::endl;
        return;
    }

    CommandBuffer& cmdBuf = getCommandBuffer(Command::DestroyFrameBuffer);
    cmdBuf.write<FrameBufferHandle>(_handle);
}

void Context::vertexLayoutIncrRef(VertexLayoutHandle _handle) {
	VertexLayoutRef& ref = m_vertexLayouts[_handle.idx];
	ref.refCount++;
}

void Context::vertexLayoutDecrRef(VertexLayoutHandle _handle) {
	VertexLayoutRef& ref = m_vertexLayouts[_handle.idx];
	ref.refCount--;

	if (ref.refCount == 0) {
		CommandBuffer& cmdBuf = getCommandBuffer(Command::DestroyVertexLayout);
		cmdBuf.write<VertexLayoutHandle>(_handle);
	}
}

void Context::shaderIncrRef(ShaderHandle _handle) {
	ShaderRef& ref = m_shaders[_handle.idx];
	ref.refCount++;
}

void Context::shaderDecrRef(ShaderHandle _handle) {
	ShaderRef& ref = m_shaders[_handle.idx];
	ref.refCount--;

	if (ref.refCount == 0) {
		CommandBuffer& cmdBuf = getCommandBuffer(Command::DestroyShader);
		cmdBuf.write<ShaderHandle>(_handle);
	}
}

void Context::textureIncrRef(TextureHandle _handle) {
	TextureRef& ref = m_textures[_handle.idx];
	ref.refCount++;
}

void Context::textureDecrRef(TextureHandle _handle) {
	TextureRef& ref = m_textures[_handle.idx];
	ref.refCount--;

	if (ref.refCount == 0) {
		CommandBuffer& cmdBuf = getCommandBuffer(Command::DestroyTexture);
		cmdBuf.write<TextureHandle>(_handle);
	}
}

bool Context::isTextureFormatsValid(TexFormat _internalFormat, TexFormat _format) {
	if (_internalFormat >= VGL_TEXTURE_FORMAT_COUNT
		|| _format >= VGL_TEXTURE_FORMAT_COUNT) {
		return false;
	}
	TexFormatInfo internalInfo = s_kFormatInfoMap.at(_internalFormat);
	TexFormatInfo userInfo = s_kFormatInfoMap.at(_format);
	if (internalInfo.attachmentType != userInfo.attachmentType) {
		return false;
	}
	return true;
}

bool Context::isAttachmentValid(const Attachment& _attachment) {
	if (!isValid(_attachment.texHandle)) {
		return false;
	}
	TextureRef& ref = m_textures[_attachment.texHandle.idx];
	TexFormatInfo texInfo = s_kFormatInfoMap.at(ref.internalFormat);
	AttachmentType attachmentType = s_kAttachmentSlotTypeMap.at(_attachment.attachmentSlot);
	if (texInfo.attachmentType != attachmentType) {
		return false;
	}
	return true;
}

void Context::setViewRect(ViewID _id, int16_t _x, int16_t _y, uint16_t _width, uint16_t _height) {
	if (_id >= VGL_MAX_VIEWS) {
		std::cout << "WARNING: Cannot set on invalid view id." << std::endl;
		return;
	}
	Rect& rect = m_pSubmit->views[_id].rect;
	rect.x 		= _x;
	rect.y 		= _y;
	rect.width 	= _width;
	rect.height = _height;
}

void Context::setViewScissor(ViewID _id, uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height) {
	if (_id >= VGL_MAX_VIEWS) {
		std::cout << "WARNING: Cannot set on invalid view id." << std::endl;
		return;
	}
	Rect& scissor = m_pSubmit->views[_id].scissor;
	scissor.x 		= _x;
	scissor.y		= _y;
	scissor.width 	= _width;
	scissor.height 	= _height;
}

void Context::setViewClear(ViewID _id, ClearFlags _clearFlags, uint32_t _rgba, float _depth, uint8_t _stencil) {
	if (_id >= VGL_MAX_VIEWS) {
		std::cout << "WARNING: Cannot set on invalid view id." << std::endl;
		return;
	}
	Clear& clear 		= m_pSubmit->views[_id].clear;
	clear.flags 		= _clearFlags;
	clear.colour		= _rgba;
	clear.depth 		= _depth;
	clear.stencil 		= _stencil;
}

void Context::setViewFrameBuffer(ViewID _id, FrameBufferHandle _handle) {
	if (_id >= VGL_MAX_VIEWS) {
		std::cout << "WARNING: Cannot set on invalid view id." << std::endl;
		return;
	}
	FrameBufferHandle& fbh = m_pSubmit->views[_id].fbh;
	fbh = _handle;
}

void Context::resetView(ViewID _id) {
	if (_id >= VGL_MAX_VIEWS) {
		std::cout << "WARNING: Cannot set on invalid view id." << std::endl;
		return;
	}
	View& view = m_pSubmit->views[_id];
	view.reset();
}

} // namespace vgl
