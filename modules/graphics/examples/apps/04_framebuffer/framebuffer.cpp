#include "framebuffer.hpp"

// internal
#include "vgl/defines.hpp"
#include "common/util.hpp"
#include "common/driver.hpp"
// external
#include "stb/stb_image.h"

using namespace vgl;

REGISTER_APP_INSTANCE(FramebufferApp, framebufferAppInstance)
App* getFramebufferAppInstance() {
	return framebufferAppInstance;
}

FramebufferApp::FramebufferApp() {
	// register app instance
	if (!registerApp(4, this)) {
		std::cerr << "[FramebufferApp] ERROR: Failed to register app instance (index=4)." << std::endl;
	}
}

bool FramebufferApp::init() {
	const Resolution& res = getResolution();

	// vertex layouts
	m_posLayout.begin()
		.add(VGL_ATTRIB_POSITION, VGL_ATTRIB_TYPE_FLOAT, 3)
	.end();
	m_posTexLayout.begin()
		.add(VGL_ATTRIB_POSITION, VGL_ATTRIB_TYPE_FLOAT, 3)
		.add(VGL_ATTRIB_TEXCOORD0, VGL_ATTRIB_TYPE_FLOAT, 2)
	.end();
	m_hPosLayout = createVertexLayout(m_posLayout);
	m_hPosTexLayout = createVertexLayout(m_posTexLayout);

	// cube
	m_hCubeVertexBuffer = createVertexBuffer(
		sizeof(util::kCubeVertices),
		m_hPosLayout,
		reinterpret_cast<const void*>(util::kCubeVertices),
		VGL_BUFFER_NONE
	);
	m_hCubeIndexBuffer = createIndexBuffer(
		sizeof(util::kCubeIndices),
		reinterpret_cast<const void*>(util::kCubeIndices),
		VGL_BUFFER_INDEX32
	);
	// triangle
	m_hTriVertexBuffer = createVertexBuffer(
		sizeof(util::kTriangleVertices),
		m_hPosTexLayout,
		reinterpret_cast<const void*>(util::kTriangleVertices),
		VGL_BUFFER_NONE
	);
	m_hTriIndexBuffer = createIndexBuffer(
		sizeof(util::kTriangleIndices),
		reinterpret_cast<const void*>(util::kTriangleIndices),
		VGL_BUFFER_NONE
	);

	/// shaders
	// read shader from "common/shaders/simple.xxx" as .vert/.frag and store as char* 
	m_vertStr = util::loadTextFile(m_vertPath);
	m_posFragStr = util::loadTextFile(m_posFragPath);
	m_texFragStr = util::loadTextFile(m_texFragPath);

	m_hVertShader = createShader(VGL_SHADER_TYPE_VERTEX, m_vertStr.c_str());
	m_hPosFragShader = createShader(VGL_SHADER_TYPE_FRAGMENT, m_posFragStr.c_str());
	m_hTexFragShader = createShader(VGL_SHADER_TYPE_FRAGMENT, m_texFragStr.c_str());
	m_hPosProgram = createProgram(m_hVertShader, m_hPosFragShader);
	m_hTexProgram = createProgram(m_hVertShader, m_hTexFragShader);


	// texture
	int w, h, c = 0;
	uint8_t* data = nullptr;
	data = stbi_load(m_tex0Path, &w, &h, &c, 0);
	std::cout << "[TextureApp] Loaded image (" << w << "x" << h << "x" << c << ") @ " << m_tex0Path << std::endl;
	m_hTexture0 = createTexture2D(
		VGL_TEXTURE_FORMAT_RGBA32F,
		w,
		h,
		1,
		true,
		VGL_TEXTURE_FORMAT_RGB8U,
		static_cast<void*>(data),
		VGL_TEXTURE_DEFAULT
	);
	stbi_image_free(data);

	// create framebuffer attachments
	m_hFBColourTex = createTexture2D(
		VGL_TEXTURE_FORMAT_RGBA32F,
		res.width,
		res.height,
		1,
		false,
		VGL_TEXTURE_FORMAT_RGBA32F,
		nullptr,
		VGL_TEXTURE_DEFAULT
	);
	m_hFBDepthStencilTex = createTexture2D(
		VGL_TEXTURE_FORMAT_D24FS8U,
		res.width,
		res.height,
		1,
		false,
		VGL_TEXTURE_FORMAT_D24FS8U,
		nullptr,
		VGL_TEXTURE_DEFAULT
	);
	m_hFrameBuffer = createFrameBuffer(
		m_hFBColourTex,
		m_hFBDepthStencilTex	
	);

	return true;
}

void FramebufferApp::drawCube(LA::mat4 _model) {
	Encoder* encoder = begin();
	if (!encoder) {
		std::cout << "[FramebufferApp] ERROR: Failed to get encoder in drawCube." << std::endl;
		return;
	}

	// upload camera uniforms
	const Resolution& res = getResolution();
	encoder->setUniform("uView", VGL_UNIFORM_TYPE_MAT4, &m_view, 1);
	encoder->setUniform("uProjection", VGL_UNIFORM_TYPE_MAT4, &m_proj, 1);

	// draw cube
	encoder->setUniform("uModel", VGL_UNIFORM_TYPE_MAT4, &_model, 1);
	encoder->setState(VGL_STATE_DEFAULT);
	encoder->setVertexBuffer(0, m_hCubeVertexBuffer);
	encoder->setIndexBuffer(m_hCubeIndexBuffer, 0, util::kNumCubeIndices);
	encoder->submit(1, m_hPosProgram, VGL_DISCARD_ALL);
}

void FramebufferApp::drawTexTri(LA::mat4 _model, TextureHandle _hTex) {
	// open encoder
	Encoder* encoder = begin();
	if (!encoder) {
		std::cout << "[FramebufferApp] ERROR: Failed to get encoder in drawQuad." << std::endl;
		return;
	}

	// upload camera uniforms
	const Resolution& res = getResolution();
	LA::mat4 mat = LA::mat4();
	encoder->setUniform("uView", VGL_UNIFORM_TYPE_MAT4, &mat, 1);
	encoder->setUniform("uProjection", VGL_UNIFORM_TYPE_MAT4, &mat, 1);

	// draw quad right2
	encoder->setUniform("uModel", VGL_UNIFORM_TYPE_MAT4, &_model, 1);
	encoder->setTextureUnit(0, "uTexture", _hTex);
	// encoder->setState(VGL_STATE_DEPTH_TEST_DISABLED | VGL_STATE_CULL_TEST_CW | VGL_STATE_PT_TRIANGLES);
	encoder->setState(VGL_STATE_DEFAULT);
	encoder->setVertexBuffer(0, m_hTriVertexBuffer);	
	encoder->setIndexBuffer(m_hTriIndexBuffer, 0, util::kNumTriangleIndices);
	encoder->submit(0, m_hTexProgram, VGL_DISCARD_ALL);

	// close encoder
	end(encoder);
}

void FramebufferApp::draw(double _dt) {
	// get frame consts
	const Resolution& res = getResolution();
	const float time = (float)Driver::getTime();
	m_view = LA::LookAt({0.0f, 1.0f, 2.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
	m_proj = LA::Perspective(60.0f, res.aspect(), 0.1f, 100.0f);
	uint32_t secs = static_cast<uint32_t>(time);
	const float kSpeed = 360.0f / 10.0f;

	// set default view state
	setViewClear(0, VGL_CLEAR_ALL, 0x303030ff);
	setViewRect(0, 0, 0, res.width, res.height);
	setViewScissor(0, 0, 0, res.width, res.height);
	setViewFrameBuffer(0, VGL_INVALID_HANDLE);

	// set offscreen view state
	setViewClear(1, VGL_CLEAR_ALL, 0x555555ff);
	setViewRect(1, 0, 0, res.width, res.height);
	setViewScissor(1, 0, 0, 0, 0);
	setViewFrameBuffer(1, m_hFrameBuffer);

	// draw rotating cube to offscreen fb
	LA::mat4 model = LA::Transformation(
		{0.0f, 0.0f, 0.0f},
		{0.0f, time * kSpeed, 0.0f},
		{0.25f, 0.25f, 0.25f}
	);
	FramebufferApp::drawCube(model);

	// draw fb/tex on triangle
	LA::mat4 mat = LA::mat4();
	TextureHandle hTex = { VGL_INVALID_HANDLE };
	if (secs % 2) {
		hTex = m_hFBColourTex;
	} else {
		hTex = m_hTexture0;
	}
	FramebufferApp::drawTexTri(mat, hTex);
	
	// finish frame
	frame();

	// draw to screen
	uint32_t frameIdx = render();
	std::cout << "[FramebufferApp] Completed Frame " << frameIdx << std::endl;
};

void FramebufferApp::shutdown() {
	// destroy resources
	destroy(m_hTexture0);
	destroy(m_hPosProgram);
	destroy(m_hTexProgram);
	destroy(m_hVertShader);
	destroy(m_hPosFragShader);
	destroy(m_hTexFragShader);
	destroy(m_hPosLayout);
	destroy(m_hPosTexLayout);
	destroy(m_hCubeVertexBuffer);
	destroy(m_hCubeIndexBuffer);
	destroy(m_hTriVertexBuffer);
	destroy(m_hTriIndexBuffer);
	destroy(m_hFBColourTex);
	destroy(m_hFBDepthStencilTex);
	destroy(m_hFrameBuffer);
};
