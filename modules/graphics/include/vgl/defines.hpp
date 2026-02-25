#pragma once

// std libs
#include <cstdint>
#include <cassert>

/// --- SUPPORT MACROS --- ///
// bitwise flag operations
#define _VGL_FLAG_DECODE(_type, _value, _mask, _shift) ( \
	(static_cast<_type>(_value) & _mask) >> _shift \
)
#define _VGL_FLAG_ENCODE(_type, _value, _mask, _shift) ( \
	(static_cast<_type>(_value) << _shift) & _mask \
)

/// --- RENDERER --- ///
typedef uint8_t RendererType;
#define VGL_RENDERER_NOOP                       UINT8_C(0x00)
#define VGL_RENDERER_OPENGL                     UINT8_C(0x01)
#define VGL_RENDERER_VULKAN                     UINT8_C(0x02)
#define VGL_RENDERER_COUNT                      UINT8_C(0x03)

/// --- MEMORY ACCESS --- ///
typedef uint8_t Access;
#define VGL_ACCESS_READ                         UINT8_C(0x00)
#define VGL_ACCESS_WRITE                        UINT8_C(0x01)
#define VGL_ACCESS_READ_WRITE                   UINT8_C(0x02)
#define VGL_ACCESS_COUNT                        UINT8_C(0x03)

/// --- BUFFER FLAGS --- ///
typedef uint8_t BufferFlags;
#define VGL_BUFFER_NONE        					UINT8_C(0x00)
#define VGL_BUFFER_INDEX32     					UINT8_C(0x01)

/// --- SHADER TYPE --- ///
typedef uint8_t ShaderType;
#define VGL_SHADER_TYPE_VERTEX					UINT8_C(0x00)
#define VGL_SHADER_TYPE_FRAGMENT				UINT8_C(0x01)
#define VGL_SHADER_TYPE_COUNT					UINT8_C(0x02)

/// --- SHADER ATTRIBUTE --- ///
typedef uint8_t Attrib;
#define VGL_ATTRIB_POSITION                     UINT8_C(0x00)
#define VGL_ATTRIB_NORMAL                       UINT8_C(0x01)
#define VGL_ATTRIB_TANGENT                      UINT8_C(0x02)
#define VGL_ATTRIB_BITANGENT                    UINT8_C(0x03)
#define VGL_ATTRIB_COLOR0                       UINT8_C(0x04)
#define VGL_ATTRIB_COLOR1                       UINT8_C(0x05)
#define VGL_ATTRIB_COLOR2                       UINT8_C(0x06)
#define VGL_ATTRIB_COLOR3                       UINT8_C(0x07)
#define VGL_ATTRIB_INDICES                      UINT8_C(0x08)
#define VGL_ATTRIB_WEIGHT                       UINT8_C(0x09)
#define VGL_ATTRIB_TEXCOORD0                    UINT8_C(0x0a)
#define VGL_ATTRIB_TEXCOORD1                    UINT8_C(0x0b)
#define VGL_ATTRIB_TEXCOORD2                    UINT8_C(0x0c)
#define VGL_ATTRIB_TEXCOORD3                    UINT8_C(0x0d)
#define VGL_ATTRIB_TEXCOORD4                    UINT8_C(0x0e)
#define VGL_ATTRIB_TEXCOORD5                    UINT8_C(0x0f)
#define VGL_ATTRIB_COUNT                        UINT8_C(0x10)

/// --- ATTRIBUTE TYPE --- ///
typedef uint8_t AttribType;
#define VGL_ATTRIB_TYPE_INT8                    UINT8_C(0x00)
#define VGL_ATTRIB_TYPE_INT16                   UINT8_C(0x01)
#define VGL_ATTRIB_TYPE_INT32                   UINT8_C(0x02)
#define VGL_ATTRIB_TYPE_UINT8                   UINT8_C(0x03)
#define VGL_ATTRIB_TYPE_UINT16                  UINT8_C(0x04)
#define VGL_ATTRIB_TYPE_UINT32                  UINT8_C(0x05)
#define VGL_ATTRIB_TYPE_HALF                    UINT8_C(0x06)
#define VGL_ATTRIB_TYPE_FLOAT                   UINT8_C(0x07)
#define VGL_ATTRIB_TYPE_DOUBLE                  UINT8_C(0x08)
#define VGL_ATTRIB_TYPE_COUNT                   UINT8_C(0x09)

/// --- TEXTURE FORMAT --- ///
typedef uint8_t TexFormat;
// r
#define VGL_TEXTURE_FORMAT_R8U                  UINT8_C(0x00)
#define VGL_TEXTURE_FORMAT_R8I                  UINT8_C(0x01)
#define VGL_TEXTURE_FORMAT_R16U                 UINT8_C(0x02)
#define VGL_TEXTURE_FORMAT_R16I                 UINT8_C(0x03)
#define VGL_TEXTURE_FORMAT_R16F                 UINT8_C(0x04)
#define VGL_TEXTURE_FORMAT_R32U                 UINT8_C(0x05)
#define VGL_TEXTURE_FORMAT_R32I                 UINT8_C(0x06)
#define VGL_TEXTURE_FORMAT_R32F                 UINT8_C(0x07)
// rg
#define VGL_TEXTURE_FORMAT_RG8U                 UINT8_C(0x08)
#define VGL_TEXTURE_FORMAT_RG8I                 UINT8_C(0x09)
#define VGL_TEXTURE_FORMAT_RG16U                UINT8_C(0x0a)
#define VGL_TEXTURE_FORMAT_RG16I                UINT8_C(0x0b)
#define VGL_TEXTURE_FORMAT_RG16F                UINT8_C(0x0c)
#define VGL_TEXTURE_FORMAT_RG32U                UINT8_C(0x0d)
#define VGL_TEXTURE_FORMAT_RG32I                UINT8_C(0x0e)
#define VGL_TEXTURE_FORMAT_RG32F                UINT8_C(0x0f)
// rgb
#define VGL_TEXTURE_FORMAT_RGB8U                UINT8_C(0x10)
#define VGL_TEXTURE_FORMAT_RGB8I                UINT8_C(0x11)
#define VGL_TEXTURE_FORMAT_RGB16U               UINT8_C(0x12)
#define VGL_TEXTURE_FORMAT_RGB16I               UINT8_C(0x13)
#define VGL_TEXTURE_FORMAT_RGB16F               UINT8_C(0x14)
#define VGL_TEXTURE_FORMAT_RGB32U               UINT8_C(0x15)
#define VGL_TEXTURE_FORMAT_RGB32I               UINT8_C(0x16)
#define VGL_TEXTURE_FORMAT_RGB32F               UINT8_C(0x17)
// rgba
#define VGL_TEXTURE_FORMAT_RGBA8U               UINT8_C(0x18)
#define VGL_TEXTURE_FORMAT_RGBA8I               UINT8_C(0x19)
#define VGL_TEXTURE_FORMAT_RGBA16U              UINT8_C(0x1a)
#define VGL_TEXTURE_FORMAT_RGBA16I              UINT8_C(0x1b)
#define VGL_TEXTURE_FORMAT_RGBA16F              UINT8_C(0x1c)
#define VGL_TEXTURE_FORMAT_RGBA32U              UINT8_C(0x1d)
#define VGL_TEXTURE_FORMAT_RGBA32I              UINT8_C(0x1e)
#define VGL_TEXTURE_FORMAT_RGBA32F              UINT8_C(0x1f)
// bgr
#define VGL_TEXTURE_FORMAT_BGR8U                UINT8_C(0x20)
#define VGL_TEXTURE_FORMAT_BGR8I                UINT8_C(0x21)
#define VGL_TEXTURE_FORMAT_BGR16U               UINT8_C(0x22)
#define VGL_TEXTURE_FORMAT_BGR16I               UINT8_C(0x23)
#define VGL_TEXTURE_FORMAT_BGR16F               UINT8_C(0x24)
#define VGL_TEXTURE_FORMAT_BGR32U               UINT8_C(0x25)
#define VGL_TEXTURE_FORMAT_BGR32I               UINT8_C(0x26)
#define VGL_TEXTURE_FORMAT_BGR32F               UINT8_C(0x27)
// bgra
#define VGL_TEXTURE_FORMAT_BGRA8U               UINT8_C(0x28)
#define VGL_TEXTURE_FORMAT_BGRA8I               UINT8_C(0x29)
#define VGL_TEXTURE_FORMAT_BGRA16U              UINT8_C(0x2a)
#define VGL_TEXTURE_FORMAT_BGRA16I              UINT8_C(0x2b)
#define VGL_TEXTURE_FORMAT_BGRA16F              UINT8_C(0x2c)
#define VGL_TEXTURE_FORMAT_BGRA32U              UINT8_C(0x2d)
#define VGL_TEXTURE_FORMAT_BGRA32I              UINT8_C(0x2e)
#define VGL_TEXTURE_FORMAT_BGRA32F              UINT8_C(0x2f)
// depth
#define VGL_TEXTURE_FORMAT_D16F                 UINT8_C(0x30)
#define VGL_TEXTURE_FORMAT_D24F                 UINT8_C(0x31)
#define VGL_TEXTURE_FORMAT_D32F                 UINT8_C(0x32)
// depth stencil
#define VGL_TEXTURE_FORMAT_D24FS8U              UINT8_C(0x33)
#define VGL_TEXTURE_FORMAT_D32FS8U              UINT8_C(0x34)
// stencil
#define VGL_TEXTURE_FORMAT_S8U				 	UINT8_C(0x35)
#define VGL_TEXTURE_FORMAT_S16U			 		UINT8_C(0x36)		
#define VGL_TEXTURE_FORMAT_COUNT                UINT8_C(0x37)

/// --- TEXTURE LAYOUT --- ///
typedef uint8_t TexLayout;
#define VGL_TEXTURE_LAYOUT_R					UINT8_C(0x00)
#define VGL_TEXTURE_LAYOUT_RG					UINT8_C(0x01)
#define VGL_TEXTURE_LAYOUT_RGB					UINT8_C(0x02)
#define VGL_TEXTURE_LAYOUT_RGBA					UINT8_C(0x03)
#define VGL_TEXTURE_LAYOUT_BGR					UINT8_C(0x04)
#define VGL_TEXTURE_LAYOUT_BGRA					UINT8_C(0x05)
#define VGL_TEXTURE_LAYOUT_DEPTH				UINT8_C(0x06)
#define VGL_TEXTURE_LAYOUT_DEPTH_STENCIL		UINT8_C(0x07)
#define VGL_TEXTURE_LAYOUT_STENCIL				UINT8_C(0x08)
#define VGL_TEXTURE_LAYOUT_COUNT				UINT8_C(0x09)

/// --- TEXTURE TYPE --- ///
typedef uint8_t TexTarget;
#define VGL_TEXTURE_TARGET_1D              		UINT8_C(0x00)
#define VGL_TEXTURE_TARGET_2D              		UINT8_C(0x01)
#define VGL_TEXTURE_TARGET_3D              		UINT8_C(0x02)
#define VGL_TEXTURE_TARGET_CUBE            		UINT8_C(0x03)
#define VGL_TEXTURE_TARGET_1D_ARRAY        		UINT8_C(0x04)
#define VGL_TEXTURE_TARGET_2D_ARRAY        		UINT8_C(0x05)
#define VGL_TEXTURE_TARGET_CUBE_ARRAY      		UINT8_C(0x06)
#define VGL_TEXTURE_TARGET_COUNT               	UINT8_C(0x07)

/// --- TEXTURE FACE --- ///
typedef uint8_t TexFace;
#define VGL_TEXTURE_FACE_POSITIVE_X          	UINT8_C(0x00)
#define VGL_TEXTURE_FACE_NEGATIVE_X          	UINT8_C(0x01)
#define VGL_TEXTURE_FACE_POSITIVE_Y          	UINT8_C(0x02)
#define VGL_TEXTURE_FACE_NEGATIVE_Y          	UINT8_C(0x03)
#define VGL_TEXTURE_FACE_POSITIVE_Z          	UINT8_C(0x04)
#define VGL_TEXTURE_FACE_NEGATIVE_Z          	UINT8_C(0x05)
#define VGL_TEXTURE_FACE_COUNT               	UINT8_C(0x06)

/// --- TEXTURE FILTER --- ///
typedef uint8_t TexFilter;
#define VGL_TEXTURE_FILTER_NEAREST				UINT8_C(0x00)
#define VGL_TEXTURE_FILTER_LINEAR				UINT8_C(0x01)
#define VGL_TEXTURE_FILTER_COUNT				UINT8_C(0x02)

/// --- TEXTURE WRAP --- ///
typedef uint8_t TexWrap;
#define VGL_TEXTURE_WRAP_REPEAT					UINT8_C(0x00)
#define VGL_TEXTURE_WRAP_MIRROR					UINT8_C(0x01)
#define VGL_TEXTURE_WRAP_CLAMP					UINT8_C(0x02)
#define VGL_TEXTURE_WRAP_BORDER					UINT8_C(0x03)
#define VGL_TEXTURE_WRAP_COUNT					UINT8_C(0x04)

typedef uint8_t TexCompare;
#define VGL_TEXTURE_COMPARE_NONE				UINT8_C(0x00)
#define VGL_TEXTURE_COMPARE_LESS				UINT8_C(0x01)
#define VGL_TEXTURE_COMPARE_LEQUAL				UINT8_C(0x02)
#define VGL_TEXTURE_COMPARE_EQUAL				UINT8_C(0x03)
#define VGL_TEXTURE_COMPARE_GEQUAL				UINT8_C(0x04)
#define VGL_TEXTURE_COMPARE_GREATER				UINT8_C(0x05)
#define VGL_TEXTURE_COMPARE_NOTEQUAL			UINT8_C(0x06)
#define VGL_TEXTURE_COMPARE_NEVER				UINT8_C(0x07)
#define VGL_TEXTURE_COMPARE_ALWAYS				UINT8_C(0x08)
#define VGL_TEXTURE_COMPARE_COUNT				UINT8_C(0x09)

/// --- TEXTURE FLAGS --- ///
typedef uint64_t TexFlags;
// texture compare (shadow mapping)
#define VGL_TEXTURE_COMPARE_SHIFT				0
#define VGL_TEXTURE_COMPARE_MASK				UINT64_C(0x000000000000000f)
#define VGL_TEXTURE_COMPARE(_v)					_VGL_FLAG_ENCODE( 		\
	TexFlags, _v, VGL_TEXTURE_COMPARE_MASK, VGL_TEXTURE_COMPARE_SHIFT 	\
)
// texture wrapping
#define VGL_TEXTURE_WRAP_U_SHIFT				4
#define VGL_TEXTURE_WRAP_U_MASK					UINT64_C(0x00000000000000f0)
#define VGL_TEXTURE_WRAP_U(_v)					_VGL_FLAG_ENCODE(		\
	TexFlags, _v, VGL_TEXTURE_WRAP_U_MASK, VGL_TEXTURE_WRAP_U_SHIFT		\
)
#define VGL_TEXTURE_WRAP_V_SHIFT				8
#define VGL_TEXTURE_WRAP_V_MASK					UINT64_C(0x0000000000000f00)
#define VGL_TEXTURE_WRAP_V(_v)					_VGL_FLAG_ENCODE(		\
	TexFlags, _v, VGL_TEXTURE_WRAP_V_MASK, VGL_TEXTURE_WRAP_V_SHIFT		\
)
#define VGL_TEXTURE_WRAP_W_SHIFT				12
#define VGL_TEXTURE_WRAP_W_MASK					UINT64_C(0x000000000000f000)
#define VGL_TEXTURE_WRAP_W(_v)					_VGL_FLAG_ENCODE(		\
	TexFlags, _v, VGL_TEXTURE_WRAP_W_MASK, VGL_TEXTURE_WRAP_W_SHIFT		\
)
#define VGL_TEXTURE_WRAP(_v)					( 	\
	VGL_TEXTURE_WRAP_U(_v)							\
	| VGL_TEXTURE_WRAP_V(_v) 						\
	| VGL_TEXTURE_WRAP_W(_v)						\
)
// texture filtering
#define VGL_TEXTURE_FILTER_MAG_SHIFT			16
#define VGL_TEXTURE_FILTER_MAG_MASK				UINT64_C(0x00000000000f0000)
#define VGL_TEXTURE_FILTER_MAG(_v)				(	\
	TexFlags, _v, VGL_TEXTURE_FILTER_MAG_MASK, VGL_TEXTURE_FILTER_MAG_SHIFT \
)
#define VGL_TEXTURE_FILTER_MIN_SHIFT			20
#define VGL_TEXTURE_FILTER_MIN_MASK				UINT64_C(0x0000000000f00000)
#define VGL_TEXTURE_FILTER_MIN(_v)				(	\
	TexFlags, _v, VGL_TEXTURE_FILTER_MIN_MASK, VGL_TEXTURE_FILTER_MIN_SHIFT \
)
#define VGL_TEXTURE_FILTER_MIPMAP_SHIFT			24
#define VGL_TEXTURE_FILTER_MIPMAP_MASK			UINT64_C(0x000000000f000000)
#define VGL_TEXTURE_FILTER_MIPMAP(_v)			(	\
	TexFlags, _v, VGL_TEXTURE_FILTER_MIPMAP_MASK, VGL_TEXTURE_FILTER_MIPMAP_SHIFT \
)
#define VGL_TEXTURE_FILTER(_v)					(	\
	VGL_TEXTURE_FILTER_MAG(_v)		\
	| VGL_TEXTURE_FILTER_MIN(_v)	\
	| VGL_TEXTURE_FILTER_MIPMAP(_v)	\
)
// border colour
#define VGL_TEXTURE_BORDER_COLOUR_SHIFT			32
#define VGL_TEXTURE_BORDER_COLOUR_MASK			UINT32_C(0xffffffff00000000)
#define VGL_TEXTURE_BORDER_COLOUR(_v)			_VGL_FLAG_ENCODE( \
	TexFlags, _v, VGL_TEXTURE_BORDER_COLOUR_MASK, VGL_TEXTURE_BORDER_COLOUR_SHIFT \
)
#define VGL_TEXTURE_DEFAULT						UINT64_C(0x333333ff00000000)

/// --- TEXTURE MIP MAP --- ///
typedef uint8_t ResolveFlags;
#define VGL_RESOLVE_NONE					 	UINT8_C(0x00)
#define VGL_RESOLVE_AUTO_GEN_MIPS				UINT8_C(0x01)

/// --- FRAMEBUFFER ATTACHMENTS --- ///
typedef uint8_t AttachmentSlot;
#define VGL_ATTACHMENT_COLOUR0					UINT8_C(0x00)
#define VGL_ATTACHMENT_COLOUR1					UINT8_C(0x01)
#define VGL_ATTACHMENT_COLOUR2					UINT8_C(0x02)
#define VGL_ATTACHMENT_COLOUR3					UINT8_C(0x03)
#define VGL_ATTACHMENT_COLOUR4					UINT8_C(0x04)
#define VGL_ATTACHMENT_COLOUR5					UINT8_C(0x05)
#define VGL_ATTACHMENT_COLOUR6					UINT8_C(0x06)
#define VGL_ATTACHMENT_COLOUR7					UINT8_C(0x07)
#define VGL_ATTACHMENT_DEPTH					UINT8_C(0x08)
#define VGL_ATTACHMENT_STENCIL					UINT8_C(0x09)
#define VGL_ATTACHMENT_DEPTH_STENCIL			UINT8_C(0x0a)

/// --- TEXTURE ATTACHMENT TYPES --- ///
typedef uint8_t AttachmentType;
#define VGL_ATTACHMENT_TYPE_COLOUR				UINT8_C(0x00)
#define VGL_ATTACHMENT_TYPE_DEPTH				UINT8_C(0x01)
#define VGL_ATTACHMENT_TYPE_STENCIL				UINT8_C(0x02)
#define VGL_ATTACHMENT_TYPE_DEPTH_STENCIL 		UINT8_C(0x03)
#define VGL_ATTACHMENT_TYPE_COUNT 				UINT8_C(0x04)

/// TODO: extend texture properties
// - texture border colour
// - texture comparisons

/// --- UNIFORM TYPE --- ///
// note: assume float unless prefaced (I)nterger or (U)nsigned
typedef uint8_t UniformType;
// scalars
#define VGL_UNIFORM_TYPE_INT32                  UINT8_C(0x00)
#define VGL_UNIFORM_TYPE_UINT32                 UINT8_C(0x01)
#define VGL_UNIFORM_TYPE_FLOAT                  UINT8_C(0x02)
#define VGL_UNIFORM_TYPE_DOUBLE                 UINT8_C(0x03)
// vectors
#define VGL_UNIFORM_TYPE_IVEC2                  UINT8_C(0x04)
#define VGL_UNIFORM_TYPE_IVEC3                  UINT8_C(0x05)
#define VGL_UNIFORM_TYPE_IVEC4                  UINT8_C(0x06)
#define VGL_UNIFORM_TYPE_UVEC2                  UINT8_C(0x07)
#define VGL_UNIFORM_TYPE_UVEC3                  UINT8_C(0x08)
#define VGL_UNIFORM_TYPE_UVEC4                  UINT8_C(0x09)
#define VGL_UNIFORM_TYPE_VEC2                   UINT8_C(0x0a)
#define VGL_UNIFORM_TYPE_VEC3                   UINT8_C(0x0b)
#define VGL_UNIFORM_TYPE_VEC4                   UINT8_C(0x0c)
#define VGL_UNIFORM_TYPE_DVEC2                  UINT8_C(0x0d)
#define VGL_UNIFORM_TYPE_DVEC3                  UINT8_C(0x0e)
#define VGL_UNIFORM_TYPE_DVEC4                  UINT8_C(0x0f)
// matrices
#define VGL_UNIFORM_TYPE_MAT2                   UINT8_C(0x10)
#define VGL_UNIFORM_TYPE_MAT2X3                 UINT8_C(0x11)
#define VGL_UNIFORM_TYPE_MAT2X4                 UINT8_C(0x12)
#define VGL_UNIFORM_TYPE_MAT3X2                 UINT8_C(0x13)
#define VGL_UNIFORM_TYPE_MAT3                   UINT8_C(0x14)
#define VGL_UNIFORM_TYPE_MAT3X4                 UINT8_C(0x15)
#define VGL_UNIFORM_TYPE_MAT4X2                 UINT8_C(0x16)
#define VGL_UNIFORM_TYPE_MAT4X3                 UINT8_C(0x17)
#define VGL_UNIFORM_TYPE_MAT4                   UINT8_C(0x18)
// colour textures
#define VGL_UNIFORM_TYPE_SAMPLER_1D             UINT8_C(0x19)
#define VGL_UNIFORM_TYPE_SAMPLER_2D             UINT8_C(0x1a)
#define VGL_UNIFORM_TYPE_SAMPLER_3D             UINT8_C(0x1b)
#define VGL_UNIFORM_TYPE_SAMPLER_CUBE           UINT8_C(0x1c)
#define VGL_UNIFORM_TYPE_SAMPLER_2D_RECT        UINT8_C(0x1d)
#define VGL_UNIFORM_TYPE_SAMPLER_1D_ARRAY       UINT8_C(0x1e)
#define VGL_UNIFORM_TYPE_SAMPLER_2D_ARRAY       UINT8_C(0x1f)
#define VGL_UNIFORM_TYPE_SAMPLER_CUBE_ARRAY     UINT8_C(0x20)
#define VGL_UNIFORM_TYPE_SAMPLER_BUFFER         UINT8_C(0x21)
// shadow textures
#define VGL_UNIFORM_TYPE_SAMPLER_1D_SHADOW      UINT8_C(0x22)
#define VGL_UNIFORM_TYPE_SAMPLER_2D_SHADOW      UINT8_C(0x23)
#define VGL_UNIFORM_TYPE_SAMPLER_CUBE_SHADOW    UINT8_C(0x24)
#define VGL_UNIFORM_TYPE_SAMPLER_2D_RECT_SHADOW UINT8_C(0x25)
#define VGL_UNIFORM_TYPE_SAMPLER_1D_ARRAY_SHADOW UINT8_C(0x26)
#define VGL_UNIFORM_TYPE_SAMPLER_2D_ARRAY_SHADOW UINT8_C(0x27)
#define VGL_UNIFORM_TYPE_SAMPLER_CUBE_ARRAY_SHADOW UINT8_C(0x28)
#define VGL_UNIFORM_TYPE_COUNT                  UINT8_C(0x29)

/// --- PRIMITIVE TOPOLOGY --- ///
typedef uint8_t PrimTopology;
#define VGL_PRIM_TOPOLOGY_POINT_LIST            UINT8_C(0x00)
#define VGL_PRIM_TOPOLOGY_LINE_LIST             UINT8_C(0x01)
#define VGL_PRIM_TOPOLOGY_LINE_STRIP            UINT8_C(0x02)
#define VGL_PRIM_TOPOLOGY_TRIANGLE_LIST         UINT8_C(0x03)
#define VGL_PRIM_TOPOLOGY_TRIANGLE_STRIP        UINT8_C(0x04)
#define VGL_PRIM_TOPOLOGY_TRIANGLE_FAN          UINT8_C(0x05)
#define VGL_PRIM_TOPOLOGY_COUNT                 UINT8_C(0x06)

/// TODO: delete??
// /// --- FACES --- ///
// typedef uint8_t FacesEnum;
// #define VGL_FACE_NONE							UINT8_C(0x00)
// #define VGL_FACE_FRONT							UINT8_C(0x01)
// #define VGL_FACE_BACK							UINT8_C(0x02)
// #define VGL_FACE_FRONT_AND_BACK					UINT8_C(0x03)
// #define VGL_FACE_COUNT							UINT8_C(0x04)

/// --- STENCIL FLAGS --- ///s
typedef uint32_t StencilFlags;
// macro operations
#define VGL_STENCIL_DECODE(_v, _m, _s)			_VGL_FLAG_DECODE(StencilFlags, _v, _m, _s)
#define VGL_STENCIL_ENCODE(_v, _m, _s) 			_VGL_FLAG_ENCODE(StencilFlags, _v, _m, _s)
// reference value
#define VGL_STENCIL_FUNC_RVAL_SHIFT				0
#define VGL_STENCIL_FUNC_RVAL_MASK				UINT32_C(0x00000000000000ff)
#define VGL_STENCIL_FUNC_RVAL(_v)				VGL_STENCIL_ENCODE( \
	_v, VGL_STENCIL_FUNC_RVAL_MASK, VGL_STENCIL_FUNC_RVAL_SHIFT \
)
// write mask
#define VGL_STENCIL_FUNC_WMASK_SHIFT			8
#define VGL_STENCIL_FUNC_WMASK_MASK				UINT32_C(0x000000000000ff00)
#define VGL_STENCIL_FUNC_WMASK(_v)				VGL_STENCIL_ENCODE( \
	_v, VGL_STENCIL_FUNC_WMASK_MASK, VGL_STENCIL_FUNC_WMASK_SHIFT \
)
// fragment test
#define VGL_STENCIL_TEST_DISABLE				UINT32_C(0x00000000)
#define VGL_STENCIL_TEST_LESS					UINT32_C(0x00010000)
#define VGL_STENCIL_TEST_LEQUAL					UINT32_C(0x00020000)
#define VGL_STENCIL_TEST_EQUAL					UINT32_C(0x00030000)
#define VGL_STENCIL_TEST_GEQUAL					UINT32_C(0x00040000)
#define VGL_STENCIL_TEST_GREATER				UINT32_C(0x00050000)
#define VGL_STENCIL_TEST_NOTEQUAL				UINT32_C(0x00060000)
#define VGL_STENCIL_TEST_NEVER					UINT32_C(0x00070000)
#define VGL_STENCIL_TEST_ALWAYS					UINT32_C(0x00080000)
#define VGL_STENCIL_TEST_SHIFT					16
#define VGL_STENCIL_TEST_MASK					UINT32_C(0x000f0000)
#define VGL_STENCIL_TEST_DEFAULT				VGL_STENCIL_TEST_DISABLE
// operation on stencil test fail
#define VGL_STENCIL_OP_FAIL_S_ZERO    			UINT32_C(0x00000000)
#define VGL_STENCIL_OP_FAIL_S_KEEP    			UINT32_C(0x00100000)
#define VGL_STENCIL_OP_FAIL_S_REPLACE 			UINT32_C(0x00200000)
#define VGL_STENCIL_OP_FAIL_S_INCR    			UINT32_C(0x00300000)
#define VGL_STENCIL_OP_FAIL_S_INCRSAT 			UINT32_C(0x00400000)
#define VGL_STENCIL_OP_FAIL_S_DECR    			UINT32_C(0x00500000)
#define VGL_STENCIL_OP_FAIL_S_DECRSAT 			UINT32_C(0x00600000)
#define VGL_STENCIL_OP_FAIL_S_INVERT  			UINT32_C(0x00700000)
#define VGL_STENCIL_OP_FAIL_S_SHIFT   			20
#define VGL_STENCIL_OP_FAIL_S_MASK    			UINT32_C(0x00f00000)
// operation on depth test fail
#define VGL_STENCIL_OP_FAIL_Z_ZERO    			UINT32_C(0x00000000)
#define VGL_STENCIL_OP_FAIL_Z_KEEP    			UINT32_C(0x01000000)
#define VGL_STENCIL_OP_FAIL_Z_REPLACE 			UINT32_C(0x02000000)
#define VGL_STENCIL_OP_FAIL_Z_INCR    			UINT32_C(0x03000000)
#define VGL_STENCIL_OP_FAIL_Z_INCRSAT 			UINT32_C(0x04000000)
#define VGL_STENCIL_OP_FAIL_Z_DECR    			UINT32_C(0x05000000)
#define VGL_STENCIL_OP_FAIL_Z_DECRSAT 			UINT32_C(0x06000000)
#define VGL_STENCIL_OP_FAIL_Z_INVERT  			UINT32_C(0x07000000)
#define VGL_STENCIL_OP_FAIL_Z_SHIFT   			24
#define VGL_STENCIL_OP_FAIL_Z_MASK    			UINT32_C(0x0f000000)
// operation on stencil & depth tests pass
#define VGL_STENCIL_OP_PASS_SZ_ZERO    			UINT32_C(0x00000000)
#define VGL_STENCIL_OP_PASS_SZ_KEEP    			UINT32_C(0x10000000)
#define VGL_STENCIL_OP_PASS_SZ_REPLACE 			UINT32_C(0x20000000)
#define VGL_STENCIL_OP_PASS_SZ_INCR    			UINT32_C(0x30000000)
#define VGL_STENCIL_OP_PASS_SZ_INCRSAT 			UINT32_C(0x40000000)
#define VGL_STENCIL_OP_PASS_SZ_DECR    			UINT32_C(0x50000000)
#define VGL_STENCIL_OP_PASS_SZ_DECRSAT 			UINT32_C(0x60000000)
#define VGL_STENCIL_OP_PASS_SZ_INVERT  			UINT32_C(0x70000000)
#define VGL_STENCIL_OP_PASS_SZ_SHIFT   			20
#define VGL_STENCIL_OP_PASS_SZ_MASK    			UINT32_C(0xf0000000)
// default values
#define VGL_STENCIL_NONE						UINT32_C(0x00000000)
#define VGL_STENCIL_DEFAULT						VGL_STENCIL_NONE

/// --- VIEW CLEAR --- ///
/// TODO: add per attachment clear
typedef uint8_t ClearFlags;
#define VGL_CLEAR_NONE							UINT8_C(0x00)
#define VGL_CLEAR_COLOUR  						UINT8_C(0x01)
#define VGL_CLEAR_DEPTH   						UINT8_C(0x02)
#define VGL_CLEAR_STENCIL 						UINT8_C(0x04)
#define VGL_CLEAR_ALL							UINT8_C(0x07)

/// --- DEPTH TEST --- ///
typedef uint8_t DepthTest;
#define VGL_DEPTH_TEST_DISABLED            		UINT8_C(0x00)
#define VGL_DEPTH_TEST_LESS               		UINT8_C(0x01)
#define VGL_DEPTH_TEST_LEQUAL             		UINT8_C(0x02)
#define VGL_DEPTH_TEST_EQUAL              		UINT8_C(0x03)
#define VGL_DEPTH_TEST_GEQUAL             		UINT8_C(0x04)
#define VGL_DEPTH_TEST_GREATER            		UINT8_C(0x05)
#define VGL_DEPTH_TEST_NOTEQUAL           		UINT8_C(0x06)
#define VGL_DEPTH_TEST_NEVER              		UINT8_C(0x07)
#define VGL_DEPTH_TEST_ALWAYS             		UINT8_C(0x08)
#define VGL_DEPTH_TEST_COUNT					UINT8_C(0x09)

/// --- CULL TEST --- ///
typedef uint8_t CullTest;
#define VGL_CULL_TEST_DISABLED					UINT8_C(0x00)
#define VGL_CULL_TEST_CW						UINT8_C(0x01)
#define VGL_CULL_TEST_CCW						UINT8_C(0x02)
#define VGL_CULL_TEST_COUNT						UINT8_C(0x03)

/// --- PRIMITIVE --- ///
typedef uint8_t PrimitiveType;
#define VGL_PT_POINTS                  			UINT8_C(0x00)
#define VGL_PT_LINE_STRIP              			UINT8_C(0x01)
#define VGL_PT_LINE_LOOP               			UINT8_C(0x02)
#define VGL_PT_LINES                   			UINT8_C(0x03)
#define VGL_PT_LINE_STRIP_ADJACENCY    			UINT8_C(0x04)
#define VGL_PT_LINES_ADJACENCY         			UINT8_C(0x05)
#define VGL_PT_TRIANGLE_STRIP          			UINT8_C(0x06)
#define VGL_PT_TRIANGLE_FAN            			UINT8_C(0x07)
#define VGL_PT_TRIANGLES               			UINT8_C(0x08)
#define VGL_PT_TRIANGLE_STRIP_ADJACENCY			UINT8_C(0x09)
#define VGL_PT_TRIANGLES_ADJACENCY     			UINT8_C(0x0a)
#define VGL_PT_COUNT							UINT8_C(0x0b)

/// --- POLYGON --- ///
/// TODO:
// - always front and back alway same from GL 3+ (capability/feature)
// - rework as wireframe mode i.e. fusion360
typedef uint8_t Polygon;
#define VGL_POLYGON_FILL						UINT8_C(0x00)
#define VGL_POLYGON_LINE						UINT8_C(0x01)
#define VGL_POLYGON_POINT						UINT8_C(0x02)
#define VGL_POLYGON_COUNT						UINT8_C(0x03)


/// --- STATE FLAGS --- ///
typedef uint64_t StateFlags;
// macro operations
#define VGL_STATE_DECODE(_v, _m, _s)			_VGL_FLAG_DECODE(StateFlags, _v, _m, _s)
#define VGL_STATE_ENCODE(_v, _m, _s) 			_VGL_FLAG_ENCODE(StateFlags, _v, _m, _s)
// depth test enum
#define VGL_STATE_DEPTH_TEST_DISABLED			UINT64_C(0x0000000000000000)
#define VGL_STATE_DEPTH_TEST_LESS               UINT64_C(0x0000000000000001)
#define VGL_STATE_DEPTH_TEST_LEQUAL             UINT64_C(0x0000000000000002)
#define VGL_STATE_DEPTH_TEST_EQUAL              UINT64_C(0x0000000000000003)
#define VGL_STATE_DEPTH_TEST_GEQUAL             UINT64_C(0x0000000000000004)
#define VGL_STATE_DEPTH_TEST_GREATER            UINT64_C(0x0000000000000005)
#define VGL_STATE_DEPTH_TEST_NOTEQUAL           UINT64_C(0x0000000000000006)
#define VGL_STATE_DEPTH_TEST_NEVER              UINT64_C(0x0000000000000007)
#define VGL_STATE_DEPTH_TEST_ALWAYS             UINT64_C(0x0000000000000008)
#define VGL_STATE_DEPTH_TEST_SHIFT              0
#define VGL_STATE_DEPTH_TEST_MASK               UINT64_C(0x000000000000000f)
#define VGL_STATE_DEPTH_TEST_DEFAULT			VGL_STATE_ENCODE( \
	VGL_DEPTH_TEST_LESS, \
	VGL_STATE_DEPTH_TEST_MASK, \
	VGL_STATE_DEPTH_TEST_SHIFT \
)
// face culling (enum)
#define VGL_STATE_CULL_TEST_DISABLED			UINT64_C(0x0000000000000000)
#define VGL_STATE_CULL_TEST_CW					UINT64_C(0x0000000000000010)
#define VGL_STATE_CULL_TEST_CCW					UINT64_C(0x0000000000000020)
#define VGL_STATE_CULL_TEST_SHIFT				4
#define VGL_STATE_CULL_TEST_MASK				UINT64_C(0x00000000000000f0)
#define VGL_STATE_CULL_TEST_DEFAULT				VGL_STATE_ENCODE( \
	VGL_CULL_TEST_CW, \
	VGL_STATE_CULL_TEST_MASK, \
	VGL_STATE_CULL_TEST_SHIFT \
)
// primitive type (enum)
#define VGL_STATE_PT_POINTS                  	UINT64_C(0x0000000000000000)
#define VGL_STATE_PT_LINE_STRIP              	UINT64_C(0x0000000000000100)
#define VGL_STATE_PT_LINE_LOOP               	UINT64_C(0x0000000000000200)
#define VGL_STATE_PT_LINES                   	UINT64_C(0x0000000000000300)
#define VGL_STATE_PT_LINE_STRIP_ADJACENCY    	UINT64_C(0x0000000000000400)
#define VGL_STATE_PT_LINES_ADJACENCY         	UINT64_C(0x0000000000000500)
#define VGL_STATE_PT_TRIANGLE_STRIP          	UINT64_C(0x0000000000000600)
#define VGL_STATE_PT_TRIANGLE_FAN            	UINT64_C(0x0000000000000700)
#define VGL_STATE_PT_TRIANGLES               	UINT64_C(0x0000000000000800)
#define VGL_STATE_PT_TRIANGLE_STRIP_ADJACENCY	UINT64_C(0x0000000000000900)
#define VGL_STATE_PT_TRIANGLES_ADJACENCY     	UINT64_C(0x0000000000000a00)
#define VGL_STATE_PT_SHIFT                      8
#define VGL_STATE_PT_MASK                       UINT64_C(0x0000000000000f00)
#define VGL_STATE_PT_DEFAULT					VGL_STATE_ENCODE( \
	VGL_PT_TRIANGLES, \
	VGL_STATE_PT_MASK, \
	VGL_STATE_PT_SHIFT \
)
// polygon wireframe (flags)
#define VGL_STATE_POLYGON_FILL					UINT64_C(0x0000000000000000)
#define VGL_STATE_POLYGON_LINE					UINT64_C(0x0000000000001000)
#define VGL_STATE_POLYGON_POINT					UINT64_C(0x0000000000002000)
#define VGL_STATE_POLYGON_COUNT					UINT64_C(0x0000000000003000)
#define VGL_STATE_POLYGON_SHIFT					12
#define VGL_STATE_POLYGON_MASK					UINT64_C(0x000000000000f000)
#define VGL_STATE_POLYGON_DEFAULT				VGL_STATE_ENCODE( \
	VGL_POLYGON_FILL, \
	VGL_STATE_POLYGON_MASK, \
	VGL_STATE_POLYGON_SHIFT \
)
// point size (byte)
#define VGL_STATE_POINT_SIZE_SHIFT              16
#define VGL_STATE_POINT_SIZE_MASK               UINT64_C(0x0000000000ff0000)
#define VGL_STATE_POINT_SIZE_DEFAULT			UINT64_C(0x0000000000010000)
#define VGL_STATE_POINT_SIZE(_v)				VGL_STATE_ENCODE( \
	_v, VGL_STATE_POINT_SIZE_MASK, VGL_STATE_POINT_SIZE_SHIFT \
)
// line width (byte)
#define VGL_STATE_LINE_WIDTH_SHIFT              24
#define VGL_STATE_LINE_WIDTH_MASK               UINT64_C(0x00000000ff000000)
#define VGL_STATE_LINE_WIDTH_DEFAULT            UINT64_C(0x0000000001000000)
#define VGL_STATE_LINE_WIDTH(_v)				VGL_STATE_ENCODE( \
	_v, VGL_STATE_LINE_WIDTH_MASK, VGL_STATE_LINE_WIDTH_SHIFT \
)
// default params
#define VGL_STATE_MASK							UINT64_C(0x00000000ffffffff)
#define VGL_STATE_DEFAULT						( \
	VGL_STATE_DEPTH_TEST_DEFAULT \
	| VGL_STATE_CULL_TEST_DEFAULT \
	| VGL_STATE_PT_TRIANGLES \
	| VGL_STATE_POLYGON_DEFAULT \
	| VGL_STATE_POINT_SIZE_DEFAULT \
	| VGL_STATE_LINE_WIDTH_DEFAULT \
)

/// TODO:
// enable/disable write r/g/b/z

/// --- MAX LIMITS --- ///
// api context
#define VGL_MAX_DRAW_CALLS                      65535
#define VGL_MIN_RESOURCE_COMMAND_BUFFER_SIZE    65536
#define VGL_MAX_ENCODERS						16
// resources
#define VGL_MAX_PROGRAMS						256
#define VGL_MAX_VIEWS                       	256
#define VGL_MAX_VERTEX_LAYOUTS              	64
#define VGL_MAX_INDEX_BUFFERS               	4096
#define VGL_MAX_VERTEX_BUFFERS              	4096
#define VGL_MAX_VERTEX_STREAMS              	4
#define VGL_MAX_SHADERS                     	512
#define VGL_MAX_TEXTURES                    	4096
#define VGL_MAX_TEXTURE_UNITS 		   			16
#define VGL_MAX_TEXTURE_SAMPLERS            	16
#define VGL_MAX_FRAME_BUFFERS               	128
#define VGL_MAX_COLOUR_ATTACHMENTS		    	8
#define VGL_MAX_UNIFORMS                    	32	// 512 per shader?
// string buffers
#define VGL_MAX_UNIFORM_NAME_LENGTH             128
// attributes
#define VGL_MAX_ATTRIBUTES						16

/// --- DISCARD --- ///
typedef uint8_t DiscardFlags;
#define VGL_DISCARD_NONE               			UINT8_C(0x00)
#define VGL_DISCARD_VERTEX_STREAMS     			UINT8_C(0x01)
#define VGL_DISCARD_INDEX_BUFFER       			UINT8_C(0x02)
#define VGL_DISCARD_STATE              			UINT8_C(0x04)
#define VGL_DISCARD_UNIFORMS					UINT8_C(0x08)
#define VGL_DISCARD_TEXTURES					UINT8_C(0x10)
#define VGL_DISCARD_ALL                			UINT8_C(0xff)

/// --- VIEW PIPELINE --- ///
typedef uint16_t ViewID;

/// --- HANDLE MACROS --- ///
// note: 	handles must be wrapped in structs to allow for type differentiating,
// 			this allows for function overloading
#define VGL_HANDLE(_name)						\
	struct _name { 								\
		uint16_t idx; 							\
		static _name invalid() { return { vgl::kInvalidHandle }; } \
	};											\
	inline bool isValid(_name _handle) { return vgl::kInvalidHandle != _handle.idx; }
#define VGL_INVALID_HANDLE 						{ vgl::kInvalidHandle }

/// --- ASSERTS --- ///
#define VGL_ASSERT(_cond, _msg)					assert((_cond) && (_msg));
