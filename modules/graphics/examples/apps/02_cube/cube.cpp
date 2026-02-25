#include "cube.hpp"

// internal
#include "vgl/defines.hpp"
#include "common/util.hpp"
#include "common/driver.hpp"

using namespace vgl;

REGISTER_APP_INSTANCE(CubeApp, cubeAppInstance)
App* getCubeAppInstance() {
	return cubeAppInstance;
}

CubeApp::CubeApp() {
	// register app instance
	if (!registerApp(2, this)) {
		std::cerr << "[CubeApp] ERROR: Failed to register app instance (index=2)." << std::endl;
	}
}

bool CubeApp::init() {
	const Resolution& res = getResolution();

	// cube mesh resources
	m_posLayout.begin()
		.add(VGL_ATTRIB_POSITION, VGL_ATTRIB_TYPE_FLOAT, 3)
	.end();

	m_hPosLayout = createVertexLayout(
		m_posLayout
	);

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

	/// shaders
	// read shader from "common/shaders/simple.xxx" as .vert/.frag and store as char* 
	m_vertStr = util::loadTextFile(m_vertPath);
	m_fragStr = util::loadTextFile(m_fragPath);

	m_hVertShader = createShader(VGL_SHADER_TYPE_VERTEX, m_vertStr.c_str());
	m_hFragShader = createShader(VGL_SHADER_TYPE_FRAGMENT, m_fragStr.c_str());
	m_hProgram = createProgram(m_hVertShader, m_hFragShader);


	return true;
}

void CubeApp::drawCube(LA::mat4 _model, LA::vec4 _colour) {
	// open encoder
	Encoder* encoder = begin();
	if (!encoder) {
		std::cout << "[CubeApp] ERROR: Failed to get encoder in drawCube." << std::endl;
		return;
	}
	
	// upload camera uniforms
	const Resolution& res = getResolution();
	encoder->setUniform("uView", VGL_UNIFORM_TYPE_MAT4, &m_view, 1);
	encoder->setUniform("uProjection", VGL_UNIFORM_TYPE_MAT4, &m_proj, 1);

	// draw cube right2
	encoder->setUniform("uModel", VGL_UNIFORM_TYPE_MAT4, &_model, 1);
	encoder->setUniform("uColour", VGL_UNIFORM_TYPE_VEC4, &_colour, 1);
	// encoder->setState(VGL_STATE_DEPTH_TEST_LESS | VGL_STATE_CULL_TEST_CW | VGL_STATE_PT_TRIANGLES);
	encoder->setState(VGL_STATE_DEFAULT);
	encoder->setVertexBuffer(0, m_hCubeVertexBuffer);	
	encoder->setIndexBuffer(m_hCubeIndexBuffer, 0, util::kNumCubeIndices);
	encoder->submit(0, m_hProgram, VGL_DISCARD_ALL);

	// close encoder
	end(encoder);
}

void CubeApp::draw(double _dt) {
	// get frame consts
	const Resolution& res = getResolution();
	const double time = Driver::getTime();
	m_view = LA::LookAt({0.0f, 1.0f, 2.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});
	m_proj = LA::Perspective(60.0f, res.aspect(), 0.1f, 100.0f);
	const float kRotateSpeed = 360.0f / 8.0f;

	// set default view state
	setViewClear(0, VGL_CLEAR_ALL, 0x303030ff);
	setViewRect(0, 0, 0, res.width, res.height);
	setViewScissor(0, 0, 0, res.width, res.height);
	setViewFrameBuffer(0, VGL_INVALID_HANDLE);

	// draw cube centre
	LA::mat4 model = LA::Transformation(
		{0.0f, 0.0f, 0.0f},
		{0.0f, (float)time * kRotateSpeed, 0.0f},
		{0.25f, 0.25f, 0.25f}
	);
	LA::vec4 colour = LA::vec4({0.0f, 1.0f, 0.0f, 1.0f});
	CubeApp::drawCube(model, colour);
	
	// finish frame
	frame();

	// draw to screen
	uint32_t frameIdx = render();
	std::cout << "[CubeApp] Completed Frame " << frameIdx << std::endl;
};

void CubeApp::shutdown() {
	// destroy resources
	destroy(m_hProgram);
	destroy(m_hVertShader);
	destroy(m_hFragShader);
	destroy(m_hCubeVertexBuffer);
	destroy(m_hCubeIndexBuffer);
	destroy(m_hPosLayout);
};
