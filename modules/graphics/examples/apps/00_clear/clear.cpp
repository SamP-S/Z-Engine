#include "clear.hpp"

// internal
#include "vgl/defines.hpp"
#include "common/util.hpp"
#include "common/driver.hpp"
// external
#include "la/la_extended.h"

using namespace vgl;

REGISTER_APP_INSTANCE(ClearApp, clearAppInstance)
App* getClearAppInstance() {
	return clearAppInstance;
}

ClearApp::ClearApp() {
	// register app instance
	if (!registerApp(0, this)) {
		std::cerr << "[ClearApp] ERROR: Failed to register app instance (index=0)." << std::endl;
	}
}

bool ClearApp::init() {
	return true;
}

void ClearApp::draw(double _dt) {
	// get frame consts
	const Resolution& res = getResolution();
	const double time = Driver::getTime();

	// colour cycle params
	const float kSpeed = 0.5f;
	const float kOffset = 2 * LA::PI / 3.0f;

	// change clear colour over elapsed time
	float red = (std::sin(static_cast<float>(time * kSpeed))) * 0.5f + 0.5f;
	float green = (std::sin(static_cast<float>(time * kSpeed + kOffset))) * 0.5f + 0.5f;
	float blue = (std::sin(static_cast<float>(time * kSpeed + 2 * kOffset))) * 0.5f + 0.5f;
	uint32_t clearColour =
		(static_cast<uint32_t>(red * 255.0f) << 24)
		| (static_cast<uint32_t>(green * 255.0f) << 16)
		| (static_cast<uint32_t>(blue * 255.0f) << 8)
		| (0xff);

	// set default view state
	setViewClear(0, VGL_CLEAR_ALL, clearColour);
	setViewRect(0, 0, 0, res.width, res.height);
	setViewScissor(0, 0, 0, res.width, res.height);
	setViewFrameBuffer(0, VGL_INVALID_HANDLE);

	// touch draw call to force clear
	Encoder* encoder = begin();
	if (encoder) {
		encoder->touch(0);
	} else {
		std::cout << "[ClearApp] ERROR: Failed to get encoder in draw." << std::endl;
	}
	end(encoder);

	// finish frame
	frame();

	// draw to screen
	uint32_t frameIdx = render();
	std::cout << "[ClearApp] Completed Frame " << frameIdx << std::endl;
};

void ClearApp::shutdown() {};
