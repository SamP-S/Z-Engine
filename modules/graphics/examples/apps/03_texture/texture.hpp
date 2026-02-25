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

class TextureApp : public App{
private:

	// resource file paths
	const char* m_vertPath 	= "apps/03_texture/shaders/texture.vert";
	const char* m_fragPath 	= "apps/03_texture/shaders/texture.frag";
	const char* m_tex0Path = "shared/assets/textures/wood_color_1k.jpg";
	const char* m_tex1Path = "shared/assets/textures/wood_preview.jpg";

	// shader source c-strings
	std::string m_vertStr = "";
	std::string m_fragStr = "";

	// layouts
	VertexLayout m_posTexLayout;
	
	// handles
	VertexLayoutHandle 	m_hPosTexLayout = { VGL_INVALID_HANDLE };
	VertexBufferHandle 	m_hQuadVertexBuffer = { VGL_INVALID_HANDLE };
	IndexBufferHandle 	m_hQuadIndexBuffer = { VGL_INVALID_HANDLE };
	ShaderHandle 		m_hVertShader = { VGL_INVALID_HANDLE };
	ShaderHandle 		m_hFragShader = { VGL_INVALID_HANDLE };
	ProgramHandle 		m_hProgram = { VGL_INVALID_HANDLE };
	TextureHandle 		m_hTexture0 = { VGL_INVALID_HANDLE };
	TextureHandle 		m_hTexture1 = { VGL_INVALID_HANDLE };

	// uniforms
	LA::mat4 m_view;
	LA::mat4 m_proj;

	// draw methods
	void drawTexQuad(LA::mat4 _model, TextureHandle _hTex);

public:
	TextureApp();

	bool init() override;
	void draw(double _dt) override;
	void shutdown() override;

};
