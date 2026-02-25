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

class CubeApp : public App{
private:

	// resource file paths
	const char* m_vertPath 	= "apps/02_cube/shaders/cube.vert";
	const char* m_fragPath 	= "apps/02_cube/shaders/cube.frag";

	// shader source c-strings
	std::string m_vertStr = "";
	std::string m_fragStr = "";

	// layouts
	VertexLayout m_posLayout;
	
	// handles
	VertexLayoutHandle m_hPosLayout = { VGL_INVALID_HANDLE };
	VertexBufferHandle m_hCubeVertexBuffer = { VGL_INVALID_HANDLE };
	IndexBufferHandle m_hCubeIndexBuffer = { VGL_INVALID_HANDLE };
	ShaderHandle m_hVertShader = { VGL_INVALID_HANDLE };
	ShaderHandle m_hFragShader = { VGL_INVALID_HANDLE };
	ProgramHandle m_hProgram = { VGL_INVALID_HANDLE };

	// uniforms
	LA::mat4 m_view;
	LA::mat4 m_proj;

	// draw methods
	void drawCube(LA::mat4 _model, LA::vec4 _colour);

public:
	CubeApp();

	bool init() override;
	void draw(double _dt) override;
	void shutdown() override;

};
