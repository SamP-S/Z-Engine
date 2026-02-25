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

class TriangleApp : public App{
private:

	// resource file paths
	const char* m_vertPath 	= "apps/01_triangle/shaders/triangle.vert";
	const char* m_fragPath 	= "apps/01_triangle/shaders/triangle.frag";

	// shader source c-strings
	std::string m_vertStr = "";
	std::string m_fragStr = "";

	// layouts
	VertexLayout m_posLayout;
	
	// handles
	VertexLayoutHandle m_hPosLayout = { VGL_INVALID_HANDLE };
	VertexBufferHandle m_hTriVertexBuffer = { VGL_INVALID_HANDLE };
	IndexBufferHandle m_hTriIndexBuffer = { VGL_INVALID_HANDLE };
	ShaderHandle m_hVertShader = { VGL_INVALID_HANDLE };
	ShaderHandle m_hFragShader = { VGL_INVALID_HANDLE };
	ProgramHandle m_hProgram = { VGL_INVALID_HANDLE };

	// draw methods
	void drawTriangle();

public:
	TriangleApp();

	bool init() override;
	void draw(double _dt) override;
	void shutdown() override;

};
