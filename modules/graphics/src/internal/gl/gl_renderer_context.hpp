#pragma once

// internal
#include "internal/i_renderer_context.hpp"
// external
#include <GL/glew.h>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/fmt.h"

namespace vgl {

namespace gl {

struct VertexBufferGL {
    GLuint id 			= 0;
    GLenum target 		= 0;
    uint32_t size 		= 0;
    VertexLayoutHandle layoutHandle = { VGL_INVALID_HANDLE };
    BufferFlags flags 	= VGL_BUFFER_NONE;
    bool isDynamic 		= false;
};

struct IndexBufferGL {
    GLuint id 			= 0;
    uint32_t size 		= 0;
    BufferFlags flags 	= VGL_BUFFER_NONE;
    bool isDynamic 		= false;
};

struct ShaderGL {
    GLuint id 		= 0;
    GLenum type 	= 0;
    char* source 	= nullptr;
};

struct ProgramGL {
    GLuint id 				= 0;
	ShaderHandle vertHandle = { VGL_INVALID_HANDLE };
	ShaderHandle fragHandle = { VGL_INVALID_HANDLE };
};

struct TextureParamsGL {
    GLenum 	wrapU 			= GL_REPEAT;
    GLenum 	wrapV 			= GL_REPEAT;
    GLenum 	wrapW 			= GL_REPEAT;
    GLenum 	minFilter 		= GL_NEAREST_MIPMAP_LINEAR;
    GLenum 	magFilter 		= GL_LINEAR;
    GLenum 	compareMode 	= GL_NONE;
    GLfloat borderColour[4] = {0.0f, 0.0f, 0.0f, 0.0f};
};

struct TextureGL {
    GLuint id 				= 0;
    GLenum target 			= 0;
    GLenum internalFormat 	= 0;
    uint32_t width 			= 0;
    uint32_t height 		= 0;
    uint32_t depth 			= 0;
    uint32_t numLayers 		= 0;
    bool useMips 			= false;
    GLenum format 			= 0;     // format of supplied data
    GLenum component 		= 0;  // data type of supplied data
    TextureParamsGL params 	= {};
    uint16_t numRT 			= 0;
};

struct FrameBufferGL {
    GLuint fbo 		= 0;
    uint32_t width 	= 0;
    uint32_t height = 0;
    Attachment attachments[VGL_MAX_COLOUR_ATTACHMENTS + 2] = {};
};

struct TexFormatInfoGL {
    GLenum internalFormat	= 0;
    GLenum format 			= 0;
    GLenum component 		= 0;
};

class RendererContextGL : public IRendererContext {
private:
	Config m_config = {};

	VertexLayout m_vertexLayouts[VGL_MAX_VERTEX_LAYOUTS] 	= {};
    VertexBufferGL m_vertexBuffers[VGL_MAX_VERTEX_BUFFERS] 	= {};
    IndexBufferGL m_indexBuffers[VGL_MAX_INDEX_BUFFERS]		= {};
    ShaderGL m_shaders[VGL_MAX_SHADERS] 	= {};
    ProgramGL m_programs[VGL_MAX_PROGRAMS] 	= {};
	TextureGL m_textures[VGL_MAX_TEXTURES] 	= {};
    FrameBufferGL m_frameBuffers[VGL_MAX_FRAME_BUFFERS] 	= {};

    GLuint m_vao = 0;
    
    // shader
    static const GLenum s_kShaderTypeMap[VGL_SHADER_TYPE_COUNT];
    // texture
    static const GLenum s_kTexTargetMap[VGL_TEXTURE_TARGET_COUNT];
    static const TexFormatInfoGL s_kTexFormatMap[VGL_TEXTURE_FORMAT_COUNT];
    static const GLenum s_kTexFilterMap[VGL_TEXTURE_FILTER_COUNT];
    static const GLenum s_kTexWrapMap[VGL_TEXTURE_WRAP_COUNT];
    static const GLenum s_kTexCompareMap[VGL_TEXTURE_COMPARE_COUNT];
    static const std::unordered_map<GLenum, uint32_t> s_kTexComponentSizeMap;
    // vertex attribute
    static const GLenum s_kAttribTypeMap[VGL_ATTRIB_TYPE_COUNT];
    // state
    static const GLenum s_kDepthTestMap[VGL_DEPTH_TEST_COUNT];
    static const GLenum s_kCullTestMap[VGL_CULL_TEST_COUNT];
    static const GLenum s_kPrimitiveTypeMap[VGL_PT_COUNT];
    static const GLenum s_kPolygonMap[VGL_POLYGON_COUNT];
    // error/debugs
	static const std::unordered_map<GLenum, const char*> s_kGLErrorStrMap;

public:

	RendererContextGL();
    ~RendererContextGL() override;

    RendererType getRendererType() const override;
    const char* getRendererName() const override;

    bool init(const Config& _config) override;
    void shutdown() override;
	void window(uint16_t _width, uint16_t _height) override;

    void render(Frame* _frame) override;

	static const char* glErrorToString(GLenum _err);
	static bool glCheckError(const char* _label);
	static void glCheckErrors(const char* _label);

    template<typename... Args>
    static bool glCheckAndTrace(const char* _cmd, const char* _msg, Args&&... _args) {
        GLenum error;
        int errorCount = 0;

        // format once
        auto message = fmt::format(fmt::runtime(_msg), std::forward<Args>(_args)...);
        
        // flush errors
        while((error = glGetError()) != GL_NO_ERROR) {
            errorCount++;
            spdlog::error(
                "[OpenGL] [{}] {}: ERROR={}",
                _cmd,
                message,
                glErrorToString(error)
            );
        }

        if (errorCount == 0) {
            spdlog::trace(
                "[OpenGL] [{}] {}",
                _cmd,
                message
            );
        }

        return errorCount == 0;
    }

	// views
	void applyViewState(View& _view);

    // layouts
    void createVertexLayout(VertexLayoutHandle _handle, const VertexLayout& _layout) override;
    void destroyVertexLayout(VertexLayoutHandle _handle) override;

    // buffers
    void createVertexBuffer(VertexBufferHandle _handle, uint32_t _size, VertexLayoutHandle _layoutHandle, void* _pData, BufferFlags _flags = VGL_BUFFER_NONE) override;
    void destroyVertexBuffer(VertexBufferHandle _handle) override;
    void createIndexBuffer(IndexBufferHandle _handle, uint32_t _size, void* _pData, BufferFlags _flags = VGL_BUFFER_NONE) override;
    void destroyIndexBuffer(IndexBufferHandle _handle) override;

    // dynamic buffers
    void createDynamicVertexBuffer(VertexBufferHandle _handle, uint32_t _size, VertexLayoutHandle _layoutHandle, BufferFlags _flags = VGL_BUFFER_NONE) override;
    void updateDynamicVertexBuffer(VertexBufferHandle _handle, uint32_t _dstOffset, uint32_t _size, void* _pData) override;
    void destroyDynamicVertexBuffer(VertexBufferHandle _handle) override;
    void createDynamicIndexBuffer(IndexBufferHandle _handle, uint32_t _size, BufferFlags _flags = VGL_BUFFER_NONE) override;
    void updateDynamicIndexBuffer(IndexBufferHandle _handle, uint32_t _dstOffset, uint32_t _size, void* _pData) override;
    void destroyDynamicIndexBuffer(IndexBufferHandle _handle) override;

    // shaders
    void createShader(ShaderHandle _handle, ShaderType _shaderType, const char* _source) override;
    void destroyShader(ShaderHandle _handle) override;
    void createProgram(ProgramHandle _handle, ShaderHandle _vert, ShaderHandle _frag) override;
    void destroyProgram(ProgramHandle _handle) override;

    // textures
	GLenum getMinFilter(uint8_t _minFilter, uint8_t _mipFilter, bool _useMips);
	TextureParamsGL unpackTexureParams(TexFlags _flags, bool _useMips);
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

    // framebuffers
   	//  void createFrameBuffer(FrameBufferHandle _handle, uint16_t _width, uint16_t _height, TexFormat _format, TexFormat _depthFormat) override;
    void createFrameBuffer(FrameBufferHandle _handle, uint8_t _num, const Attachment* _pAttachments) override;
    void destroyFrameBuffer(FrameBufferHandle _handle) override;
};

} // namespace gl

} // namespace vgl