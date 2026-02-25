#include "vgl/g_structs.hpp"

// std lib
#include <memory>
#include <cstring>
#include <algorithm>
// external
#include "spdlog/spdlog.h"

/*
Vertex Layout:
1. It must encode a vertex buffer's data layout to the correct binding points.
2. It must track the relevant offsets and sizes of each attribute
3. It must track the total stride
*/

namespace vgl {

const std::unordered_map<AttribType, uint8_t> VertexLayout::kAttribTypeSizeMap = {
	{VGL_ATTRIB_TYPE_INT8, 		1},
	{VGL_ATTRIB_TYPE_INT16, 	2},
	{VGL_ATTRIB_TYPE_INT32, 	4},
	{VGL_ATTRIB_TYPE_UINT8, 	1},
	{VGL_ATTRIB_TYPE_UINT16, 	2},
	{VGL_ATTRIB_TYPE_UINT32, 	4},
	{VGL_ATTRIB_TYPE_HALF, 		2},
	{VGL_ATTRIB_TYPE_FLOAT, 	4},
	{VGL_ATTRIB_TYPE_DOUBLE, 	8}
};

float Resolution::aspect() const {
	if (height == 0) {
		return 0.0f;
	}
	return (float)width / (float)height;
}

VertexLayout::VertexLayout()
	: m_stride(0), m_numAttributes(0), 
	m_indexes({-1}), m_finished(false) {}

VertexLayout::~VertexLayout() {}

VertexLayout& VertexLayout::begin() {
	m_stride = 0;
	m_numAttributes = 0;
	m_finished = false;
	memset(m_indexes, -1, sizeof(m_indexes));

	return *this;
}

void VertexLayout::end() {
	m_finished = true;
}

VertexLayout& VertexLayout::add(
	Attrib _attrib,
	AttribType _type,
	uint8_t	_numComponents,
	uint8_t _numElements,
	bool _normalised,
	bool _optional
) {
	// check valid
	if (_type >= VGL_ATTRIB_TYPE_COUNT) {
		spdlog::error("No attribute type ({})");
		return *this;
	}
	// limit to min 1
	_numComponents = std::max(uint8_t(1), _numComponents);
	_numElements = std::max(uint8_t(1), _numElements);
	// set va struct
	VertexAttribute va;
	va.attrib = _attrib;
	va.offset = m_stride;
	va.type = _type;
	va.size = kAttribTypeSizeMap.at(_type) * _numComponents * _numElements;
	va.numComponents = _numComponents;
	va.numElements = _numElements;
	va.normalised = _normalised;
	va.optional = _optional;
	// push to array, set index, update stride
	m_attributes[m_numAttributes] = va;
	m_indexes[va.attrib] = m_numAttributes;
	m_numAttributes += 1;
	m_stride += va.size;
}

VertexLayout& VertexLayout::skip(uint8_t _num) {
	m_stride += _num;

	return *this;
}

bool VertexLayout::has(Attrib _attrib) const {
	return !(m_indexes[_attrib] < 0);
}

uint16_t VertexLayout::getStride() const {
	return m_stride;
}

const VertexAttribute* VertexLayout::getAttributes(uint8_t& _numAttributes) {
	_numAttributes = m_numAttributes;
	return m_attributes;
}

} // namespace vgl
