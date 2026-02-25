#pragma once
// std lib
#include <bitset>
#include <vector>
#include <unordered_map>
#include <queue>
#include <mutex>
// internal libs
#include "vgl/defines.hpp"
#include "vgl/vgl.hpp"
#include "vgl/encoder.hpp"
#include "internal/encoder_impl.hpp"
#include "internal/i_renderer_context.hpp"

namespace vgl {

// handle allocator
template<typename T, uint32_t MAX>
class HandleAllocator {
private:
	std::bitset<MAX> m_allocated;
	uint32_t m_nextFree;

public:
	HandleAllocator()
		: m_nextFree(0) {
		m_allocated.reset();
	}

	// allocate a handle or return invalid handle if none available
	T alloc() {
		for (uint32_t i = 0; i < MAX; ++i) {
			uint32_t handle = (m_nextFree + i) % MAX;
			if (!m_allocated.test(handle)) {
				m_allocated.set(handle);
				m_nextFree = (handle + 1) % MAX;
				return static_cast<T>(handle);
			}
		}
		return VGL_INVALID_HANDLE;
	}

	// free a previously allocated handle
	void free(T handle) {
		uint32_t index = static_cast<uint32_t>(handle);
		if (index < MAX && m_allocated.test(index)) {
			m_allocated.reset(index);
			m_nextFree = index; // Update next free to the recently freed handle
		}
	}

	// check handle is free
	bool isAvailable(T handle) const {
		uint32_t index = static_cast<uint32_t>(handle);
		return index < MAX && m_allocated.test(index);
	}

	// check num of allocated handles
	T getNumAllocated() { 
		return m_allocated.count();
	}

	// reset allocated
	void reset() {
		m_allocated.reset();
	}
};

struct TexFormatInfo {
	uint8_t 		size;
	TexLayout 		layout;
	AttachmentType 	attachmentType;
};

/// --- Resource Reference Structs --- ///
// note:
// - cpu copies of resource meta data
// - it does not need to clear data as the handle is used to determine if its in use
/// TODO:
// add dynamic buffers

struct VertexLayoutRef {
	VertexLayout layout;
	int16_t refCount;
};

struct VertexBufferRef {
	uint32_t size;
	VertexLayoutHandle layoutHandle;
	BufferFlags flags;
};

struct IndexBufferRef {
	uint32_t size;
	BufferFlags flags;
};

struct ShaderRef {
	ShaderType type;
	int16_t refCount;
};

struct ProgramRef {
	ShaderHandle vertexHandle;
	ShaderHandle fragmentHandle;
};

struct TextureRef {
	TexTarget target;
	TexFormat internalFormat;
	uint16_t width;
	uint16_t height;
	uint16_t depth;
	uint8_t numLayers;
	bool useMips;
	TexFormat format;
	TexFlags flags;
	int16_t refCount;
};

struct FrameBufferRef {
	uint8_t numAttachments;
	Attachment attachments[VGL_MAX_COLOUR_ATTACHMENTS + 2];
};

/// TODO:
// - add overall & frame stats
// - support seperate encoding/command & render thread

// graphics context layer
class Context {
private:
	// renderer required
	Config 				m_config;
	IRendererContext*	m_rendererCtx = nullptr;
	
	// render pipeline
	HandleAllocator<uint16_t, VGL_MAX_ENCODERS> m_encoderAllocator;
	EncoderImpl 		m_encoders[VGL_MAX_ENCODERS];
	std::bitset<VGL_MAX_ENCODERS> m_encodersSet;
	std::mutex 			m_encoderLock;
	Frame				m_aFrames[2];
	Frame* 				m_pSubmit;
	Frame* 				m_pRender;
	
	// statistics
	Stats 				m_performanceStats;
	int64_t 			m_lastFrameTimestamp = 0;
	uint32_t 			m_numFrames = 0;

	// resource allocators
	HandleAllocator<VertexLayoutHandle, VGL_MAX_VERTEX_LAYOUTS> m_vertexLayoutAllocator;
	HandleAllocator<VertexBufferHandle, VGL_MAX_VERTEX_BUFFERS> m_vertexBufferAllocator;
	HandleAllocator<IndexBufferHandle, VGL_MAX_INDEX_BUFFERS> m_indexBufferAllocator;
	HandleAllocator<ShaderHandle, VGL_MAX_SHADERS> m_shaderAllocator;
	HandleAllocator<ProgramHandle, VGL_MAX_PROGRAMS> m_programAllocator;
	HandleAllocator<FrameBufferHandle, VGL_MAX_FRAME_BUFFERS> m_frameBufferAllocator;
	HandleAllocator<TextureHandle, VGL_MAX_TEXTURES> m_textureAllocator;

	// resource refs
	VertexLayoutRef m_vertexLayouts[VGL_MAX_VERTEX_LAYOUTS];
	VertexBufferRef m_vertexBuffers[VGL_MAX_VERTEX_BUFFERS];
	IndexBufferRef 	m_indexBuffers[VGL_MAX_INDEX_BUFFERS];
	ShaderRef 		m_shaders[VGL_MAX_SHADERS];
	ProgramRef 		m_programs[VGL_MAX_PROGRAMS];
	TextureRef 		m_textures[VGL_MAX_TEXTURES];
	FrameBufferRef 	m_frameBuffers[VGL_MAX_FRAME_BUFFERS];

	// maps
	static const std::unordered_map<TexFormat, TexFormatInfo> s_kFormatInfoMap;
	static const std::unordered_map<AttachmentSlot, AttachmentType> s_kAttachmentSlotTypeMap;

	// command buffer utilities
	CommandBuffer& getCommandBuffer(Command _cmdEnum);
	
public:
	Context();
	~Context();

	bool init(const Config& _config = {});
	void shutdown();
	void window(uint16_t _width, uint16_t _height);
	Encoder* begin();
	void end(Encoder* _pEncoder);
	void frame();
	// execs encoded frame, call after swapping
	uint32_t render();
	void execCommands(CommandBuffer& _cmdBuf);

	// stats
	const Stats& getPerformanceStats();
	const Resolution& getResolution();
	const RendererType getRendererType();
	const char* getRendererName();

	/// --- RESOURCE CREATE/DESTROY --- ///
	/// TODO:
	// Resource get/update methods

	// Note: Resource Method arguments follow:
	// 1. Target
	// 2. Size/Format/Dimensions
	// 3. Data Pointer
	// 4. Flags

	VertexLayoutHandle createVertexLayout(const VertexLayout& _layout);
	void destroy(VertexLayoutHandle _handle);
	VertexBufferHandle createVertexBuffer(uint32_t _size, VertexLayoutHandle& _layoutHandle, const void* _pData, BufferFlags _flags = VGL_BUFFER_NONE);
	void destroy(VertexBufferHandle _handle);
	IndexBufferHandle createIndexBuffer(uint32_t _size, const void* _pData, BufferFlags _flags = VGL_BUFFER_NONE);
	void destroy(IndexBufferHandle _handle);
	ShaderHandle createShader(ShaderType _shaderType, const char* _source);
	void destroy(ShaderHandle _handle);
	ProgramHandle createProgram(ShaderHandle _vert, ShaderHandle _frag);
	void destroy(ProgramHandle _handle);

	TextureHandle createTexture2D(
		TexFormat _internalFormat,
		uint16_t _width, uint16_t _height,
		uint8_t _numLayers, bool _useMips,
		TexFormat _format,
		const void* _pData, TexFlags _flags
	);
	TextureHandle createTexture3D(
		TexFormat _internalFormat,
		uint16_t _width, uint16_t _height, uint16_t _depth,
		bool _useMips,
		TexFormat _format,
		const void* _pData, TexFlags _flags
	);
	TextureHandle createTextureCube(
		TexFormat _internalFormat,
		uint16_t _width,
		uint8_t _numLayers, bool _useMips,
		TexFormat _format,
		const void* _pData, TexFlags _flags
	);
	TextureHandle createTexture(
		TexTarget _target, TexFormat _internalFormat,
		uint16_t _width, uint16_t _height, uint16_t _depth,
		uint8_t _numLayers, bool _useMips,
		TexFormat _format,
		const void* _pData, TexFlags _flags
	);
	void destroy(TextureHandle _handle);

	FrameBufferHandle createFrameBuffer(
		TextureHandle _colourTex, TextureHandle _depthStencilTex
	);
	FrameBufferHandle createFrameBuffer(
		uint8_t _num, const Attachment* _pAttachments
	);
	void destroy(FrameBufferHandle _handle);

	/// --- RESOURCE REF MANAGEMENT --- ///
	void vertexLayoutIncrRef(VertexLayoutHandle _handle);
	void vertexLayoutDecrRef(VertexLayoutHandle _handle);
	void shaderIncrRef(ShaderHandle _handle);
	void shaderDecrRef(ShaderHandle _handle);
	void textureIncrRef(TextureHandle _handle);
	void textureDecrRef(TextureHandle _handle);


	/// --- RESOURCE HELPER VALIDATIONS --- ///
	bool isTextureFormatsValid(
		TexFormat _internalFormat,
		TexFormat _format
	);
	bool isAttachmentValid(const Attachment& _attachment);

	/// --- VIEW PIPELINE --- ///
	void setViewRect(ViewID _id, int16_t _x, int16_t _y, uint16_t _width, uint16_t _height);
	void setViewScissor(ViewID _id, uint16_t _x, uint16_t _y, uint16_t _width, uint16_t _height);
	void setViewClear(ViewID _id, ClearFlags _clearFlags, uint32_t _rgba, float _depth, uint8_t _stencil);
	void setViewFrameBuffer(ViewID _id, FrameBufferHandle _handle);
	void resetView(ViewID _id);

};


} // namespace vgl
