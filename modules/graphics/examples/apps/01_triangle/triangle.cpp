#include "triangle.hpp"

// internal
#include "vgl/defines.hpp"
#include "common/util.hpp"

using namespace vgl;

REGISTER_APP_INSTANCE(TriangleApp, triangleAppInstance)
App* getTriangleAppInstance() {
	return triangleAppInstance;
}

TriangleApp::TriangleApp() {
	// register app instance
	if (!registerApp(1, this)) {
		std::cerr << "[TriangleApp] ERROR: Failed to register app instance (index=1)." << std::endl;
	}
}

bool TriangleApp::init() {
	const Resolution& res = getResolution();

	// cube mesh resources
	m_posLayout.begin()
		.add(VGL_ATTRIB_POSITION, VGL_ATTRIB_TYPE_FLOAT, 3)
		.skip(sizeof(float) * 2)
	.end();

	m_hPosLayout = createVertexLayout(
		m_posLayout
	);

	m_hTriVertexBuffer = createVertexBuffer(
		sizeof(util::kTriangleVertices),
		m_hPosLayout,
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
	m_fragStr = util::loadTextFile(m_fragPath);

	m_hVertShader = createShader(VGL_SHADER_TYPE_VERTEX, m_vertStr.c_str());
	m_hFragShader = createShader(VGL_SHADER_TYPE_FRAGMENT, m_fragStr.c_str());
	m_hProgram = createProgram(m_hVertShader, m_hFragShader);


	return true;
}

void TriangleApp::drawTriangle() {
	// open encoder
	Encoder* encoder = begin();
	if (!encoder) {
		std::cout << "[TriangleApp] ERROR: Failed to get encoder in drawCube." << std::endl;
		return;
	}

	// draw cube right
	//encoder->setState(VGL_STATE_DEPTH_TEST_DISABLED | VGL_STATE_CULL_TEST_DISABLED | VGL_STATE_PT_TRIANGLES);
	encoder->setState(VGL_STATE_DEFAULT);
	encoder->setVertexBuffer(0, m_hTriVertexBuffer);	
	encoder->setIndexBuffer(m_hTriIndexBuffer, 0, util::kNumTriangleIndices);
	encoder->submit(0, m_hProgram, VGL_DISCARD_ALL);

	// close encoder
	end(encoder);
}

void TriangleApp::draw(double _dt) {
	// get frame consts
	const Resolution& res = getResolution();

	// set default view state
	setViewClear(0, VGL_CLEAR_ALL, 0x303030ff);
	setViewRect(0, 0, 0, res.width, res.height);
	setViewScissor(0, 0, 0, res.width, res.height);
	setViewFrameBuffer(0, VGL_INVALID_HANDLE);

	// draw triangle
	TriangleApp::drawTriangle();
	
	// finish frame
	frame();

	// draw to screen
	uint32_t frameIdx = render();
	std::cout << "[TriangleApp] Completed Frame " << frameIdx << std::endl;
};

void TriangleApp::shutdown() {
	// destroy resources
	destroy(m_hProgram);
	destroy(m_hVertShader);
	destroy(m_hFragShader);
	destroy(m_hTriVertexBuffer);
	destroy(m_hTriIndexBuffer);
	destroy(m_hPosLayout);
};
