#pragma once

// std lib
#include <string>

namespace util {

	/* --- TRIANGLE */
	constexpr uint32_t kNumTriangleVertexComponents = 5;
	constexpr uint32_t kNumTriangleVertices = 3;
	constexpr uint32_t kNumTriangleVertexElements = 15;
	constexpr uint32_t kNumTriangleIndices = 3;
	// triangle mesh data of vertices x, y, z
	constexpr float kTriangleVertices[kNumTriangleVertexElements] = {
		0.0f,  1.0f, 0.0f, 0.5f, 1.0f,	// top
	   -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,	// bottom-left
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f	// bottom-right
	};
	constexpr uint16_t kTriangleIndices[kNumTriangleIndices] = {
		0, 1, 2
	};

    /* --- QUAD --- */
	constexpr uint32_t kNumQuadVertexComponents = 5;
	constexpr uint32_t kNumQuadVertices = 4;
	constexpr uint32_t kNumQuadVertexElements = 20;
	constexpr uint32_t kNumQuadIndices = 6;
	// quad mesh data of vertices x, y, z, u, v
	constexpr float kQuadVertices[kNumQuadVertexElements] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,	// bottom-left
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f	// top-left
	};
	constexpr uint16_t kQuadIndices[kNumQuadIndices] = {
		0, 1, 2,
		2, 3, 0
	};
	
	/* --- CUBE --- */
	constexpr uint32_t kNumCubeVertexComponents = 3;
	constexpr uint32_t kNumCubeVertices = 8;
	constexpr uint32_t kNumCubeVertexElements = 24;
	constexpr uint32_t kNumCubeIndices = 36;
	// cube mesh data of vertices x, y, z
	constexpr float kCubeVertices[kNumCubeVertexElements] = {
		// positions
		 1.0f, -1.0f, -1.0f,	// 0
		-1.0f, -1.0f, -1.0f,	// 1
		-1.0f,  1.0f, -1.0f,	// 2
		 1.0f,  1.0f, -1.0f,	// 3
		 1.0f, -1.0f,  1.0f,	// 4
		-1.0f, -1.0f,  1.0f,	// 5
		-1.0f,  1.0f,  1.0f,	// 6
		 1.0f,  1.0f,  1.0f 	// 7
	};
	constexpr uint32_t kCubeIndices[kNumCubeIndices] = {
		0, 1, 2, 2, 3, 0, // -Z
		1, 5, 6, 6, 2, 1, // +X
		5, 4, 7, 7, 6, 5, // +Z
		4, 0, 3, 3, 7, 4, // -X
		3, 2, 6, 6, 7, 3, // +Y
		4, 5, 1, 1, 0, 4  // -Y
	};

    /* --- LOAD --- */
    std::string loadTextFile(const std::string& _path);

} // namespace util