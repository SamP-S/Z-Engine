#include "internal/gl/gl_renderer_context.hpp"

// std libs
#include <cstring>
#include <cstdint>
#include <unordered_map>
#include <algorithm>
// external
#include "spdlog/spdlog.h"

/// TODO:
// - add handle x max checks and error logs

namespace vgl {

namespace gl {

const GLenum RendererContextGL::s_kShaderTypeMap[VGL_SHADER_TYPE_COUNT] = {
	GL_VERTEX_SHADER,       // VGL_SHADER_TYPE_VERTEX
	GL_FRAGMENT_SHADER,     // VGL_SHADER_TYPE_FRAGMENT
};

const GLenum RendererContextGL::s_kTexTargetMap[VGL_TEXTURE_TARGET_COUNT] = {
	GL_TEXTURE_1D,              // VGL_TEXTURE_TARGET_1D
	GL_TEXTURE_2D,              // VGL_TEXTURE_TARGET_2D
	GL_TEXTURE_3D,              // VGL_TEXTURE_TARGET_3D
	GL_TEXTURE_CUBE_MAP,        // VGL_TEXTURE_TARGET_CUBE
	GL_TEXTURE_1D_ARRAY,        // VGL_TEXTURE_TARGET_1D_ARRAY
	GL_TEXTURE_2D_ARRAY,        // VGL_TEXTURE_TARGET_2D_ARRAY
	GL_TEXTURE_CUBE_MAP_ARRAY,  // VGL_TEXTURE_TARGET_CUBE_ARRAY
};

const TexFormatInfoGL RendererContextGL::s_kTexFormatMap[VGL_TEXTURE_FORMAT_COUNT] = {
	// r
	{GL_R8,             GL_RED,     		GL_UNSIGNED_BYTE},      // VGL_TEXTURE_FORMAT_R8U
	{GL_R8_SNORM,       GL_RED,     		GL_BYTE},               // VGL_TEXTURE_FORMAT_R8I
	{GL_R16,            GL_RED,     		GL_UNSIGNED_SHORT},     // VGL_TEXTURE_FORMAT_R16U
	{GL_R16_SNORM,      GL_RED,     		GL_SHORT},              // VGL_TEXTURE_FORMAT_R16I
	{GL_R16F,           GL_RED,     		GL_HALF_FLOAT},         // VGL_TEXTURE_FORMAT_R16F
	{GL_R32UI,          GL_RED_INTEGER,     GL_UNSIGNED_INT},       // VGL_TEXTURE_FORMAT_R32U
	{GL_R32I,           GL_RED_INTEGER,     GL_INT},                // VGL_TEXTURE_FORMAT_R32I
	{GL_R32F,           GL_RED,     		GL_FLOAT},              // VGL_TEXTURE_FORMAT_R32F
	// rg
	{GL_RG8,            GL_RG,      		GL_UNSIGNED_BYTE},      // VGL_TEXTURE_FORMAT_RG8U
	{GL_RG8_SNORM,      GL_RG,      		GL_BYTE},               // VGL_TEXTURE_FORMAT_RG8I
	{GL_RG16,           GL_RG,      		GL_UNSIGNED_SHORT},     // VGL_TEXTURE_FORMAT_RG16U
	{GL_RG16_SNORM,     GL_RG,      		GL_SHORT},              // VGL_TEXTURE_FORMAT_RG16I
	{GL_RG16F,          GL_RG,      		GL_HALF_FLOAT},         // VGL_TEXTURE_FORMAT_RG16F
	{GL_RG32UI,         GL_RG_INTEGER,      GL_UNSIGNED_INT},       // VGL_TEXTURE_FORMAT_RG32U
	{GL_RG32I,          GL_RG_INTEGER,      GL_INT},                // VGL_TEXTURE_FORMAT_RG32I
	{GL_RG32F,          GL_RG,      		GL_FLOAT},              // VGL_TEXTURE_FORMAT_RG32F
	// rgb
	{GL_RGB8,           GL_RGB,     		GL_UNSIGNED_BYTE},      // VGL_TEXTURE_FORMAT_RGB8U
	{GL_RGB8_SNORM,     GL_RGB,     		GL_BYTE},               // VGL_TEXTURE_FORMAT_RGB8I
	{GL_RGB16,          GL_RGB,     		GL_UNSIGNED_SHORT},     // VGL_TEXTURE_FORMAT_RGB16U
	{GL_RGB16_SNORM,    GL_RGB,     		GL_SHORT},              // VGL_TEXTURE_FORMAT_RGB16I
	{GL_RGB16F,         GL_RGB,     		GL_HALF_FLOAT},         // VGL_TEXTURE_FORMAT_RGB16F
	{GL_RGB32UI,        GL_RGB_INTEGER,     GL_UNSIGNED_INT},       // VGL_TEXTURE_FORMAT_RGB32U
	{GL_RGB32I,         GL_RGB_INTEGER,     GL_INT},                // VGL_TEXTURE_FORMAT_RGB32I
	{GL_RGB32F,         GL_RGB,     		GL_FLOAT},              // VGL_TEXTURE_FORMAT_RGB32F
	// rgba
	{GL_RGBA8,          GL_RGBA,    		GL_UNSIGNED_BYTE},      // VGL_TEXTURE_FORMAT_RGBA8U
	{GL_RGBA8_SNORM,    GL_RGBA,    		GL_BYTE},               // VGL_TEXTURE_FORMAT_RGBA8I
	{GL_RGBA16,         GL_RGBA,    		GL_UNSIGNED_SHORT},     // VGL_TEXTURE_FORMAT_RGBA16U
	{GL_RGBA16_SNORM,   GL_RGBA,    		GL_SHORT},              // VGL_TEXTURE_FORMAT_RGBA16I
	{GL_RGBA16F,        GL_RGBA,    		GL_HALF_FLOAT},         // VGL_TEXTURE_FORMAT_RGBA16F
	{GL_RGBA32UI,       GL_RGBA_INTEGER,    GL_UNSIGNED_INT},       // VGL_TEXTURE_FORMAT_RGBA32U
	{GL_RGBA32I,        GL_RGBA_INTEGER,    GL_INT},                // VGL_TEXTURE_FORMAT_RGBA32I
	{GL_RGBA32F,        GL_RGBA,    		GL_FLOAT},              // VGL_TEXTURE_FORMAT_RGBA32F
	// bgr (no sized internal format in GL)
	{GL_FALSE,          GL_BGR,     		GL_UNSIGNED_BYTE},      // VGL_TEXTURE_FORMAT_BGR8U
	{GL_FALSE,          GL_BGR,     		GL_BYTE},               // VGL_TEXTURE_FORMAT_BGR8I
	{GL_FALSE,          GL_BGR,     		GL_UNSIGNED_SHORT},     // VGL_TEXTURE_FORMAT_BGR16U
	{GL_FALSE,          GL_BGR,     		GL_SHORT},              // VGL_TEXTURE_FORMAT_BGR16I
	{GL_FALSE,          GL_BGR,     		GL_HALF_FLOAT},         // VGL_TEXTURE_FORMAT_BGR16F
	{GL_FALSE,          GL_BGR,     		GL_UNSIGNED_INT},       // VGL_TEXTURE_FORMAT_BGR32U
	{GL_FALSE,          GL_BGR,     		GL_INT},                // VGL_TEXTURE_FORMAT_BGR32I
	{GL_FALSE,          GL_BGR,     		GL_FLOAT},              // VGL_TEXTURE_FORMAT_BGR32F
	// bgra (no sized internal format in GL)
	{GL_FALSE,          GL_BGRA,    		GL_UNSIGNED_BYTE},      // VGL_TEXTURE_FORMAT_BGRA8U
	{GL_FALSE,          GL_BGRA,    		GL_BYTE},               // VGL_TEXTURE_FORMAT_BGRA8I
	{GL_FALSE,          GL_BGRA,    		GL_UNSIGNED_SHORT},     // VGL_TEXTURE_FORMAT_BGRA16U
	{GL_FALSE,          GL_BGRA,    		GL_SHORT},              // VGL_TEXTURE_FORMAT_BGRA16I
	{GL_FALSE,          GL_BGRA,    		GL_HALF_FLOAT},         // VGL_TEXTURE_FORMAT_BGRA16F
	{GL_FALSE,          GL_BGRA,    		GL_UNSIGNED_INT},       // VGL_TEXTURE_FORMAT_BGRA32U
	{GL_FALSE,          GL_BGRA,    		GL_INT},                // VGL_TEXTURE_FORMAT_BGRA32I
	{GL_FALSE,          GL_BGRA,    		GL_FLOAT},              // VGL_TEXTURE_FORMAT_BGRA32F
	// depth
	{GL_DEPTH_COMPONENT16,  GL_DEPTH_COMPONENT, GL_FLOAT},  // VGL_TEXTURE_FORMAT_D16F
	{GL_DEPTH_COMPONENT24,  GL_DEPTH_COMPONENT, GL_FLOAT},  // VGL_TEXTURE_FORMAT_D24F
	{GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT},  // VGL_TEXTURE_FORMAT_D32F
	// depth stencil
	{GL_DEPTH24_STENCIL8,   GL_DEPTH_STENCIL,   GL_UNSIGNED_INT_24_8},              // VGL_TEXTURE_FORMAT_D24FS8U
	{GL_DEPTH32F_STENCIL8,  GL_DEPTH_STENCIL,   GL_FLOAT_32_UNSIGNED_INT_24_8_REV}, // VGL_TEXTURE_FORMAT_D32FS8U
	// stencil
	{GL_STENCIL_INDEX8,     GL_STENCIL_INDEX,   GL_UNSIGNED_BYTE},      // VGL_TEXTURE_FORMAT_S8U
	{GL_STENCIL_INDEX16,    GL_STENCIL_INDEX,   GL_UNSIGNED_SHORT},     // VGL_TEXTURE_FORMAT_S16U
};

const GLenum RendererContextGL::s_kTexFilterMap[VGL_TEXTURE_FILTER_COUNT] = {
	GL_NEAREST,             // VGL_TEXTURE_FILTER_NEAREST
	GL_LINEAR,              // VGL_TEXTURE_FILTER_LINEAR
};


const GLenum RendererContextGL::s_kTexWrapMap[VGL_TEXTURE_WRAP_COUNT] = {
	GL_REPEAT,              // VGL_TEXTURE_WRAP_REPEAT
	GL_MIRRORED_REPEAT,     // VGL_TEXTURE_WRAP_MIRRORED_REPEAT
	GL_CLAMP_TO_EDGE,       // VGL_TEXTURE_WRAP_CLAMP_TO_EDGE
	GL_CLAMP_TO_BORDER,     // VGL_TEXTURE_WRAP_CLAMP_TO_BORDER
};

const GLenum RendererContextGL::s_kTexCompareMap[VGL_TEXTURE_COMPARE_COUNT] = {
	GL_NONE,                // VGL_TEXTURE_COMPARE_NONE
	GL_LEQUAL,              // VGL_TEXTURE_COMPARE_LEQUAL
	GL_GEQUAL,              // VGL_TEXTURE_COMPARE_GEQUAL
	GL_LESS,                // VGL_TEXTURE_COMPARE_LESS
	GL_GREATER,             // VGL_TEXTURE_COMPARE_GREATER
	GL_EQUAL,               // VGL_TEXTURE_COMPARE_EQUAL
	GL_NOTEQUAL,            // VGL_TEXTURE_COMPARE_NOTEQUAL
	GL_ALWAYS,              // VGL_TEXTURE_COMPARE_ALWAYS
	GL_NEVER,               // VGL_TEXTURE_COMPARE_NEVER
};

const std::unordered_map<GLenum, uint32_t> RendererContextGL::s_kTexComponentSizeMap = {
	{GL_UNSIGNED_BYTE, 1},
	{GL_BYTE, 1},
	{GL_UNSIGNED_SHORT, 2},
	{GL_SHORT, 2},
	{GL_HALF_FLOAT, 2},
	{GL_UNSIGNED_INT, 4},
	{GL_INT, 4},
	{GL_FLOAT, 4},
	{GL_UNSIGNED_INT_24_8, 3},
	{GL_FLOAT_32_UNSIGNED_INT_24_8_REV, 4}
};

const GLenum RendererContextGL::s_kAttribTypeMap[VGL_ATTRIB_TYPE_COUNT] = {
	GL_BYTE, 				// VGL_ATTRIB_TYPE_INT8
	GL_SHORT, 				// VGL_ATTRIB_TYPE_INT16
	GL_INT, 				// VGL_ATTRIB_TYPE_INT32
	GL_UNSIGNED_BYTE, 		// VGL_ATTRIB_TYPE_UINT8
	GL_UNSIGNED_SHORT, 		// VGL_ATTRIB_TYPE_UINT16
	GL_UNSIGNED_INT, 		// VGL_ATTRIB_TYPE_UINT32
	GL_HALF_FLOAT, 			// VGL_ATTRIB_TYPE_HALF
	GL_FLOAT, 				// VGL_ATTRIB_TYPE_FLOAT
	GL_DOUBLE 				// VGL_ATTRIB_TYPE_DOUBLE
};

const GLenum RendererContextGL::s_kDepthTestMap[VGL_DEPTH_TEST_COUNT] = {
	0, 				// VGL_DEPTH_TEST_DISABLED
	GL_LESS, 		// VGL_DEPTH_TEST_LESS    
	GL_LEQUAL, 		// VGL_DEPTH_TEST_LEQUAL  
	GL_EQUAL, 		// VGL_DEPTH_TEST_EQUAL   
	GL_GEQUAL, 		// VGL_DEPTH_TEST_GEQUAL  
	GL_GREATER, 	// VGL_DEPTH_TEST_GREATER 
	GL_NOTEQUAL, 	// VGL_DEPTH_TEST_NOTEQUAL
	GL_NEVER, 		// VGL_DEPTH_TEST_NEVER   
	GL_ALWAYS, 		// VGL_DEPTH_TEST_ALWAYS  
};

// inverted as backface culling
const GLenum RendererContextGL::s_kCullTestMap[VGL_CULL_TEST_COUNT] = {
	0, 			// VGL_CULL_TEST_DISABLED
	GL_CCW, 	// VGL_CULL_TEST_CW
	GL_CW, 		// VGL_CULL_TEST_CCW
};

const GLenum RendererContextGL::s_kPrimitiveTypeMap[VGL_PT_COUNT] = {
	GL_POINTS,                  	// VGL_PT_POINTS
	GL_LINE_STRIP,              	// VGL_PT_LINE_STRIP
	GL_LINE_LOOP,               	// VGL_PT_LINE_LOOP
	GL_LINES,                   	// VGL_PT_LINES
	GL_LINE_STRIP_ADJACENCY,    	// VGL_PT_LINE_STRIP_ADJACENCY
	GL_LINES_ADJACENCY,         	// VGL_PT_LINES_ADJACENCY
	GL_TRIANGLE_STRIP,          	// VGL_PT_TRIANGLE_STRIP
	GL_TRIANGLE_FAN,            	// VGL_PT_TRIANGLE_FAN
	GL_TRIANGLES,               	// VGL_PT_TRIANGLES
	GL_TRIANGLE_STRIP_ADJACENCY,	// VGL_PT_TRIANGLE_STRIP_ADJACENCY
	GL_TRIANGLES_ADJACENCY      	// VGL_PT_TRIANGLES_ADJACENCY
};

const GLenum RendererContextGL::s_kPolygonMap[VGL_POLYGON_COUNT] = {
	GL_FILL, 	// VGL_POLYGON_FILL
	GL_LINE, 	// VGL_POLYGON_LINE
	GL_POINT,	 // VGL_POLYGON_POINT
};

const std::unordered_map<GLenum, const char*> RendererContextGL::s_kGLErrorStrMap = {
	{GL_NO_ERROR, "GL_NO_ERROR"},
	{GL_INVALID_ENUM, "GL_INVALID_ENUM"},
	{GL_INVALID_VALUE, "GL_INVALID_VALUE"},
	{GL_INVALID_OPERATION, "GL_INVALID_OPERATION"},
	{GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY"},
	{GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION"},
	{GL_CONTEXT_LOST, "GL_CONTEXT_LOST"},
	{GL_STACK_OVERFLOW, "GL_STACK_OVERFLOW"},
	{GL_STACK_UNDERFLOW, "GL_STACK_UNDERFLOW"},
	{GL_TABLE_TOO_LARGE, "GL_TABLE_TOO_LARGE"}
};

RendererContextGL::RendererContextGL() {}

RendererContextGL::~RendererContextGL() {}

RendererType RendererContextGL::getRendererType() const {
	return VGL_RENDERER_OPENGL;
}

const char* RendererContextGL::getRendererName() const {
	return "OpenGL";
}

bool RendererContextGL::init(const Config& _config) {
	// glew to load opengl
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		const char* msg = reinterpret_cast<const char*>(glewGetErrorString(err));
		spdlog::critical("Failed to initialize GLEW: {}", msg);
		return false;
	}
	if (!GLEW_VERSION_3_3) {
		spdlog::critical("OpenGL 3.3 not supported.");
		return false;
	}
	return true;

	glGenVertexArrays(1, &m_vao);
	glCheckAndTrace("glGenVertexArrays", "Generated vertex array ({})", m_vao);
	glBindVertexArray(m_vao);
	glCheckAndTrace("glBindVertexArray", "Bound vertex array ({})", m_vao);
}

void RendererContextGL::shutdown() {
	glBindVertexArray(0);
	glCheckAndTrace("glBindVertexArray", "Unbound VAO");
	glDeleteVertexArrays(1, &m_vao);
	glCheckAndTrace("glDeleteVertexArrays", "Deleted vertex array ({})", m_vao);
	m_vao = 0;

	for (auto& vbo : m_vertexBuffers) {
		if (vbo.id != 0) {
			glDeleteBuffers(1, &vbo.id);
			glCheckAndTrace("glDeleteBuffers", "Deleted vertex buffer ({})", vbo.id);
			vbo = VertexBufferGL{};
		}
	}
	for (auto& ibo : m_indexBuffers) {
		if (ibo.id != 0) {
			glDeleteBuffers(1, &ibo.id);
			glCheckAndTrace("glDeleteBuffers", "Deleted index buffer ({})", ibo.id);
			ibo = IndexBufferGL{};
		}
	}
	for (auto& shader : m_shaders) {
		if (shader.id != 0) {
			glDeleteShader(shader.id);
			glCheckAndTrace("glDeleteShader", "Deleted shader ({})", shader.id);
			shader.id = 0;
		}
		if (shader.source) {
			free(shader.source);
			shader.source = nullptr;
		}
		shader = ShaderGL{};
	}
	for (auto& program : m_programs) {
		if (program.id != 0) {
			glDeleteProgram(program.id);
			glCheckAndTrace("glDeleteProgram", "Deleted program ({})", program.id);
			program = ProgramGL{};
		}
	}
	for (auto& fb : m_frameBuffers) {
		if (fb.fbo != 0) {
			glDeleteFramebuffers(1, &fb.fbo);
			glCheckAndTrace("glDeleteFramebuffers", "Deleted framebuffer ({})", fb.fbo);
			fb = FrameBufferGL{};
		}
	}
	for (auto& texture : m_textures) {
		if (texture.id != 0 && texture.numRT == 0) {
			glDeleteTextures(1, &texture.id);
			glCheckAndTrace("glDeleteTextures", "Deleted texture ({})", texture.id);
			texture = TextureGL{};
		}
	}
}

void RendererContextGL::window(uint16_t _width, uint16_t _height) {
	m_config.resolution = {_width, _height};
}

void RendererContextGL::render(Frame* _frame) {
	/// TODO:
	// - implement vao usage
	// - implement state caching to reduce redundant state changes
	// - implement instanced rendering

	// --- Draw Calls ---
	// For each draw call, set up the pipeline and issue the draw
	ViewID curViewID = UINT16_MAX;
	GLuint curProgram = 0;
	std::bitset<VGL_MAX_VIEWS> clearedViews = std::bitset<VGL_MAX_VIEWS>();

	for (int i = 0; i < _frame->numDrawCalls; i++) {
		DrawCall& draw = _frame->drawCalls[i];
		spdlog::trace("[OpenGL] --- Begin draw call ({}) ---", i);

		// --- Pipeline Setup Order ---
		// 1. Setup view if changed
		if (draw.viewID >= VGL_MAX_VIEWS) {
			spdlog::error("[OpenGL] Draw call {} has invalid view id {}", i, draw.viewID);
			continue;
		} else if (draw.viewID != curViewID) {
			// Apply view state if view changes
			curViewID = draw.viewID;
			View& view = _frame->views[curViewID];
			spdlog::trace("[OpenGL] [View ({})] Open View", curViewID);
			this->applyViewState(view);
			// Clear view if not cleared yet
			if (!clearedViews.test(curViewID)) {
				clearedViews.set(curViewID);
				Clear& clear = view.clear;
				GLbitfield mask = 0;
				if (clear.flags & VGL_CLEAR_COLOUR) {
					float r = ((clear.colour >> 24) & 0xFF) / 255.0f;
					float g = ((clear.colour >> 16) & 0xFF) / 255.0f;
					float b = ((clear.colour >> 8) & 0xFF) / 255.0f;
					float a = (clear.colour & 0xFF) / 255.0f;
					glClearColor(r, g, b, a);
					glCheckAndTrace("glClearColor", "Set clear colour to ({:.2f}, {:.2f}, {:.2f}, {:.2f})", r, g, b, a);
					mask |= GL_COLOR_BUFFER_BIT;
				}
				if (clear.flags & VGL_CLEAR_DEPTH) {
					glClearDepth(clear.depth);
					glCheckAndTrace("glClearDepth", "Set clear depth to {:.2f}", clear.depth);
					mask |= GL_DEPTH_BUFFER_BIT;
				}
				if (clear.flags & VGL_CLEAR_STENCIL) {
					glClearStencil(clear.stencil);
					glCheckAndTrace("glClearStencil", "Set clear stencil to {}", clear.stencil);
					mask |= GL_STENCIL_BUFFER_BIT;
				}
				if (mask != 0) {
					glClear(mask);
					glCheckAndTrace("glClear", "Cleared buffers with mask 0x{:X}", mask);
				}
			}
			spdlog::trace("[OpenGL] [View ({})] Close View", curViewID);
		}

		// 2. Set pipeline state (blend, depth, stencil, etc.)
		// TODO: 
		// - Implement scissor setup from draw.state, draw.frontStencil, draw.backStencil
		// - Implement blending
		// - polygon mode
		// - point size
		// - line width
		glDisable(GL_BLEND);
		glCheckAndTrace("glDisable", "Disable (GL_BLEND)");
		glDisable(GL_STENCIL_TEST);
		glCheckAndTrace("glDisable", "Disable (GL_STENCIL_TEST)");

		StateFlags& state = draw.state;
		DepthTest depthTest = VGL_STATE_DECODE(state, VGL_STATE_DEPTH_TEST_MASK, VGL_STATE_DEPTH_TEST_SHIFT);
		if (depthTest >= VGL_DEPTH_TEST_COUNT) {
			spdlog::warn("[OpenGL] State depth test enum is invalid ({}), max is {}. Using default 'VGL_DEPTH_TEST_LESS'.", depthTest, VGL_DEPTH_TEST_COUNT-1);
			depthTest = VGL_DEPTH_TEST_LESS;
		}
		if (depthTest == 0) {
			glDisable(GL_DEPTH_TEST);
			glCheckAndTrace("glDisable", "Disable depth test, GL_DEPTH_TEST.");
		} else {
			glEnable(GL_DEPTH_TEST);
			glCheckAndTrace("glEnable", "Enable depth test, GL_DEPTH_TEST.");
			GLenum func = s_kDepthTestMap[depthTest];
			glDepthFunc(func);
			glCheckAndTrace("glDepthFunc", "Set depth func (0x{:X})", func);
		}
		
		CullTest cullTest = VGL_STATE_DECODE(state, VGL_STATE_CULL_TEST_MASK, VGL_STATE_CULL_TEST_SHIFT);
		if (cullTest >= VGL_CULL_TEST_COUNT) {
			spdlog::warn("[OpenGL] State cull test enum is invalid ({}), max is {}. Using default 'VGL_CULL_TEST_CCW'.", cullTest, VGL_CULL_TEST_COUNT-1);
			cullTest = VGL_CULL_TEST_CCW;
		}
		if (cullTest == 0) {
			glDisable(GL_CULL_FACE);
			glCheckAndTrace("glDisable", "Disable cull test, GL_CULL_FACE.");
		} else {
			glEnable(GL_CULL_FACE);
			glCheckAndTrace("glEnable", "Enable cull test, GL_CULL_FACE.");
			glCullFace(GL_BACK);
			glCheckAndTrace("glCullFace", "Set cull face to back, GL_BACK.");
			GLenum cullDir = s_kCullTestMap[cullTest];
			glFrontFace(cullDir);
			glCheckAndTrace("glFrontFace", "Set front face (0x{:X})", cullDir);
		}

		PrimitiveType primitiveType = VGL_STATE_DECODE(state, VGL_STATE_PT_MASK, VGL_STATE_PT_SHIFT);
		if (primitiveType >= VGL_PT_COUNT) {
			spdlog::warn("[OpenGL] State primitive type enum is invalid ({}), max is {}. Using default 'VGL_PT_TRIANGLES'.", primitiveType, VGL_PT_COUNT-1);
			primitiveType = VGL_PT_TRIANGLES;
		}
		GLenum primitiveGL = s_kPrimitiveTypeMap[primitiveType];

		if (!isValid(draw.programHandle) || draw.programHandle.idx >= VGL_MAX_PROGRAMS) {
			spdlog::warn("[OpenGL] Invalid program handle {}", draw.programHandle.idx);
			continue;
		}
		ProgramGL& program = m_programs[draw.programHandle.idx];
		if (program.id != curProgram) {
			if (m_programs[draw.programHandle.idx].id == 0) {
				spdlog::error("[OpenGL] Program handle has invalid OpenGL resource 0.");
				continue;
			}
			glUseProgram(program.id);
			glCheckAndTrace("glUseProgram", "Using program id {}", program.id);
			curProgram = program.id;
		}

		for (int j = 0; j < VGL_MAX_TEXTURE_UNITS; j++) {
			if (draw.textureUnitsSet.test(j)) {
				const TextureUnit& texUnit = draw.textureUnits[j];
				if (isValid(texUnit.handle)) {
					TextureGL& tex = m_textures[texUnit.handle.idx];
					glActiveTexture(GL_TEXTURE0 + j);
					glCheckAndTrace("glActiveTexture", "Activated texture unit {}", j);
					glBindTexture(tex.target, tex.id);
					glCheckAndTrace("glBindTexture", "Bound texture id {} to target 0x{:X} on unit {}", tex.id, tex.target, j);
					GLuint loc = glGetUniformLocation(curProgram, texUnit.name);
					glCheckAndTrace("glGetUniformLocation", "Got location ({}) of sampler uniform '{}'", loc, texUnit.name);
					glUniform1i(loc, j);
					glCheckAndTrace("glUniform1i", "Bound texture unit {} to sampler '{}'", j, texUnit.name);
				}
			}
		}

		// Uniforms: Replace all spdlog::trace and gl* calls with glCheckAndTrace as appropriate.
		// For brevity, only show a single example, but you should apply this pattern to all uniform cases.
		for (int j = 0; j < draw.numUniforms; j++) {
			const Uniform& uniform = draw.uniforms[j];
			GLuint loc = glGetUniformLocation(curProgram, uniform.name);
			glCheckAndTrace("glGetUniformLocation", "Got location ({}) of uniform '{}'", loc, uniform.name);
			switch (uniform.type) {
				// scalars
				case VGL_UNIFORM_TYPE_INT32: {
					glUniform1iv(loc, uniform.numElements,
						reinterpret_cast<const GLint*>(uniform.pData));
					glCheckAndTrace("glUniform1iv", "({}) Set INT32 uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					glCheckAndTrace("glUniform1iv", "({}) data = {}", loc, reinterpret_cast<const GLint*>(uniform.pData)[0]);
				} break;
				case VGL_UNIFORM_TYPE_UINT32: {
					glUniform1uiv(loc, uniform.numElements,
						reinterpret_cast<const GLuint*>(uniform.pData));
					glCheckAndTrace("glUniform1uiv", "({}) Set UINT32 uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					glCheckAndTrace("glUniform1uiv", "({}) data = {}", loc, reinterpret_cast<const GLuint*>(uniform.pData)[0]);
				} break;
				case VGL_UNIFORM_TYPE_FLOAT: {
					glUniform1fv(loc, uniform.numElements,
						reinterpret_cast<const GLfloat*>(uniform.pData));
					glCheckAndTrace("glUniform1fv", "({}) Set FLOAT uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					glCheckAndTrace("glUniform1fv", "({}) data = {:.3f}", loc, reinterpret_cast<const GLfloat*>(uniform.pData)[0]);
				} break;
				case VGL_UNIFORM_TYPE_DOUBLE: {
					glUniform1dv(loc, uniform.numElements,
						reinterpret_cast<const GLdouble*>(uniform.pData));
					glCheckAndTrace("glUniform1dv", "({}) Set DOUBLE uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					glCheckAndTrace("glUniform1dv", "({}) data = {:.3f}", loc, reinterpret_cast<const GLdouble*>(uniform.pData)[0]);
				} break;

				// vectors
				case VGL_UNIFORM_TYPE_IVEC2: {
					glUniform2iv(loc, uniform.numElements,
						reinterpret_cast<const GLint*>(uniform.pData));
					glCheckAndTrace("glUniform2iv", "({}) Set IVEC2 uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					const GLint* v = reinterpret_cast<const GLint*>(uniform.pData);
					glCheckAndTrace("glUniform2iv", "({}) data = {}, {}", loc, v[0], v[1]);
				} break;
				case VGL_UNIFORM_TYPE_IVEC3: {
					glUniform3iv(loc, uniform.numElements,
						reinterpret_cast<const GLint*>(uniform.pData));
					glCheckAndTrace("glUniform3iv", "({}) Set IVEC3 uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					const GLint* v = reinterpret_cast<const GLint*>(uniform.pData);
					glCheckAndTrace("glUniform3iv", "({}) data = {}, {}, {}", loc, v[0], v[1], v[2]);
				} break;
				case VGL_UNIFORM_TYPE_IVEC4: {
					glUniform4iv(loc, uniform.numElements,
						reinterpret_cast<const GLint*>(uniform.pData));
					glCheckAndTrace("glUniform4iv", "({}) Set IVEC4 uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					const GLint* v = reinterpret_cast<const GLint*>(uniform.pData);
					glCheckAndTrace("glUniform4iv", "({}) data = {}, {}, {}, {}", loc, v[0], v[1], v[2], v[3]);
				} break;

				case VGL_UNIFORM_TYPE_UVEC2: {
					glUniform2uiv(loc, uniform.numElements,
						reinterpret_cast<const GLuint*>(uniform.pData));
					glCheckAndTrace("glUniform2uiv", "({}) Set UVEC2 uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					const GLuint* v = reinterpret_cast<const GLuint*>(uniform.pData);
					glCheckAndTrace("glUniform2uiv", "({}) data = {}, {}", loc, v[0], v[1]);
				} break;
				case VGL_UNIFORM_TYPE_UVEC3: {
					glUniform3uiv(loc, uniform.numElements,
						reinterpret_cast<const GLuint*>(uniform.pData));
					glCheckAndTrace("glUniform3uiv", "({}) Set UVEC3 uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					const GLuint* v = reinterpret_cast<const GLuint*>(uniform.pData);
					glCheckAndTrace("glUniform3uiv", "({}) data = {}, {}, {}", loc, v[0], v[1], v[2]);
				} break;
				case VGL_UNIFORM_TYPE_UVEC4: {
					glUniform4uiv(loc, uniform.numElements,
						reinterpret_cast<const GLuint*>(uniform.pData));
					glCheckAndTrace("glUniform4uiv", "({}) Set UVEC4 uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					const GLuint* v = reinterpret_cast<const GLuint*>(uniform.pData);
					glCheckAndTrace("glUniform4uiv", "({}) data = {}, {}, {}, {}", loc, v[0], v[1], v[2], v[3]);
				} break;

				case VGL_UNIFORM_TYPE_VEC2: {
					glUniform2fv(loc, uniform.numElements,
						reinterpret_cast<const GLfloat*>(uniform.pData));
					glCheckAndTrace("glUniform2fv", "({}) Set VEC2 uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					const GLfloat* v = reinterpret_cast<const GLfloat*>(uniform.pData);
					glCheckAndTrace("glUniform2fv", "({}) data = {:.3f}, {:.3f}", loc, v[0], v[1]);
				} break;
				case VGL_UNIFORM_TYPE_VEC3: {
					glUniform3fv(loc, uniform.numElements,
						reinterpret_cast<const GLfloat*>(uniform.pData));
					glCheckAndTrace("glUniform3fv", "({}) Set VEC3 uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					const GLfloat* v = reinterpret_cast<const GLfloat*>(uniform.pData);
					glCheckAndTrace("glUniform3fv", "({}) data = {:.3f}, {:.3f}, {:.3f}", loc, v[0], v[1], v[2]);
				} break;
				case VGL_UNIFORM_TYPE_VEC4: {
					glUniform4fv(loc, uniform.numElements,
						reinterpret_cast<const GLfloat*>(uniform.pData));
					glCheckAndTrace("glUniform4fv", "({}) Set VEC4 uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					const GLfloat* v = reinterpret_cast<const GLfloat*>(uniform.pData);
					glCheckAndTrace("glUniform4fv", "({}) data = {:.3f}, {:.3f}, {:.3f}, {:.3f}", loc, v[0], v[1], v[2], v[3]);
				} break;

				case VGL_UNIFORM_TYPE_DVEC2: {
					glUniform2dv(loc, uniform.numElements,
						reinterpret_cast<const GLdouble*>(uniform.pData));
					glCheckAndTrace("glUniform2dv", "({}) Set DVEC2 uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					const GLdouble* v = reinterpret_cast<const GLdouble*>(uniform.pData);
					glCheckAndTrace("glUniform2dv", "({}) data = {:.3f}, {:.3f}", loc, v[0], v[1]);
				} break;
				case VGL_UNIFORM_TYPE_DVEC3: {
					glUniform3dv(loc, uniform.numElements,
						reinterpret_cast<const GLdouble*>(uniform.pData));
					glCheckAndTrace("glUniform3dv", "({}) Set DVEC3 uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					const GLdouble* v = reinterpret_cast<const GLdouble*>(uniform.pData);
					glCheckAndTrace("glUniform3dv", "({}) data = {:.3f}, {:.3f}, {:.3f}", loc, v[0], v[1], v[2]);
				} break;
				case VGL_UNIFORM_TYPE_DVEC4: {
					glUniform4dv(loc, uniform.numElements,
						reinterpret_cast<const GLdouble*>(uniform.pData));
					glCheckAndTrace("glUniform4dv", "({}) Set DVEC4 uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					const GLdouble* v = reinterpret_cast<const GLdouble*>(uniform.pData);
					glCheckAndTrace("glUniform4dv", "({}) data = {:.3f}, {:.3f}, {:.3f}, {:.3f}", loc, v[0], v[1], v[2], v[3]);
				} break;

				// matrices
				case VGL_UNIFORM_TYPE_MAT2: {
					glUniformMatrix2fv(loc, uniform.numElements,
						uniform.transpose,
						reinterpret_cast<const GLfloat*>(uniform.pData));
					glCheckAndTrace("glUniformMatrix2fv", "({}) Set MAT2 uniform '{}' with {} elements, transpose={}, data =", loc, uniform.name, uniform.numElements, uniform.transpose);
					const GLfloat* m = reinterpret_cast<const GLfloat*>(uniform.pData);
					glCheckAndTrace("glUniformMatrix2fv", "({}) row (0):\t [{:.3f}, {:.3f}]", loc, m[0], m[1]);
					glCheckAndTrace("glUniformMatrix2fv", "({}) row (1):\t [{:.3f}, {:.3f}]", loc, m[2], m[3]);
				} break;
				case VGL_UNIFORM_TYPE_MAT2X3: {
					glUniformMatrix2x3fv(loc, uniform.numElements,
						uniform.transpose,
						reinterpret_cast<const GLfloat*>(uniform.pData));
					glCheckAndTrace("glUniformMatrix2x3fv", "({}) Set MAT2X3 uniform '{}' with {} elements, transpose={}, data =", loc, uniform.name, uniform.numElements, uniform.transpose);
					const GLfloat* m = reinterpret_cast<const GLfloat*>(uniform.pData);
					glCheckAndTrace("glUniformMatrix2x3fv", "({}) row (0):\t [{:.3f}, {:.3f}, {:.3f}]", loc, m[0], m[1], m[2]);
					glCheckAndTrace("glUniformMatrix2x3fv", "({}) row (1):\t [{:.3f}, {:.3f}, {:.3f}]", loc, m[3], m[4], m[5]);
				} break;
				case VGL_UNIFORM_TYPE_MAT2X4: {
					glUniformMatrix2x4fv(loc, uniform.numElements,
						uniform.transpose,
						reinterpret_cast<const GLfloat*>(uniform.pData));
					glCheckAndTrace("glUniformMatrix2x4fv", "({}) Set MAT2X4 uniform '{}' with {} elements, transpose={}, data =", loc, uniform.name, uniform.numElements, uniform.transpose);
					const GLfloat* m = reinterpret_cast<const GLfloat*>(uniform.pData);
					glCheckAndTrace("glUniformMatrix2x4fv", "({}) row (0):\t [{:.3f}, {:.3f}, {:.3f}, {:.3f}]", loc, m[0], m[1], m[2], m[3]);
					glCheckAndTrace("glUniformMatrix2x4fv", "({}) row (1):\t [{:.3f}, {:.3f}, {:.3f}, {:.3f}]", loc, m[4], m[5], m[6], m[7]);
				} break;
				case VGL_UNIFORM_TYPE_MAT3X2: {
					glUniformMatrix3x2fv(loc, uniform.numElements,
						uniform.transpose,
						reinterpret_cast<const GLfloat*>(uniform.pData));
					glCheckAndTrace("glUniformMatrix3x2fv", "({}) Set MAT3X2 uniform '{}' with {} elements, transpose={}, data =", loc, uniform.name, uniform.numElements, uniform.transpose);
					const GLfloat* m = reinterpret_cast<const GLfloat*>(uniform.pData);
					glCheckAndTrace("glUniformMatrix3x2fv", "({}) row (0):\t [{:.3f}, {:.3f}]", loc, m[0], m[1]);
					glCheckAndTrace("glUniformMatrix3x2fv", "({}) row (1):\t [{:.3f}, {:.3f}]", loc, m[2], m[3]);
					glCheckAndTrace("glUniformMatrix3x2fv", "({}) row (2):\t [{:.3f}, {:.3f}]", loc, m[4], m[5]);
				} break;
				case VGL_UNIFORM_TYPE_MAT3: {
					glUniformMatrix3fv(loc, uniform.numElements,
						uniform.transpose,
						reinterpret_cast<const GLfloat*>(uniform.pData));
					glCheckAndTrace("glUniformMatrix3fv", "({}) Set MAT3 uniform '{}' with {} elements, transpose={}, data =", loc, uniform.name, uniform.numElements, uniform.transpose);
					const GLfloat* m = reinterpret_cast<const GLfloat*>(uniform.pData);
					glCheckAndTrace("glUniformMatrix3fv", "({}) row (0):\t [{:.3f}, {:.3f}, {:.3f}]", loc, m[0], m[1], m[2]);
					glCheckAndTrace("glUniformMatrix3fv", "({}) row (1):\t [{:.3f}, {:.3f}, {:.3f}]", loc, m[3], m[4], m[5]);
					glCheckAndTrace("glUniformMatrix3fv", "({}) row (2):\t [{:.3f}, {:.3f}, {:.3f}]", loc, m[6], m[7], m[8]);
				} break;
				case VGL_UNIFORM_TYPE_MAT3X4: {
					glUniformMatrix3x4fv(loc, uniform.numElements,
						uniform.transpose,
						reinterpret_cast<const GLfloat*>(uniform.pData));
					glCheckAndTrace("glUniformMatrix3x4fv", "({}) Set MAT3X4 uniform '{}' with {} elements, transpose={}, data =", loc, uniform.name, uniform.numElements, uniform.transpose);
					const GLfloat* m = reinterpret_cast<const GLfloat*>(uniform.pData);
					glCheckAndTrace("glUniformMatrix3x4fv", "({}) row (0):\t [{:.3f}, {:.3f}, {:.3f}, {:.3f}]", loc, m[0], m[1], m[2], m[3]);
					glCheckAndTrace("glUniformMatrix3x4fv", "({}) row (1):\t [{:.3f}, {:.3f}, {:.3f}, {:.3f}]", loc, m[4], m[5], m[6], m[7]);
					glCheckAndTrace("glUniformMatrix3x4fv", "({}) row (2):\t [{:.3f}, {:.3f}, {:.3f}, {:.3f}]", loc, m[8], m[9], m[10], m[11]);
				} break;
				case VGL_UNIFORM_TYPE_MAT4X2: {
					glUniformMatrix4x2fv(loc, uniform.numElements,
						uniform.transpose,
						reinterpret_cast<const GLfloat*>(uniform.pData));
					glCheckAndTrace("glUniformMatrix4x2fv", "({}) Set MAT4X2 uniform '{}' with {} elements, transpose={}, data =", loc, uniform.name, uniform.numElements, uniform.transpose);
					const GLfloat* m = reinterpret_cast<const GLfloat*>(uniform.pData);
					glCheckAndTrace("glUniformMatrix4x2fv", "({}) row (0):\t [{:.3f}, {:.3f}]", loc, m[0], m[1]);
					glCheckAndTrace("glUniformMatrix4x2fv", "({}) row (1):\t [{:.3f}, {:.3f}]", loc, m[2], m[3]);
					glCheckAndTrace("glUniformMatrix4x2fv", "({}) row (2):\t [{:.3f}, {:.3f}]", loc, m[4], m[5]);
					glCheckAndTrace("glUniformMatrix4x2fv", "({}) row (3):\t [{:.3f}, {:.3f}]", loc, m[6], m[7]);
				} break;
				case VGL_UNIFORM_TYPE_MAT4X3: {
					glUniformMatrix4x3fv(loc, uniform.numElements,
						uniform.transpose,
						reinterpret_cast<const GLfloat*>(uniform.pData));
					glCheckAndTrace("glUniformMatrix4x3fv", "({}) Set MAT4X3 uniform '{}' with {} elements, transpose={}, data=", loc, uniform.name, uniform.numElements, uniform.transpose);
					const GLfloat* m = reinterpret_cast<const GLfloat*>(uniform.pData);
					glCheckAndTrace("glUniformMatrix4x3fv", "({}) row (0):\t [{:.3f}, {:.3f}, {:.3f}]", loc, m[0], m[1], m[2]);
					glCheckAndTrace("glUniformMatrix4x3fv", "({}) row (1):\t [{:.3f}, {:.3f}, {:.3f}]", loc, m[3], m[4], m[5]);
					glCheckAndTrace("glUniformMatrix4x3fv", "({}) row (2):\t [{:.3f}, {:.3f}, {:.3f}]", loc, m[6], m[7], m[8]);
					glCheckAndTrace("glUniformMatrix4x3fv", "({}) row (3):\t [{:.3f}, {:.3f}, {:.3f}]", loc, m[9], m[10], m[11]);
				} break;
				case VGL_UNIFORM_TYPE_MAT4: {
					glUniformMatrix4fv(loc, uniform.numElements,
						uniform.transpose,
						reinterpret_cast<const GLfloat*>(uniform.pData));
					glCheckAndTrace("glUniformMatrix4fv", "({}) Set MAT4 uniform '{}' with {} elements, transpose={}, data=", loc, uniform.name, uniform.numElements, uniform.transpose);
					const GLfloat* m = reinterpret_cast<const float*>(uniform.pData);
					glCheckAndTrace("glUniformMatrix4fv", "({}) row (0):\t [{:.3f}, {:.3f}, {:.3f}, {:.3f}]", loc, m[0], m[1], m[2], m[3]);
					glCheckAndTrace("glUniformMatrix4fv", "({}) row (1):\t [{:.3f}, {:.3f}, {:.3f}, {:.3f}]", loc, m[4], m[5], m[6], m[7]);
					glCheckAndTrace("glUniformMatrix4fv", "({}) row (2):\t [{:.3f}, {:.3f}, {:.3f}, {:.3f}]", loc, m[8], m[9], m[10], m[11]);
					glCheckAndTrace("glUniformMatrix4fv", "({}) row (3):\t [{:.3f}, {:.3f}, {:.3f}, {:.3f}]", loc, m[12], m[13], m[14], m[15]);
				} break;

				// colour textures
				case VGL_UNIFORM_TYPE_SAMPLER_1D:
				case VGL_UNIFORM_TYPE_SAMPLER_2D:
				case VGL_UNIFORM_TYPE_SAMPLER_3D:
				case VGL_UNIFORM_TYPE_SAMPLER_CUBE:
				case VGL_UNIFORM_TYPE_SAMPLER_2D_RECT:
				case VGL_UNIFORM_TYPE_SAMPLER_1D_ARRAY:
				case VGL_UNIFORM_TYPE_SAMPLER_2D_ARRAY:
				case VGL_UNIFORM_TYPE_SAMPLER_CUBE_ARRAY:
				case VGL_UNIFORM_TYPE_SAMPLER_BUFFER: {
					glUniform1iv(loc, uniform.numElements,
						reinterpret_cast<const GLint*>(uniform.pData));
					glCheckAndTrace("glUniform1iv", "({}) Set SAMPLER uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					glCheckAndTrace("glUniform1iv", "({}) data = {}", loc, reinterpret_cast<const GLint*>(uniform.pData)[0]);
				} break;

				// shadow textures
				case VGL_UNIFORM_TYPE_SAMPLER_1D_SHADOW:
				case VGL_UNIFORM_TYPE_SAMPLER_2D_SHADOW:
				case VGL_UNIFORM_TYPE_SAMPLER_CUBE_SHADOW:
				case VGL_UNIFORM_TYPE_SAMPLER_2D_RECT_SHADOW:
				case VGL_UNIFORM_TYPE_SAMPLER_1D_ARRAY_SHADOW:
				case VGL_UNIFORM_TYPE_SAMPLER_2D_ARRAY_SHADOW:
				case VGL_UNIFORM_TYPE_SAMPLER_CUBE_ARRAY_SHADOW: {
					glUniform1iv(loc, uniform.numElements,
						reinterpret_cast<const GLint*>(uniform.pData));
					glCheckAndTrace("glUniform1iv", "({}) Set SHADOW SAMPLER uniform '{}' with {} elements", loc, uniform.name, uniform.numElements);
					glCheckAndTrace("glUniform1iv", "({}) data = {}", loc, reinterpret_cast<const GLint*>(uniform.pData)[0]);
				} break;
				default: {
					spdlog::warn("[OpenGL] ({}) Uniform ({}) has unknown type ({}).",
						loc, uniform.name, static_cast<uint32_t>(uniform.type));
				} break;
			}
		}

		// 6. Bind vertex buffers and layouts
		/// TODO:
		// - vao creation & caching
		GLuint maxAttrib = 0;
		for (int j = 0; j < VGL_MAX_VERTEX_STREAMS; j++) {
			if (draw.vertexStreamsSet.test(j)) {
				const VertexStream& vstream = draw.vertexStreams[j];
				if (!isValid(vstream.vertexBufferHandle)) {
					spdlog::error("[OpenGL] [Draw ({})] has invalid vertex buffer handle.", i);
					continue;
				}
				VertexBufferGL& vbo = m_vertexBuffers[vstream.vertexBufferHandle.idx];
				VertexLayout& layout = m_vertexLayouts[vbo.layoutHandle.idx];
				if (!isValid(vbo.layoutHandle)) {
					spdlog::error("[OpenGL] Using vertex buffer {} with invalid vertex layout handle {}.", vstream.vertexBufferHandle.idx, vbo.layoutHandle.idx);
					continue;
				}
				glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
				glCheckAndTrace("glBindBuffer", "Bound vertex buffer {} to GL_ARRAY_BUFFER", vbo.id);
				// setup vertex attributes
				uint8_t numAttribs = 0;
				const VertexAttribute* vertAttribs = layout.getAttributes(numAttribs);
				GLsizei stride = layout.getStride();
				for (uint8_t k = 0; k < numAttribs; k++) {
					const VertexAttribute& attrib = vertAttribs[k];
					GLuint index = attrib.attrib;
					maxAttrib = std::max(maxAttrib, index);
					if (attrib.numComponents == 0 || attrib.numComponents > 4) {
						spdlog::error("[OpenGL] Vertex attribute ({}) has invalid number of components ({}).", attrib.attrib, attrib.numComponents);
						continue;
					}
					GLint numComponents = attrib.numComponents;
					if (attrib.type >= VGL_ATTRIB_TYPE_COUNT) {
						spdlog::error("[OpenGL] Vertex attribute ({}) has unknown type ({}).", attrib.attrib, static_cast<uint32_t>(attrib.type));
						continue;
					}
					GLenum type = s_kAttribTypeMap[attrib.type];
					const void* pointer = (void*)(attrib.offset);
					glEnableVertexAttribArray(attrib.attrib);
					glCheckAndTrace("glEnableVertexAttribArray", "Enabled vertex attribute array ({})", attrib.attrib);
					switch (attrib.type) {
						case VGL_ATTRIB_TYPE_INT8:
						case VGL_ATTRIB_TYPE_INT16:
						case VGL_ATTRIB_TYPE_INT32:
						case VGL_ATTRIB_TYPE_UINT8:
						case VGL_ATTRIB_TYPE_UINT16:
						case VGL_ATTRIB_TYPE_UINT32: {
							GLboolean normalised = GL_FALSE; // integer types are never normalised
							glVertexAttribIPointer(
								index,
								numComponents,
								type,
								stride,
								pointer
							);
							glCheckAndTrace("glVertexAttribIPointer", "Set integer vertex attribute pointer for attribute ({})", attrib.attrib);
						} break;
						case VGL_ATTRIB_TYPE_HALF:
						case VGL_ATTRIB_TYPE_FLOAT:
						case VGL_ATTRIB_TYPE_DOUBLE: {
							GLboolean normalised = (attrib.normalised) ? GL_TRUE : GL_FALSE;
							glVertexAttribPointer(
								index,
								numComponents,
								type,
								normalised,
								stride,
								pointer
							);
							glCheckAndTrace("glVertexAttribPointer", "Set float vertex attribute pointer for attribute ({})", attrib.attrib);
						} break;
						default: {
							spdlog::error("[OpenGL] A big bad wolf.", i, attrib.attrib, static_cast<uint32_t>(attrib.type));
						} break;
					}
				}
			}
		}

		// 7. Issue draw call, bind index buffer if needed
			if (isValid(draw.indexStream.indexBufferHandle)) {
			IndexBufferGL& ibo = m_indexBuffers[draw.indexStream.indexBufferHandle.idx];
			if (ibo.id == 0) {
				spdlog::error("[OpenGL] [Draw ({})] has invalid OpenGL index buffer resource handle (0).", i);
				continue;
			}
			GLenum type = (ibo.flags & VGL_BUFFER_INDEX32) ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT; 
			GLuint indexSize = (type == GL_UNSIGNED_INT) ? sizeof(uint32_t) : sizeof(uint16_t);
			GLuint startIndex = draw.indexStream.startIndex;
			GLuint numIndices = draw.indexStream.numIndices;
			if (startIndex == 0 && numIndices == 0) {
				startIndex = 0;
				numIndices = ibo.size / indexSize;
				spdlog::trace("[OpenGL] Draw range & offset 0. Defaulting to use whole index buffer range ({}, {})", startIndex, numIndices);
			} else if (startIndex + numIndices > (ibo.size / indexSize)) {
				spdlog::error("[OpenGL] Draw has out-of-bounds index range (start: {}, count: {}) for index buffer of size {}.", startIndex, numIndices, ibo.size);
				continue;
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.id);
			glCheckAndTrace("glBindBuffer", "Bound index buffer ({}) to GL_ELEMENT_ARRAY_BUFFER", ibo.id);
			// issue draw call
			void* offset = (void*)(startIndex * indexSize);
			glDrawElements(primitiveGL, numIndices, type, offset);
			glCheckAndTrace("glDrawElements", "Issued indexed draw call of 0x{:X} with {} indices starting from index {}", primitiveGL, numIndices, startIndex);
		} else {
			glDrawArrays(primitiveGL, draw.vertexStreams[0].startVertex, draw.vertexStreams[0].numVertices);
			glCheckAndTrace("glDrawArrays", "Issued non-indexed draw call of 0x{:X} with {} vertices starting from vertex {}", primitiveGL, draw.vertexStreams[0].numVertices, draw.vertexStreams[0].startVertex);
		}

		// 8. Unbind resources if needed (optional, for safety)
		/// TODO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glCheckAndTrace("glBindBuffer", "Unbound GL_ARRAY_BUFFER");
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glCheckAndTrace("glBindBuffer", "Unbound GL_ELEMENT_ARRAY_BUFFER");
		for (int j = 0; j <= maxAttrib; j++) {
			glDisableVertexAttribArray(j);
			glCheckAndTrace("glDisableVertexAttribArray", "Disabled vertex attribute array ({})", j);
		}

		// 9. Debug info
		spdlog::trace("[OpenGL] --- End draw call ({}) ---", i);
	}
}


const char* RendererContextGL::glErrorToString(GLenum _err) {
	auto it = s_kGLErrorStrMap.find(_err);
	if (it != s_kGLErrorStrMap.end()) {
		return it->second;
	}
	return "UNKNOWN ERROR";
}

bool RendererContextGL::glCheckError(const char* _label) {
	GLenum err = glGetError();
	if (err == GL_NO_ERROR) {
		return true;
	}
	spdlog::error("[OpenGL] {}: {}", _label, glErrorToString(err));
	return false;
}

void RendererContextGL::glCheckErrors(const char* _label) {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		spdlog::error("[OpenGL] {}: {}", _label, glErrorToString(err));
	}
}

// views
void RendererContextGL::applyViewState(View& _view) {
	// bind frame buffer
	if (isValid(_view.fbh)) {
		FrameBufferGL& fbo = m_frameBuffers[_view.fbh.idx];
		glBindFramebuffer(GL_FRAMEBUFFER, fbo.fbo);
		glCheckAndTrace("glBindFramebuffer",  "Bound framebuffer ({})", fbo.fbo);
	} else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glCheckAndTrace("glBindFramebuffer", "Bound default framebuffer (0)");
	}
	
	// viewport rect
	Rect& rect = _view.rect;
	if (rect.isZero()) {
		Resolution& res = m_config.resolution;
		glViewport(0, 0, res.width, res.height);
		glCheckAndTrace("glViewport", "Set viewport to window resolution (0, 0, {}, {})", res.width, res.height);
	} else if (rect.isZeroArea()) {
		// skip
	} else {
		glViewport(rect.x, rect.y, rect.width, rect.height);
		glCheckAndTrace("glViewport", "Set viewport to rect ({}, {}, {}, {})", rect.x, rect.y, rect.width, rect.height);
	}
	
	/// TODO:
	// - scissor view
	// - set scissor test
	glDisable(GL_SCISSOR_TEST);
	glCheckAndTrace("glDisable", "Disabled scissor test, GL_SCISSOR_TEST");

	// // scissor
	// Rect& scissor = view.scissor;
	// if (scissor.isZero()) {
	// 	glScissor(0, 0, m_config.width, m_config.height);
	// } else if (scissor.isZeroArea()) {
	// 	// skip
	// } else {
	// 	glScissor(scissor.x, scissor.y, scissor.width, scissor.height);
	// }
}

// layouts
void RendererContextGL::createVertexLayout(VertexLayoutHandle _handle, const VertexLayout& _layout) {
	m_vertexLayouts[_handle.idx] = _layout;
}

void RendererContextGL::destroyVertexLayout(VertexLayoutHandle _handle) {
	m_vertexLayouts[_handle.idx] = {};
}

// buffers
void RendererContextGL::createVertexBuffer(VertexBufferHandle _handle, uint32_t _size, VertexLayoutHandle _layoutHandle, void* _pData, BufferFlags _flags) {
	VertexBufferGL& vbo = m_vertexBuffers[_handle.idx];
	if (vbo.id != 0) {
		spdlog::warn("Vertex buffer handle {} already in use. Overwrite existing.", _handle.idx);
		glDeleteBuffers(1, &vbo.id);
		glCheckAndTrace("glDeleteBuffers", "Delete vertex buffer {}", vbo.id);
		vbo = VertexBufferGL{};
	}
	if (_size == 0 || _pData == nullptr) {
		spdlog::error("Cannot create vertex buffer with size 0 or null data.");
		return;
	}
	glGenBuffers(1, &vbo.id);
	glCheckAndTrace("glGenBuffers", "Generate vertex buffer {}", vbo.id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
	glCheckAndTrace("glBindBuffer", "Bind vertex buffer {} to GL_ARRAY_BUFFER", vbo.id);
	glBufferData(GL_ARRAY_BUFFER, _size, _pData, GL_STATIC_DRAW);
	glCheckAndTrace("glBufferData", "Set bound GL_ARRAY_BUFFER data of size {} as GL_STATIC_DRAW (0x{:p})", _size, _pData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glCheckAndTrace("glBindBuffer", "Bind default vertex buffer (0) to GL_ARRAY_BUFFER");
	spdlog::info("Created vertex buffer handle {} with size {}.", _handle.idx, _size);
	vbo.size = _size;
	vbo.target = GL_ARRAY_BUFFER;
	vbo.layoutHandle = _layoutHandle;
	vbo.flags = _flags;    
	vbo.isDynamic = false;
}

void RendererContextGL::destroyVertexBuffer(VertexBufferHandle _handle) {
	VertexBufferGL& vbo = m_vertexBuffers[_handle.idx];
	// delete opengl resource if exists
	if (vbo.id == 0) {
		spdlog::warn("Skip destroying invalid vertex buffer, vertex handle {}", _handle.idx);
	} else {
		glDeleteBuffers(1, &vbo.id);
		glCheckAndTrace("glDeleteBuffers", "Delete vertex buffer {}", vbo.id);
	}
	vbo = VertexBufferGL{};
	spdlog::info("Destroyed vertex buffer handle {}.", _handle.idx);
}

void RendererContextGL::createIndexBuffer(IndexBufferHandle _handle, uint32_t _size, void* _pData, BufferFlags _flags) {
	IndexBufferGL& ibo = m_indexBuffers[_handle.idx];
	// clear if in use
	if (ibo.id != 0) {
		spdlog::warn("Index buffer handle {} already in use. Overwrite existing.", _handle.idx);
		glDeleteBuffers(1, &ibo.id);
		glCheckAndTrace("glDeleteBuffers", "Delete index buffer {}", ibo.id);
		ibo = IndexBufferGL{};
	}
	// data checks
	if (_size == 0 || _pData == nullptr) {
		spdlog::error("Cannot create index buffer with size 0 or null data.");
		return;
	}
	// create buffer
	glGenBuffers(1, &ibo.id);
	glCheckAndTrace("glGenBuffers", "Generate index buffer {}", ibo.id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.id);
	glCheckAndTrace("glBindBuffer", "Bind index buffer {} to GL_ELEMENT_ARRAY_BUFFER", ibo.id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, _pData, GL_STATIC_DRAW);
	glCheckAndTrace("glBufferData", "Set bound GL_ELEMENT_ARRAY_BUFFER data of size {} as GL_STATIC_DRAW (0x{:p})", _size, _pData);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glCheckAndTrace("glBindBuffer", "Bind default index buffer (0) to GL_ELEMENT_ARRAY_BUFFER");
	spdlog::info("Created index buffer handle {} with size {}.", _handle.idx, _size);
	// write to repr
	ibo.size = _size;
	ibo.flags = _flags;    
	ibo.isDynamic = false;
}

void RendererContextGL::destroyIndexBuffer(IndexBufferHandle _handle) {
	IndexBufferGL& ibo = m_indexBuffers[_handle.idx];
	// delete opengl resource if exists
	if (ibo.id == 0) {
		spdlog::warn("Skip destroying invalid index buffer, index handle {}", _handle.idx);
	} else {
		glDeleteBuffers(1, &ibo.id);
		glCheckAndTrace("glDeleteBuffers", "Delete index buffer {}", ibo.id);
	}
	ibo = IndexBufferGL{};
	spdlog::info("Destroyed index buffer handle {}.", _handle.idx);
}

// dynamic buffers
void RendererContextGL::createDynamicVertexBuffer(VertexBufferHandle _handle, uint32_t _size, VertexLayoutHandle _layoutHandle, BufferFlags _flags) {
	VertexBufferGL& vbo = m_vertexBuffers[_handle.idx];
	// clear if in use
	if (vbo.id != 0) {
		spdlog::warn("Vertex buffer handle {} already in use. Overwrite existing.", _handle.idx);
		glDeleteBuffers(1, &vbo.id);
		glCheckAndTrace("glDeleteBuffers", "Delete dynamic vertex buffer {}", vbo.id);
		vbo = VertexBufferGL{};
	}
	// data checks
	if (_size == 0) {
		spdlog::error("Cannot create vertex buffer with size 0.");
		return;
	}
	// create buffer
	GLuint id;
	glGenBuffers(1, &id);
	glCheckAndTrace("glGenBuffers", "Generate dynamic vertex buffer {}", id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glCheckAndTrace("glBindBuffer", "Bind dynamic vertex buffer {} to GL_ARRAY_BUFFER", id);
	glBufferData(GL_ARRAY_BUFFER, _size, nullptr, GL_DYNAMIC_DRAW);
	glCheckAndTrace("glBufferData", "Set bound GL_ARRAY_BUFFER data of size {} as GL_DYNAMIC_DRAW (nullptr)", _size);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glCheckAndTrace("glBindBuffer", "Bind default vertex buffer (0) to GL_ARRAY_BUFFER");
	spdlog::info("Created dynamic vertex buffer handle {} with size {}.", _handle.idx, _size);
	// write to repr
	vbo.id = id;
	vbo.size = _size;
	vbo.target = GL_ARRAY_BUFFER;
	vbo.layoutHandle = _layoutHandle;
	vbo.flags = _flags;
	vbo.isDynamic = true;
}

void RendererContextGL::updateDynamicVertexBuffer(VertexBufferHandle _handle, uint32_t _dstOffset, uint32_t _size, void* _pData) {
	VertexBufferGL& vbo = m_vertexBuffers[_handle.idx];
	// handle check
	if (vbo.id == 0) {
		spdlog::error("Vertex buffer handle {} not created. Cannot update.", _handle.idx);
		return;
	}
	// data checks
	if (false
		|| _pData == nullptr || _size == 0
		|| (_dstOffset + _size) > vbo.size
		|| vbo.isDynamic == false
	) {
		spdlog::error("Cannot update vertex buffer with invalid data, dst region or non-dynamic buffer.");
		return;
	}
	// update buffer sub region
	glBindBuffer(vbo.target, vbo.id);
	glCheckAndTrace("glBindBuffer", "Bind dynamic vertex buffer {} to GL_ARRAY_BUFFER", vbo.id);
	glBufferSubData(vbo.target, _dstOffset, _size, static_cast<uint8_t*>(_pData));
	glCheckAndTrace("glBufferSubData", "Update dynamic vertex buffer {} at offset {} with size {}", vbo.id, _dstOffset, _size);
	glBindBuffer(vbo.target, 0);
	glCheckAndTrace("glBindBuffer", "Bind default vertex buffer (0) to GL_ARRAY_BUFFER");
	spdlog::info("Updated dynamic vertex buffer handle {} (dstOffset: {}, size: {}).", _handle.idx, _dstOffset, _size);
}

void RendererContextGL::destroyDynamicVertexBuffer(VertexBufferHandle _handle) {
	VertexBufferGL& vbo = m_vertexBuffers[_handle.idx];
	// delete opengl resource if exists
	if (vbo.id == 0) {
		spdlog::warn("Skip destroying invalid dynamic vertex buffer, vertex handle {}", _handle.idx);
	} else {
		glDeleteBuffers(1, &vbo.id);
		glCheckAndTrace("glDeleteBuffers", "Delete dynamic vertex buffer {}", vbo.id);
	}
	vbo = VertexBufferGL{};
	spdlog::info("Destroyed dynamic vertex buffer handle {}.", _handle.idx);
}

void RendererContextGL::createDynamicIndexBuffer(IndexBufferHandle _handle, uint32_t _size, BufferFlags _flags) {
	IndexBufferGL& ibo = m_indexBuffers[_handle.idx];
	// clear if in use
	if (ibo.id != 0) {
		spdlog::warn("Index buffer handle {} already in use. Overwrite existing.", _handle.idx);
		glDeleteBuffers(1, &ibo.id);
		glCheckAndTrace("glDeleteBuffers", "Delete dynamic index buffer {}", ibo.id);
		ibo = IndexBufferGL{};
	}
	// data checks
	if (_size == 0) {
		spdlog::error("Cannot create index buffer with size 0.");
		return;
	}
	// create buffer
	GLuint id;
	glGenBuffers(1, &id);
	glCheckAndTrace("glGenBuffers", "Generate dynamic index buffer {}", id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glCheckAndTrace("glBindBuffer", "Bind dynamic index buffer {} to GL_ELEMENT_ARRAY_BUFFER", id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, nullptr, GL_DYNAMIC_DRAW);
	glCheckAndTrace("glBufferData", "Set bound GL_ELEMENT_ARRAY_BUFFER data of size {} as GL_DYNAMIC_DRAW (nullptr)", _size);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glCheckAndTrace("glBindBuffer", "Bind default index buffer (0) to GL_ELEMENT_ARRAY_BUFFER");
	spdlog::info("Created dynamic index buffer handle {} with size {}.", _handle.idx, _size);
	// write to repr
	ibo.id = id;
	ibo.size = _size;
	ibo.flags = _flags;
	ibo.isDynamic = true;
}

void RendererContextGL::updateDynamicIndexBuffer(IndexBufferHandle _handle, uint32_t _dstOffset, uint32_t _size, void* _pData) {
	IndexBufferGL& ibo = m_indexBuffers[_handle.idx];
	// check buffer exists
	if (ibo.id == 0) {
		spdlog::error("Index buffer handle {} not created. Cannot update.", _handle.idx);
		return;
	}
	// data checks
	if (false
		|| _pData == nullptr || _size == 0
		|| (_dstOffset + _size) > ibo.size
		|| ibo.isDynamic == false
	) {
		spdlog::error("Cannot update index buffer with invalid data, dst region or non-dynamic buffer.");
		return;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.id);
	glCheckAndTrace("glBindBuffer", "Bind dynamic index buffer {} to GL_ELEMENT_ARRAY_BUFFER", ibo.id);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, _dstOffset, _size, static_cast<uint8_t*>(_pData));
	glCheckAndTrace("glBufferSubData", "Update dynamic index buffer {} at offset {} with size {}", ibo.id, _dstOffset, _size);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glCheckAndTrace("glBindBuffer", "Bind default index buffer (0) to GL_ELEMENT_ARRAY_BUFFER");
	spdlog::info("Updated dynamic index buffer handle {} (dstOffset: {}, size: {}).", _handle.idx, _dstOffset, _size);
}

void RendererContextGL::destroyDynamicIndexBuffer(IndexBufferHandle _handle) {
	IndexBufferGL& ibo = m_indexBuffers[_handle.idx];
	// delete opengl resource if exists
	if (ibo.id == 0) {
		spdlog::warn("Skip destroying invalid dynamic index buffer, index handle {}", _handle.idx);
	} else {
		glDeleteBuffers(1, &ibo.id);
		glCheckAndTrace("glDeleteBuffers", "Delete dynamic index buffer {}", ibo.id);
	}
	ibo = IndexBufferGL{};
	spdlog::info("Destroyed dynamic index buffer handle {}.", _handle.idx);
}

// shaders
void RendererContextGL::createShader(ShaderHandle _handle, ShaderType _shaderType, const char* _source) {
	ShaderGL& shader = m_shaders[_handle.idx];
	// clear if in use
	if (shader.id != 0) {
		spdlog::warn("Shader handle {} already in use. Overwrite existing.", _handle.idx);
		glDeleteShader(shader.id);
		glCheckAndTrace("glDeleteShader", "Delete shader {}", shader.id);
		shader = ShaderGL{};
	}
	// data checks
	if (_source == nullptr || std::strlen(_source) == 0 || _shaderType >= VGL_SHADER_TYPE_COUNT) {
		spdlog::error("Cannot create shader with null source or invalid shader type.");
		return;
	}
	// create shader
	GLenum type = s_kShaderTypeMap[_shaderType];
	GLuint id = glCreateShader(type);
	glCheckAndTrace("glCreateShader", "Create shader of type 0x{:X} (id: {})", type, id);
	// compile
	glShaderSource(id, 1, &_source, nullptr);
	glCheckAndTrace("glShaderSource", "Set shader source for id {}", id);
	glCompileShader(id);
	glCheckAndTrace("glCompileShader", "Compile shader id {}", id);
	GLint compileStatus;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_TRUE) {
		spdlog::info("Shader handle {} compiled successfully.", _handle.idx);
	} else {
		GLint logLen = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			char* log = new char[logLen];
			glGetShaderInfoLog(id, logLen, nullptr, log);
			spdlog::error("Failed to compile shader handle {}:\n {}", _handle.idx, log);
			delete[] log;
		} else {
			spdlog::error("Failed to compile shader handle {}: Unknown error.", _handle.idx);
		}
		glDeleteShader(id);
		glCheckAndTrace("glDeleteShader", "Delete failed shader {}", id);
		return;
	}

	// write to repr
	shader.id = id;
	shader.type = type;
	shader.source = (char*)malloc(std::strlen(_source) + 1);
	std::strcpy(shader.source, _source);
}

void RendererContextGL::destroyShader(ShaderHandle _handle) {
	ShaderGL& shader = m_shaders[_handle.idx];
	// delete opengl resource if exists
	if (shader.id == 0) {
		spdlog::warn("Skip destroying invalid shader, shader handle {}", _handle.idx);
	} else {
		glDeleteShader(shader.id);
		glCheckAndTrace("glDeleteShader", "Delete shader {}", shader.id);
	}
	if (shader.source != nullptr) {
		free(shader.source);
	}
	shader = ShaderGL{};
	spdlog::info("Destroyed shader handle {}.", _handle.idx);
}

void RendererContextGL::createProgram(ProgramHandle _handle, ShaderHandle _vert, ShaderHandle _frag) {
	ProgramGL& program = m_programs[_handle.idx];
	// clear if in use
	if (program.id != 0) {
		spdlog::warn("Program handle {} already in use. Overwrite existing.", _handle.idx);
		glDeleteProgram(program.id);
		glCheckAndTrace("glDeleteProgram", "Delete program {}", program.id);
		program = ProgramGL{};
	}
	// data check
	ShaderGL& vertShader = m_shaders[_vert.idx];
	ShaderGL& fragShader = m_shaders[_frag.idx];
	if (vertShader.id == 0 || fragShader.id == 0) {
		spdlog::error("Cannot create program with uncreated shaders (vert: {}, frag: {}).", _vert.idx, _frag.idx);
		return;
	}
	// create program
	GLuint id = glCreateProgram();
	glCheckAndTrace("glCreateProgram", "Create program id {}", id);
	glAttachShader(id, vertShader.id);
	glCheckAndTrace("glAttachShader", "Attach vertex shader {} to program {}", vertShader.id, id);
	glAttachShader(id, fragShader.id);
	glCheckAndTrace("glAttachShader", "Attach fragment shader {} to program {}", fragShader.id, id);
	glLinkProgram(id);
	glCheckAndTrace("glLinkProgram", "Link program id {}", id);
	GLint linkStatus;
	glGetProgramiv(id, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_TRUE) {
		spdlog::info("Program handle {} linked successfully.", _handle.idx);
	} else {
		GLint logLen = 0;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			char* log = new char[logLen];
			glGetProgramInfoLog(id, logLen, nullptr, log);
			spdlog::error("Failed to link program handle {}:\n {}", _handle.idx, log);
			delete[] log;
		} else {
			spdlog::error("Failed to link program handle {}: Unknown error.", _handle.idx);
		}
		glDeleteProgram(id);
		glCheckAndTrace("glDeleteProgram", "Delete failed program {}", id);
		return;
	}
	// write to repr
	program.id = id;
	program.vertHandle = _vert;
	program.fragHandle = _frag;
}

void RendererContextGL::destroyProgram(ProgramHandle _handle) {
	ProgramGL& program = m_programs[_handle.idx];
	// delete opengl resource if exists
	if (program.id == 0) {
		spdlog::warn("Skip destroying invalid program, program handle {}", _handle.idx);
	} else {
		glDeleteProgram(program.id);
		glCheckAndTrace("glDeleteProgram", "Delete program {}", program.id);
	}
	program = ProgramGL{};
	spdlog::info("Destroyed program handle {}.", _handle.idx);
}

// textures
GLenum RendererContextGL::getMinFilter(TexFilter _minFilter, TexFilter _mipFilter, bool _useMips) {
	if (!_useMips) {
		return (_minFilter == VGL_TEXTURE_FILTER_LINEAR)
			? GL_LINEAR
			: GL_NEAREST;
	}

	if (_mipFilter == VGL_TEXTURE_FILTER_LINEAR) {
		return (_minFilter == VGL_TEXTURE_FILTER_LINEAR) 
			? GL_LINEAR_MIPMAP_LINEAR
			: GL_NEAREST_MIPMAP_LINEAR;
	}

	return (_minFilter == VGL_TEXTURE_FILTER_LINEAR)
		? GL_LINEAR_MIPMAP_NEAREST
		: GL_NEAREST_MIPMAP_NEAREST;
}

TextureParamsGL RendererContextGL::unpackTexureParams(TexFlags _flags, bool _useMips) {
	TextureParamsGL params;
	
	// retreive texture parameters using masks and shifts
	TexCompare compare 			= (_flags & VGL_TEXTURE_COMPARE_MASK) >> VGL_TEXTURE_COMPARE_SHIFT;
	TexWrap wrapU 				= (_flags & VGL_TEXTURE_WRAP_U_MASK) >> VGL_TEXTURE_WRAP_U_SHIFT;
	TexWrap wrapV 				= (_flags & VGL_TEXTURE_WRAP_V_MASK) >> VGL_TEXTURE_WRAP_V_SHIFT;
	TexWrap wrapW 				= (_flags & VGL_TEXTURE_WRAP_W_MASK) >> VGL_TEXTURE_WRAP_W_SHIFT;
	TexFilter magFilter 		= (_flags & VGL_TEXTURE_FILTER_MAG_MASK) >> VGL_TEXTURE_FILTER_MAG_SHIFT;
	TexFilter minFilter 		= (_flags & VGL_TEXTURE_FILTER_MIN_MASK) >> VGL_TEXTURE_FILTER_MIN_SHIFT;
	TexFilter mipFilter 		= (_flags & VGL_TEXTURE_FILTER_MIPMAP_MASK) >> VGL_TEXTURE_FILTER_MIPMAP_SHIFT;
	uint32_t borderColorBits 	= (_flags & VGL_TEXTURE_BORDER_COLOUR_MASK) >> VGL_TEXTURE_BORDER_COLOUR_SHIFT;

	// compare mode for shadow mapping
	params.compareMode = (compare < VGL_TEXTURE_COMPARE_COUNT)
		? s_kTexCompareMap[compare]
		: GL_NONE;
	// texture wrapping
	params.wrapU = (wrapU < VGL_TEXTURE_WRAP_COUNT)
		? s_kTexWrapMap[wrapU]
		: VGL_TEXTURE_WRAP_REPEAT;
	params.wrapV = (wrapV < VGL_TEXTURE_WRAP_COUNT)
		? s_kTexWrapMap[wrapV]
		: VGL_TEXTURE_WRAP_REPEAT;
	params.wrapW = (wrapW < VGL_TEXTURE_WRAP_COUNT)
		? s_kTexWrapMap[wrapW]
		: VGL_TEXTURE_WRAP_REPEAT;
	// set border colour as floats
	float borderColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	if (wrapU == GL_CLAMP_TO_BORDER || wrapV == GL_CLAMP_TO_BORDER || wrapW == GL_CLAMP_TO_BORDER) {
		params.borderColour[0] = ((borderColorBits >> 24) & 0x000000ff) / 255.0f;
		params.borderColour[1] = ((borderColorBits >> 16) & 0x000000ff) / 255.0f;
		params.borderColour[2] = ((borderColorBits >> 8) & 0x000000ff) / 255.0f;
		params.borderColour[3] = (borderColorBits & 0x000000ff) / 255.0f;
	}
	// texture filtering
	params.magFilter = (magFilter < VGL_TEXTURE_FILTER_COUNT)
		? s_kTexFilterMap[magFilter]
		: VGL_TEXTURE_FILTER_NEAREST;
	params.minFilter = this->getMinFilter(minFilter, mipFilter, _useMips);

	return params;
}

void RendererContextGL::createTexture(
	TextureHandle _handle,
	TexTarget _target, TexFormat _internalFormat,
	uint16_t _width, uint16_t _height, uint16_t _depth, 
	uint8_t _numLayers, bool _useMips,
	TexFormat _format,
	void* _pData, TexFlags _flags
) {
	TextureGL& texture = m_textures[_handle.idx];
	// clear if in use and not bound to framebuffer
	if (texture.id != 0 && texture.numRT > 0) {
		spdlog::error("Cannot overwrite texture handle {} bound to {} frame buffers.", _handle.idx, texture.numRT);
		return;
	} else if (texture.id != 0) {
		spdlog::warn("Texture handle {} already in use and not bound to any frame buffers. Overwriting existing.", _handle.idx);
		glDeleteTextures(1, &texture.id);
		glCheckAndTrace("glDeleteTextures", "Delete texture {}", texture.id);
		texture = TextureGL{};
	}

	// convert to gl formats
	GLenum target 			= s_kTexTargetMap[_target];
	GLenum internalFormat   = s_kTexFormatMap[_internalFormat].internalFormat;
	GLenum format           = s_kTexFormatMap[_format].format;
	GLenum component        = s_kTexFormatMap[_format].component;
	GLsizei width 			= _width;
	GLsizei height 			= _height;
	GLsizei depth 			= _depth;
	GLsizei numLayers 		= _numLayers;

	// create texture
	GLuint id;
	switch(target) {
		case GL_TEXTURE_2D:
			if (_width == 0 || _height == 0) {
				spdlog::error("Cannot create 2D texture with dimensions 0, width: {}, height: {}.", _width, _height);
				return;
			}
			depth = 1;
			numLayers = 1;
			glGenTextures(1, &id);
			glCheckAndTrace("glGenTextures", "Generate 2D texture {}", id);
			glBindTexture(GL_TEXTURE_2D, id);
			glCheckAndTrace("glBindTexture", "Bind 2D texture {} to GL_TEXTURE_2D", id);
			spdlog::info("internal 0x{:X}, format 0x{:X}, component 0x{:X}", internalFormat, format, component);
			glTexImage2D(
				GL_TEXTURE_2D, 0, internalFormat,
				_width, _height, 0,
				format, component, _pData
			);
			glCheckAndTrace("glTexImage2D", "Set 2D texture image for id {} ({}x{})", id, _width, _height);
			if (_useMips) {
				glGenerateMipmap(GL_TEXTURE_2D);
				glCheckAndTrace("glGenerateMipMap", "Generate mipmaps on target GL_TEXTURE_2D");
			}
			spdlog::info("Created 2D texture handle {} (width: {}, height: {}, mips: {}).", _handle.idx, _width, _height, _useMips);
			break;

		case GL_TEXTURE_3D:
			if (_width == 0 || _height == 0 || _depth == 0) {
				spdlog::error("Cannot create 3D texture with dimensions 0, width: {}, height: {}, depth: {}.", _width, _height, _depth);
				return;
			}
			numLayers = 1;
			glGenTextures(1, &id);
			glCheckAndTrace("glGenTextures", "Generate 3D texture {}", id);
			glBindTexture(GL_TEXTURE_3D, id);
			glCheckAndTrace("glBindTexture", "Bind 3D texture {} to GL_TEXTURE_3D", id);
			glTexImage3D(
				GL_TEXTURE_3D, 0, internalFormat,
				_width, _height, _depth, 0,
				format, component, _pData
			);
			glCheckAndTrace("glTexImage3D", "Set 3D texture image for id {} ({}x{}x{})", id, _width, _height, _depth);
			if (_useMips) {
				glGenerateMipmap(GL_TEXTURE_3D);
				glCheckAndTrace("glGenerateMipMap", "Generate mipmaps on target GL_TEXTURE_3D");
			}
			spdlog::info("Created 3D texture handle {} (width: {}, height: {}, depth: {}, mips: {}).", _handle.idx, _width, _height, _depth, _useMips);
			break;

		case GL_TEXTURE_CUBE_MAP:
			if (_width == 0 || _height == 0) {
				spdlog::error("Cannot create CUBE MAP texture with dimensions 0, width: {}, height: {}.", _width, _height);
				return;
			}
			depth = 1;
			numLayers = 1;
			glGenTextures(1, &id);
			glCheckAndTrace("glGenTextures", "Generate CUBE MAP texture {}", id);
			glBindTexture(GL_TEXTURE_CUBE_MAP, id);
			glCheckAndTrace("glBindTexture", "Bind CUBE MAP texture {} to GL_TEXTURE_CUBE_MAP", id);
			for (uint8_t face = 0; face < 6; ++face) {
				glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, internalFormat,
					_width, _height, 0,
					format, component, _pData
				);
				glCheckAndTrace("glTexImage2D", "Set CUBE MAP face {} image for id {} ({}x{})", face, id, _width, _height);
			}
			if (_useMips) {
				glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
				glCheckAndTrace("glGenerateMipMap", "Generate mipmaps on target GL_TEXTURE_CUBE_MAP");
			}
			spdlog::info("Created CUBE texture handle {} (width: {}, height: {}, mips: {}).", _handle.idx, _width, _height, _useMips);
			break;

		case GL_TEXTURE_2D_ARRAY:
			if (_width == 0 || _height == 0 || _numLayers == 0) {
				spdlog::error("Cannot create 2D ARRAY texture with dimensions 0, width: {}, height: {}, layers: {}.", _width, _height, _numLayers);
				return;
			}
			depth = 1;
			glGenTextures(1, &id);
			glCheckAndTrace("glGenTextures", "Generate 2D ARRAY texture {}", id);
			glBindTexture(GL_TEXTURE_2D_ARRAY, id);
			glCheckAndTrace("glBindTexture", "Bind 2D ARRAY texture {} to GL_TEXTURE_2D_ARRAY", id);
			glTexImage3D(
				GL_TEXTURE_2D_ARRAY, 0, internalFormat,
				_width, _height, _numLayers, 0,
				format, component, _pData
			);
			glCheckAndTrace("glTexImage3D", "Set 2D ARRAY texture image for id {} ({}x{}x{})", id, _width, _height, _numLayers);
			if (_useMips) {
				glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
				glCheckAndTrace("glGenerateMipMap", "Generate mipmaps on target GL_TEXTURE_2D_ARRAY");
			}
			spdlog::info("Created 2D ARRAY texture handle {} (width: {}, height: {}, layers: {}, mips: {}).", _handle.idx, _width, _height, _numLayers, _useMips);  
			break;

		case GL_TEXTURE_CUBE_MAP_ARRAY:
			if (_width == 0 || _height == 0 || _numLayers == 0) {
				spdlog::error("Cannot create CUBE ARRAY texture with dimensions 0, width: {}, height: {}, layers: {}.", _width, _height, _numLayers);
				return;
			}
			depth = 1;
			glGenTextures(1, &id);
			glCheckAndTrace("glGenTextures", "Generate CUBE ARRAY texture {}", id);
			glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, id);
			glCheckAndTrace("glBindTexture", "Bind CUBE ARRAY texture {} to GL_TEXTURE_CUBE_MAP_ARRAY", id);
			glTexImage3D(
				GL_TEXTURE_CUBE_MAP_ARRAY, 0, internalFormat,
				_width, _height, _numLayers * 6, 0,
				format, component, _pData
			);
			glCheckAndTrace("glTexImage3D", "Set CUBE ARRAY texture image for id {} ({}x{}x{})", id, _width, _height, _numLayers * 6);
			if (_useMips) {
				glGenerateMipmap(GL_TEXTURE_CUBE_MAP_ARRAY);
				glCheckAndTrace("glGenerateMipMap", "Generate mipmaps on target GL_TEXTURE_CUBE_MAP_ARRAY");
			}
			spdlog::info("Created CUBE ARRAY texture handle {} (width: {}, height: {}, layers: {}, mips: {}).", _handle.idx, _width, _height, _numLayers, _useMips);
			break;
		default:
			spdlog::error("Unsupported target, cannot create texture handle {} (target: {})", _handle.idx, _target);
	}

	TextureParamsGL params = unpackTexureParams(_flags, _useMips);
	// wrapping
	glTexParameteri(target, GL_TEXTURE_WRAP_S, params.wrapU);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, params.wrapV);
	if (target == GL_TEXTURE_3D
		|| target == GL_TEXTURE_2D_ARRAY
		|| target == GL_TEXTURE_CUBE_MAP
		|| target == GL_TEXTURE_CUBE_MAP_ARRAY) {
		glTexParameteri(target, GL_TEXTURE_WRAP_R, params.wrapW);
	}
	// fitering
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, params.minFilter);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, params.magFilter);
	// comparison mode (shadow map)
	if (params.compareMode != GL_NONE) {
		glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC, params.compareMode);
	} else {
		glTextureParameteri(target, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	}
	// border colour
	if (params.wrapU == GL_CLAMP_TO_BORDER
		|| params.wrapV == GL_CLAMP_TO_BORDER
		|| params.wrapW == GL_CLAMP_TO_BORDER) {
		glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, params.borderColour);
	}

	glBindTexture(target, 0);
	glCheckAndTrace("glBindTexture", "Bind default texture (0) to target 0x{:X}", target);
	
	// write to repr
	texture.id = id;
	texture.target = target;
	texture.internalFormat = internalFormat;
	texture.target = target;
	texture.width = width;
	texture.height = height;
	texture.depth = depth;
	texture.numLayers = numLayers;
	texture.useMips = _useMips;
	texture.format = format;
	texture.component = component;
	texture.numRT = 0;
	texture.params = params;
}

void RendererContextGL::updateTexture(
	TextureHandle _handle,
	uint16_t _width, uint16_t _height, uint16_t _depth,
	uint8_t _layer, uint8_t _face,
	void* _pData
) {
	spdlog::error("Texture update not implemented yet.");
}

void RendererContextGL::readTexture(
	TextureHandle _handle,
	uint8_t _layer, uint8_t _face,
	void* _data
) {
	spdlog::error("Texture readback not implemented yet.");
}

void RendererContextGL::destroyTexture(TextureHandle _handle) {
	TextureGL& texture = m_textures[_handle.idx];

	// check not in use
	if (texture.numRT > 0) {
		spdlog::error("Cannot destroy texture handle {} bound to {} frame buffers.", _handle.idx, texture.numRT);
		return;
	}

	// delete opengl resource if exists
	if (texture.id == 0) {
		spdlog::warn("Skip destroying invalid texture, texture handle {}", _handle.idx);
	} else {
		glDeleteTextures(1, &texture.id);
		glCheckAndTrace("glDeleteTextures", "Delete texture {}", texture.id);
	}
	
	texture = TextureGL{};
	spdlog::info("Destroyed texture handle {}.", _handle.idx);
}

// framebuffers
void RendererContextGL::createFrameBuffer(FrameBufferHandle _handle, uint8_t _num, const Attachment* _pAttachments) {
	FrameBufferGL& fb = m_frameBuffers[_handle.idx];
	
	// if already exists, delete and clear previous attachments
	if (fb.fbo != 0) {
		spdlog::warn("Overwriting existing framebuffer handle {} (fbo {}).", _handle.idx, fb.fbo);
		for (size_t i = 0; i < VGL_MAX_COLOUR_ATTACHMENTS + 2; ++i) {
			if (isValid(fb.attachments[i].texHandle)) {
				TextureGL& tex = m_textures[fb.attachments[i].texHandle.idx];
				if (tex.numRT > 0) tex.numRT--;
			}
		}
		glDeleteFramebuffers(1, &fb.fbo);
		glCheckAndTrace("glDeleteFramebuffers", "Delete framebuffer {}", fb.fbo);
		fb = FrameBufferGL{};
	}

	GLuint fbo = 0;
	glGenFramebuffers(1, &fbo);
	glCheckAndTrace("glGenFramebuffers", "Generate framebuffer {}", fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glCheckAndTrace("glBindFramebuffer", "Bind framebuffer {} to GL_FRAMEBUFFER", fbo);

	GLenum drawBuffers[VGL_MAX_COLOUR_ATTACHMENTS];
	int drawCount = 0;
	uint32_t width = 0, height = 0;

	for (int i = 0; i < _num; i++) {
		const Attachment& a = _pAttachments[i];
		TextureGL& tex = m_textures[a.texHandle.idx];
		GLenum attachPoint = 0;
		if (a.attachmentSlot <= VGL_ATTACHMENT_COLOUR7) {
			attachPoint = GL_COLOR_ATTACHMENT0 + a.attachmentSlot;
			drawBuffers[drawCount++] = attachPoint;
		} else if (a.attachmentSlot == VGL_ATTACHMENT_DEPTH) {
			attachPoint = GL_DEPTH_ATTACHMENT;
		} else if (a.attachmentSlot == VGL_ATTACHMENT_STENCIL) {
			attachPoint = GL_STENCIL_ATTACHMENT;
		} else if (a.attachmentSlot == VGL_ATTACHMENT_DEPTH_STENCIL) {
			attachPoint = GL_DEPTH_STENCIL_ATTACHMENT;
		} else {
			spdlog::error("Unknown attachment slot {} for framebuffer {}.", static_cast<uint32_t>(a.attachmentSlot), _handle.idx);
			continue;
		}

		if (tex.id == 0) {
			spdlog::error("Cannot attach invalid texture {} to framebuffer {}.", a.texHandle.idx, _handle.idx);
			continue;
		} else if (tex.target != GL_TEXTURE_2D) {
			spdlog::error("Cannot attach non-2D texture {} to framebuffer {}", a.texHandle.idx, _handle.idx);
			continue;
		}

		// attach the texture (use generic attach to support arrays/cubes)
		glFramebufferTexture(GL_FRAMEBUFFER, attachPoint, tex.id, 0);
		glCheckAndTrace("glFramebufferTexture", "Attach texture {} to framebuffer {} at attachment 0x{:X}", tex.id, fbo, attachPoint);
		tex.numRT++;
		fb.attachments[i] = a;
		if (width == 0 && tex.width > 0) {
			width = tex.width;
			height = tex.height;
		}
	}

	if (drawCount > 0) {
		glDrawBuffers(drawCount, drawBuffers);
		glCheckAndTrace("glDrawBuffers", "Set draw buffers for framebuffer {} ({} attachments)", fbo, drawCount);
	} else {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glCheckAndTrace("glDrawBuffer", "Set draw buffer to GL_NONE for framebuffer {}", fbo);
	}

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		spdlog::error("Framebuffer {} incomplete (status: 0x{:x}).", _handle.idx, status);
	} else {
		spdlog::info("Created framebuffer handle {} (fbo: {}, attachments: {}).", _handle.idx, fbo, _num);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCheckAndTrace("glBindFramebuffer", "Bind default framebuffer (0) to GL_FRAMEBUFFER");

	fb.fbo = fbo;
	fb.width = width;
	fb.height = height;
}

void RendererContextGL::destroyFrameBuffer(FrameBufferHandle _handle) {
	FrameBufferGL& fb = m_frameBuffers[_handle.idx];

	// delete opengl resource if exists
	if (fb.fbo == 0) {
		spdlog::warn("Skip destroying invalid frame buffer, frame buffer handle {}", _handle.idx);
	} else {
		// decrement attached textures' render target counters
		for (size_t i = 0; i < VGL_MAX_COLOUR_ATTACHMENTS + 2; ++i) {
			if (isValid(fb.attachments[i].texHandle)) {
				TextureGL& tex = m_textures[fb.attachments[i].texHandle.idx];
				if (tex.numRT > 0) tex.numRT--;
			}
		}
		glDeleteFramebuffers(1, &fb.fbo);
		glCheckAndTrace("glDeleteFramebuffers", "Delete framebuffer {}", fb.fbo);
	}

	fb = FrameBufferGL{};
	spdlog::info("Destroyed framebuffer handle {}.", _handle.idx);
}

} // namespace gl

} // namespace vgl
