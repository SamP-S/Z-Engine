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

class FramebufferApp : public App{
private:

	// resource file paths
	const char* m_vertPath 	= "apps/04_framebuffer/shaders/framebuffer.vert";
	const char* m_posFragPath 	= "apps/04_framebuffer/shaders/position.frag";
	const char* m_texFragPath = "apps/04_framebuffer/shaders/texture.frag";
	const char* m_tex0Path = "shared/assets/textures/wood_color_1k.jpg";

	// shader source c-strings
	std::string m_vertStr = "";
	std::string m_posFragStr = "";
	std::string m_texFragStr = "";

	// layouts
	VertexLayout 		m_posLayout;
	VertexLayout 		m_posTexLayout;
	VertexLayoutHandle 	m_hPosLayout = { VGL_INVALID_HANDLE };
	VertexLayoutHandle 	m_hPosTexLayout = { VGL_INVALID_HANDLE };
	// cube handles
	VertexBufferHandle 	m_hCubeVertexBuffer = { VGL_INVALID_HANDLE };
	IndexBufferHandle	m_hCubeIndexBuffer = { VGL_INVALID_HANDLE };
	// triangle handles
	VertexBufferHandle 	m_hTriVertexBuffer = { VGL_INVALID_HANDLE };
	IndexBufferHandle 	m_hTriIndexBuffer = { VGL_INVALID_HANDLE };
	// shader handles
	ShaderHandle 		m_hVertShader = { VGL_INVALID_HANDLE };
	ShaderHandle 		m_hPosFragShader = { VGL_INVALID_HANDLE };
	ShaderHandle		m_hTexFragShader = { VGL_INVALID_HANDLE };
	ProgramHandle 		m_hPosProgram = { VGL_INVALID_HANDLE };
	ProgramHandle		m_hTexProgram = { VGL_INVALID_HANDLE };
	// texture
	TextureHandle 		m_hTexture0 = { VGL_INVALID_HANDLE };
	// framebuffers
	TextureHandle		m_hFBColourTex = { VGL_INVALID_HANDLE };
	TextureHandle 		m_hFBDepthStencilTex = { VGL_INVALID_HANDLE };
	FrameBufferHandle 	m_hFrameBuffer = { VGL_INVALID_HANDLE };

	// uniforms
	LA::mat4 m_view;
	LA::mat4 m_proj;

	// draw methods
	void drawCube(LA::mat4 _model);
	void drawTexTri(LA::mat4 _model, TextureHandle _hTex);

public:
	FramebufferApp();

	bool init() override;
	void draw(double _dt) override;
	void shutdown() override;

};
