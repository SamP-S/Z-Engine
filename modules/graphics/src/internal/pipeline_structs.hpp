#pragma once

// std libs
#include <memory>
#include <bitset>
#include <atomic>
// internal libs
#include "vgl/defines.hpp"
#include "vgl/g_structs.hpp"
#include "internal/command_buffer.hpp"

namespace vgl {

// vertex streams
struct VertexStream {
	uint32_t 			startVertex = 0;
	uint32_t			numVertices = 0;
	VertexBufferHandle 	vertexBufferHandle = VGL_INVALID_HANDLE;
	// VertexLayoutHandle 	vertexLayoutHandle = VGL_INVALID_HANDLE;

	void reset() {
		startVertex = 0;
		numVertices = 0;
		vertexBufferHandle = { VGL_INVALID_HANDLE };
		// vertexLayoutHandle = VGL_INVALID_HANDLE;
	}
};

struct IndexStream {
	uint32_t 			startIndex = 0;
	uint32_t			numIndices = 0;
	IndexBufferHandle 	indexBufferHandle = { VGL_INVALID_HANDLE };

	void reset() {
		startIndex = 0;
		numIndices = 0;
		indexBufferHandle = { VGL_INVALID_HANDLE };
	}
};

// rectangle for viewport and scissor
struct Rect {
	int16_t 	x = 0;
	int16_t 	y = 0;
	uint16_t 	width = 0;
	uint16_t 	height = 0;

	bool isZero() const {
		return 0 == (x + y + width + height);
	}
	bool isZeroArea() const {
		return 0 == (width * height);
	}
	void reset() {
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}
};

// clear function
/// TODO:
// add multiple colour clearing
struct Clear {
	ClearFlags 	flags = VGL_CLEAR_NONE;
	uint32_t 	colour = 0x303030ff;
	float 		depth = 1.0f;
	uint8_t 	stencil = 0;

	void reset() {
		flags = VGL_CLEAR_NONE;
		colour = 0x303030ff;
		depth = 1.0f;
		stencil = 0;
	}
};

// uniform data
struct Uniform {
	char 			name[VGL_MAX_UNIFORM_NAME_LENGTH] = {0};
	UniformType 	type = VGL_UNIFORM_TYPE_COUNT;
	void* 			pData = nullptr;
	uint16_t 		numElements = 0;
	bool			transpose = false;

	void reset() {
		memset(name, 0, VGL_MAX_UNIFORM_NAME_LENGTH);
		type = VGL_UNIFORM_TYPE_COUNT;
		pData = nullptr;
		numElements = 0;
		transpose = false;
	}
};

// texture data
struct TextureUnit {
	char 			name[VGL_MAX_UNIFORM_NAME_LENGTH] = {0};
	TextureHandle 	handle = VGL_INVALID_HANDLE;

	void reset() {
		memset(name, 0, VGL_MAX_UNIFORM_NAME_LENGTH);
		handle = { VGL_INVALID_HANDLE };
	}
};

// single draw instance
struct DrawCall {
	// view
	ViewID 				viewID = UINT16_MAX;
	// vertices
	VertexStream 		vertexStreams[VGL_MAX_VERTEX_STREAMS];
	std::bitset<VGL_MAX_VERTEX_STREAMS>	vertexStreamsSet = std::bitset<VGL_MAX_VERTEX_STREAMS>();
	uint32_t			numVertices = 0;
	// indices
	IndexStream			indexStream = {};
	// state
	StateFlags 			state = VGL_STATE_DEFAULT;
	StencilFlags		frontStencil = VGL_STENCIL_DEFAULT;
	StencilFlags		backStencil = VGL_STENCIL_DEFAULT;
	Rect				scissor = {};
	// textures
	TextureUnit			textureUnits[VGL_MAX_TEXTURE_UNITS];
	std::bitset<VGL_MAX_TEXTURE_UNITS>	textureUnitsSet = std::bitset<VGL_MAX_TEXTURE_UNITS>();
	// uniforms
	Uniform 			uniforms[VGL_MAX_UNIFORMS];
	uint32_t			numUniforms = 0;
	// program
	ProgramHandle		programHandle = { VGL_INVALID_HANDLE };

	void discard(DiscardFlags _flags) {
		// vertex buffers
		if (0 != (_flags & VGL_DISCARD_VERTEX_STREAMS)) {
			for (int i = 0; i < VGL_MAX_VERTEX_STREAMS; i++) {
				if (vertexStreamsSet.test(i)) {
					vertexStreams[i].reset();
				}
			}
			vertexStreamsSet = std::bitset<VGL_MAX_VERTEX_STREAMS>();
			numVertices = 0;
		}
		// index buffer
		if (0 != (_flags & VGL_DISCARD_INDEX_BUFFER)) {
			indexStream.reset();
		}
		// pipeline state
		if (0 != (_flags & VGL_DISCARD_STATE)) {
			state = VGL_STATE_DEFAULT;
			frontStencil = VGL_STENCIL_DEFAULT;
			backStencil = VGL_STENCIL_DEFAULT;
			scissor.reset();
		}
		// uniforms
		if (0 != (_flags & VGL_DISCARD_UNIFORMS)) {
			for (int i = 0; i < numUniforms; i++) {
				uniforms[i].reset();
			}
			numUniforms = 0;
		}
		// textures
		if (0 != (_flags & VGL_DISCARD_TEXTURES)) {
			for (int i = 0; i < VGL_MAX_TEXTURE_UNITS; i++) {
				if (textureUnitsSet.test(i)) {
					textureUnits[i].reset();
				}
			}
			textureUnitsSet = std::bitset<VGL_MAX_TEXTURE_UNITS>();
		}
		// program
		programHandle = { VGL_INVALID_HANDLE };
	}

	void reset() {
		discard(VGL_DISCARD_ALL);
	}
};

// view pipeline
struct View {
	Rect 				rect;
	Rect 				scissor;
	Clear 				clear;
	FrameBufferHandle 	fbh = { VGL_INVALID_HANDLE };

	void reset() {
		rect.reset();
		scissor.reset();
		clear.reset();
		fbh = { VGL_INVALID_HANDLE };
	}
};

// frame hold all draw calls and resource management for a single frame
/// TODO:
// - manage free handles
struct Frame {
	DrawCall 		drawCalls[VGL_MAX_DRAW_CALLS];
	std::atomic<uint32_t>	numDrawCalls{0};

	View 			views[VGL_MAX_VIEWS];
	CommandBuffer 	cmdPre;
	CommandBuffer 	cmdPost;

	void start() {
		numDrawCalls.store(0);
		cmdPre.start();
		cmdPost.start();
	}

	void finish() {
		cmdPre.finish();
		cmdPost.finish();
	}

	void reset() {

		// reset all draw calls
		for (int i = 0; i < VGL_MAX_DRAW_CALLS; i++) {
			drawCalls[i].reset();
		}
		numDrawCalls = 0;

		// reset all views
		for (int i = 0; i < VGL_MAX_VIEWS; i++) {
			views[i].reset();
		}
		
		// reset command buffers - do not use "reset" as used for re-read
		cmdPre.start();
		cmdPost.start();
		cmdPre.finish();
		cmdPost.finish();
		
		cmdPre.start();
		cmdPost.start();
	}
};

} // namespace vgl