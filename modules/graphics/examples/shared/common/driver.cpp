#include "driver.hpp"

// std lib
#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
// internal
#include "vgl/defines.hpp"
#include "common/app.hpp"

// static time
uint64_t Driver::s_startCount = 0;
double Driver::getTime() {
	uint64_t curCount = SDL_GetPerformanceCounter();
	double time = static_cast<double>(curCount - s_startCount) / SDL_GetPerformanceFrequency();
	return time;
}

/* Driver Class */
bool Driver::init(int _appIndex, RendererType _type) {
	std::cout << "[Driver] init begin" << std::endl;
	// create vgl config
	vgl::Config cfg;
	cfg.rendererType = _type;

	// set renderer
	uint32_t winFlags = 0;
	switch (cfg.rendererType) {
		case VGL_RENDERER_NOOP: {
			std::cout << "[Driver] Noop renderer selected. No SDL flags needed." << std::endl;
		} break;
		case VGL_RENDERER_OPENGL: {
			winFlags |= SDL_WINDOW_OPENGL;
			// set opengl attributes
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
			std::cout << "[Driver] OpenGL renderer selected." << std::endl;
		} break;
		case VGL_RENDERER_VULKAN: {
			std::cerr << "[Driver] ERROR: Vulkan not implemented yet." << std::endl;
			return false;
		}
		default: {
			std::cerr << "[Driver] ERROR: No renderer selected." << std::endl;
			return false;
		}
	}

	// init sdl
	if (SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "[Driver] SDL could not init! SDL_Error: " << std::endl
		<< SDL_GetError() << std::endl;
		return false;
	}

	std::cout << "[Driver] OK: SDL init" << std::endl;

	// create sdl window
	m_window = SDL_CreateWindow(
		"Example",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_windowWidth, m_windowHeight,
		winFlags
	);
	cfg.pNativeWindowHandle = (void*)m_window;
	cfg.resolution = {m_windowWidth, m_windowHeight};

	// check window
	if(!m_window) {
        std::cerr << "[Driver] ERROR: Window could not be created! SDL_Error: " << std::endl
        << SDL_GetError() << std::endl;
		return false;
    }
	
	// create context (if opengl)
	if (cfg.rendererType == VGL_RENDERER_OPENGL) {
		SDL_GLContext ctx = SDL_GL_CreateContext(m_window);
		if (!ctx) {
			std::cerr << "[Driver] ERROR: Could not create OpenGL context: " << std::endl
			<< SDL_GetError() << std::endl;
			return false;
		}
		SDL_GL_MakeCurrent(m_window, ctx);
		cfg.pContext = (void*)ctx;
		std::cout << "[Driver] OK: VGL created OpenGL context" << std::endl;
	}

	// start vgl layer
	if (!vgl::init(cfg)) {
		std::cerr << "[Driver] ERROR: VGL could not init!" << std::endl;
		return false;
	}

	// get app from registry
	m_appIndex = _appIndex;
	m_app = App::getApp(m_appIndex);
	if (!m_app) {
		std::cerr << "[Driver] ERROR: Could not get app at index " << m_appIndex << std::endl;
		return false;
	}

	// init app
	if (!m_app->init()) {
		std::cerr << "[Driver] ERROR: App init failed." << std::endl;
		return false;
	}

	// init time
	m_lastCount = SDL_GetPerformanceCounter();
	if (s_startCount == 0) {
		Driver::s_startCount = m_lastCount;
	}

	std::cout << "[Driver] init end" << std::endl;

	return true;
}

void Driver::run() {
	std::cout << "[Driver] mainloop begin" << std::endl;
	// main loop
	bool isQuit = false;
	while (!isQuit) {

		// poll to quit
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				isQuit = true;
			} else if (event.type == SDL_WINDOWEVENT) {
				if (event.window.event == SDL_WINDOWEVENT_RESIZED || event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					m_windowWidth = event.window.data1;
					m_windowHeight = event.window.data2;
					// propogate window resize & swapchain to vgl
					vgl::window(m_windowWidth, m_windowHeight);
				}
			} else if (event.type == SDL_KEYDOWN) {
				// switch apps on number key press
				if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9) {
					int appIndex = event.key.keysym.sym - SDLK_0;
					if (appIndex != m_appIndex) {
						if (!switchApp(appIndex)) {
							std::cerr << "[Driver] ERROR: Failed to switch to app " << appIndex << std::endl;
						} else {
							std::cout << "[Driver] Switched to app " << appIndex << std::endl;
						}
					}
				}
			}
		}

		// calc delta time
 		uint64_t curCount = SDL_GetPerformanceCounter();
		double dt = (double)(curCount - m_lastCount) / (double)SDL_GetPerformanceFrequency();
		m_lastCount = curCount;

		// run app frame
		m_app->draw(dt);
		
		// swap buffers
		if (vgl::getRendererType() == VGL_RENDERER_OPENGL) {
			SDL_GL_SwapWindow(m_window);
		}

		// sleep
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	}
	std::cout << "[Driver] mainloop end" << std::endl;
}

void Driver::shutdown() {
	std::cout << "[Driver] shutdown begin " << std::endl;
	
	// destroy resources
	if (m_app != nullptr) {
		m_app->shutdown();
	}
	vgl::shutdown();
	SDL_DestroyWindow(m_window);
	SDL_Quit();
	
	std::cout << "[Driver] shutdown end " << std::endl;
};

bool Driver::switchApp(int _appIndex) {
	// get new app from registry
	App* newApp = App::getApp(_appIndex);
	if (!newApp) {
		std::cerr << "[Driver] ERROR: Could not get app at index " << _appIndex << std::endl;
		return false;
	}

	// shutdown current app
	if (!m_app) {
		m_app->shutdown();
	}
	
	// init new app
	m_appIndex = _appIndex;
	m_app = newApp;
	if (!m_app->init()) {
		m_app->shutdown();
		m_app = nullptr;
		m_appIndex = -1;
		std::cerr << "[Driver] ERROR: App init failed." << std::endl;
		return false;
	}

	std::cout << "[Driver] Switched to app " << m_appIndex << std::endl;
	return true;
}
