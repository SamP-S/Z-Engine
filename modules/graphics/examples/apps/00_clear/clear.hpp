#pragma once

// std lib
#include <iostream>
#include <string>
// internal
#include "common/app.hpp"
#include "vgl/vgl.hpp"
// external
#include "la/la_extended.h"

using namespace vgl;

class ClearApp : public App {
public:
	ClearApp();

	bool init() override;
	void draw(double _dt) override;
	void shutdown() override;

};
