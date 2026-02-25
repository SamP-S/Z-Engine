#include "texture.hpp"

// internal
#include "vgl/defines.hpp"
#include "common/util.hpp"
#include "common/driver.hpp"
// external
#include "stb/stb_image.h"

using namespace vgl;

REGISTER_APP_INSTANCE(TextureApp, textureAppInstance)
App* getTextureAppInstance() {
	return textureAppInstance;
}

TextureApp::TextureApp() {
	// register app instance
	if (!registerApp(3, this)) {
		std::cerr << "[TextureApp] ERROR: Failed to register app instance (index=3)." << std::endl;
	}
}

bool TextureApp::init() {
	const Resolution& res = getResolution();

	// quad mesh resources
	m_posTexLayout.begin()
		.add(VGL_ATTRIB_POSITION, VGL_ATTRIB_TYPE_FLOAT, 3)
		.add(VGL_ATTRIB_TEXCOORD0, VGL_ATTRIB_TYPE_FLOAT, 2)
	.end();

	m_hPosTexLayout = createVertexLayout(
		m_posTexLayout
	);

	m_hQuadVertexBuffer = createVertexBuffer(
		sizeof(util::kQuadVertices),
		m_hPosTexLayout,
		reinterpret_cast<const void*>(util::kQuadVertices),
		VGL_BUFFER_NONE
	);

	m_hQuadIndexBuffer = createIndexBuffer(
		sizeof(util::kQuadIndices),
		reinterpret_cast<const void*>(util::kQuadIndices),
		VGL_BUFFER_NONE
	);

	/// shaders
	// read shader from "common/shaders/simple.xxx" as .vert/.frag and store as char* 
	m_vertStr = util::loadTextFile(m_vertPath);
	m_fragStr = util::loadTextFile(m_fragPath);

	m_hVertShader = createShader(VGL_SHADER_TYPE_VERTEX, m_vertStr.c_str());
	m_hFragShader = createShader(VGL_SHADER_TYPE_FRAGMENT, m_fragStr.c_str());
	m_hProgram = createProgram(m_hVertShader, m_hFragShader);

	// tmp vars
	int w, h, c;
	uint8_t* data = nullptr; 
	
	// load colour texture
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
	
	// load preview image
	data = stbi_load(m_tex1Path, &w, &h, &c, 0);
	std::cout << "[TextureApp] Loaded image (" << w << "x" << h << "x" << c << ") @ " << m_tex1Path << std::endl;
	m_hTexture1 = createTexture2D(
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


	return true;
}

void TextureApp::drawTexQuad(LA::mat4 _model, TextureHandle _hTex) {
	// open encoder
	Encoder* encoder = begin();
	if (!encoder) {
		std::cout << "[TextureApp] ERROR: Failed to get encoder in drawQuad." << std::endl;
		return;
	}

	// upload camera uniforms
	const Resolution& res = getResolution();
	encoder->setUniform("uView", VGL_UNIFORM_TYPE_MAT4, &m_view, 1);
	encoder->setUniform("uProjection", VGL_UNIFORM_TYPE_MAT4, &m_proj, 1);

	// draw quad right2
	encoder->setUniform("uModel", VGL_UNIFORM_TYPE_MAT4, &_model, 1);
	encoder->setTextureUnit(0, "uTexture", _hTex);
	// encoder->setState(VGL_STATE_DEPTH_TEST_LESS | VGL_STATE_CULL_TEST_CW | VGL_STATE_PT_TRIANGLES);
	encoder->setState(VGL_STATE_DEFAULT);
	encoder->setVertexBuffer(0, m_hQuadVertexBuffer);	
	encoder->setIndexBuffer(m_hQuadIndexBuffer, 0, util::kNumQuadIndices);
	encoder->submit(0, m_hProgram, VGL_DISCARD_ALL);

	// close encoder
	end(encoder);
}

void TextureApp::draw(double _dt) {
	// get frame consts
	const Resolution& res = getResolution();
	const double time = Driver::getTime();
	m_view = LA::Translate(0.0f, 0.0f, -1.5f);
	m_proj = LA::Perspective(60.0f, res.aspect(), 0.1f, 100.0f);
	uint32_t secs = static_cast<uint32_t>(time);

	// set default view state
	setViewClear(0, VGL_CLEAR_ALL, 0x303030ff);
	setViewRect(0, 0, 0, res.width, res.height);
	setViewScissor(0, 0, 0, res.width, res.height);
	setViewFrameBuffer(0, VGL_INVALID_HANDLE);

	// set model transform & texture
	LA::mat4 model = LA::Transformation(
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		{0.5f, 0.5f, 0.5f}
	);
	TextureHandle hTex = { VGL_INVALID_HANDLE };
	if (secs % 2) {
		hTex = m_hTexture0;
	} else {
		hTex = m_hTexture1;
	}

	// call draw routine
	TextureApp::drawTexQuad(model, hTex);
	
	// finish frame
	frame();

	// draw to screen
	uint32_t frameIdx = render();
	std::cout << "[TextureApp] Completed Frame " << frameIdx << std::endl;
};

void TextureApp::shutdown() {
	// destroy resources
	destroy(m_hTexture0);
	destroy(m_hTexture1);
	destroy(m_hProgram);
	destroy(m_hVertShader);
	destroy(m_hFragShader);
	destroy(m_hQuadVertexBuffer);
	destroy(m_hQuadIndexBuffer);
	destroy(m_hQuadVertexBuffer);
	destroy(m_hQuadIndexBuffer);
};
