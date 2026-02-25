#pragma once

// std lib
#include <cstdint>
#include <unordered_map>
// internal
#include "defines.hpp"

namespace vgl {

/// --- HANDLES --- ///
static const uint16_t kInvalidHandle = UINT16_MAX;

VGL_HANDLE(VertexLayoutHandle);
VGL_HANDLE(VertexBufferHandle);
VGL_HANDLE(IndexBufferHandle);
VGL_HANDLE(ShaderHandle);
VGL_HANDLE(ProgramHandle);
VGL_HANDLE(TextureHandle);
VGL_HANDLE(UniformHandle);
VGL_HANDLE(FrameBufferHandle);
	
/// --- PLATFORM & RENDERER DEPENDENCIES --- ///
struct Resolution {
	uint16_t width = 0;
	uint16_t height = 0;

	float aspect() const;
};

// init config
struct Config {

	// renderer
	RendererType rendererType = VGL_RENDERER_COUNT;
	uint16_t vendorID = 0;
	uint16_t deviceID = 0;

	// platform data
	void* pNativeDisplayType = nullptr;
	void* pNativeWindowHandle = nullptr;
	void* pContext = nullptr;
	void* pBackbuffer = nullptr;	// ??
	/// TODO: limits?

	// default frame buffer
	Resolution resolution = { 0, 0 };
};

struct Attachment {
	// Access 			access = VGL_ACCESS_READ_WRITE;
	AttachmentSlot	attachmentSlot = VGL_ATTACHMENT_COLOUR0;
	TextureHandle 	texHandle = { VGL_INVALID_HANDLE };
	// uint16_t		mipLevel = 0;
	// uint16_t		layer = 0;
	// uint16_t		numLayers = 0;
	// ResolveFlags	resolve = VGL_RESOLVE_NONE;
};

/// TODO: manage capabilities for different renderers/versions through struct/flag

// rendering statistics
struct Stats {
	// timing
	int64_t cpuTimeFrame = 0;
	int64_t cpuTimeBegin = 0;
	int64_t cpuTimeEnd = 0;
	int64_t gpuTimeBegin = 0;
	int64_t gpuTimeEnd = 0;
	// operation
	uint32_t numDrawCalls = 0;
	uint32_t numBlit = 0;
	// resources
	uint16_t numFrameBuffers = 0;
	uint16_t numIndexBuffers = 0;
	uint16_t numPrograms = 0;
	uint16_t numShaders = 0;
	uint16_t numTextures = 0;
	uint16_t numUniforms = 0;
	uint16_t numVertexBuffers = 0;
	uint16_t numVertexLayouts = 0;
	// primatives
	uint32_t numPrimatives[VGL_PRIM_TOPOLOGY_COUNT] = {};
	// backuffer
	uint16_t backbufferWidth = 0;
	uint16_t backbufferHeight = 0;
	// views
	uint16_t numViews = 0;
};

struct VertexAttribute {
	Attrib 		attrib = VGL_ATTRIB_COUNT;		// fixed shader location
	uint16_t 	offset = 0;						// byte offset into vertex
	uint16_t	size = 0;						// byte size of attribute
	AttribType	type = VGL_ATTRIB_TYPE_COUNT;	// element type
	uint8_t		numComponents = 0;				// number of components i.e. vector
	uint8_t 	numElements = 1;				// for arrays/matrices, default 1
	bool		normalised = false;				// int normalisation, default off
	bool		optional = false;				// ??
};

/// --- LIBRARY RESOURCES --- ///
class VertexLayout {
private:
	// current stride
	uint16_t m_stride;
	// array of attributes
	uint8_t m_numAttributes;
	VertexAttribute m_attributes[VGL_MAX_ATTRIBUTES];
	int8_t m_indexes[VGL_MAX_ATTRIBUTES];
	// state
	bool m_finished;

	static const std::unordered_map<AttribType, uint8_t> kAttribTypeSizeMap;

public:
	VertexLayout();
	~VertexLayout();

	// clear and open layout for vertex attributes
	VertexLayout& begin();  
	// close layout
	void end();
	
	// add attribute to vertex layout
	VertexLayout& add(
		Attrib _attrib,      // attribute semantic
		AttribType _type,    // element data type
		uint8_t _numComponents,   	// num of components i.e. 1, 2, 3 or 4
		uint8_t _numElements = 1,	// number of elements i.e. for array
		bool _normalised = false,	// normalise data to range 0.0 - 1.0
		bool _optional = false		// optional attribute
	);

	// skip num bytes in vertex stream
	VertexLayout& skip(uint8_t _num);

	// check layout has attribute
	bool has(Attrib _attrib) const;

	// get attribute relative offset from vertex
	uint16_t getOffset(Attrib _attrib) const;

	// get size of vertex buffer for number of vertices
	uint32_t getSize(uint32_t _num) const;

	// get vertex stride
	uint16_t getStride() const;

	// get dynamic attribute list & size
	const VertexAttribute* getAttributes(uint8_t& _numAttributes);
};

} // namespace vgl