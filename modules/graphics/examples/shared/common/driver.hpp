#pragma once

// std lib
#include <iostream>
// internal
#include "vgl/vgl.hpp"
// external
#include "SDL2/SDL.h"

// forward declare App
class App;

class Driver {
private:
	// SDL
	SDL_Window* m_window = nullptr;
	uint16_t m_windowWidth = 600;
	uint16_t m_windowHeight = 400;
	uint64_t m_lastCount = 0;
	static uint64_t s_startCount;
	// apps
	int m_appIndex = -1;
	App* m_app = nullptr;

public:
	Driver() = default;

	bool init(int _appIndex, RendererType _type);
	void run();
	void shutdown();

	bool switchApp(int _appIndex);

	static double getTime();

};
