#include "GLSLTool.h"

#include <SPIRV/GlslangToSpv.h>

#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		void GLSLTool::Init()
		{
			glslang::InitializeProcess();
		}

		void GLSLTool::Finalize()
		{
			glslang::FinalizeProcess();
		}

		Vector<Uint32> GLSLTool::ToSPIRV(ShaderType type, const char* code)
		{
			EShLanguage shLang;

			switch(type) {
				case ShaderType::Vertex:   shLang = EShLangVertex; break;
				case ShaderType::Pixel:    shLang = EShLangFragment; break;
				case ShaderType::Geometry: shLang = EShLangGeometry; break;
				case ShaderType::Hull:     shLang = EShLangTessControl; break;
				case ShaderType::Domain:   shLang = EShLangTessEvaluation; break;
				case ShaderType::Compute:  shLang = EShLangCompute; break;
				default: ASSERTION_FAILED("Unknown shader type ({0}).", (Uint32)type);
			}

			glslang::TShader shader(shLang);
			TBuiltInResource resources = InitResources();
			EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

			shader.setStrings(&code, 1);
			bool res = shader.parse(&resources, 100, false, messages);
			CHECK(res, "Failed to parse glsl to spir-v. ({0} / {1})", shader.getInfoLog(), shader.getInfoDebugLog());

			glslang::TProgram program;
			program.addShader(&shader);
			res = program.link(messages);
			CHECK(res, "Failed to link program. ({0} / {1})", shader.getInfoLog(), shader.getInfoDebugLog());

			Vector<Uint32> spirv;
			glslang::GlslangToSpv(*program.getIntermediate(shLang), spirv);

			return spirv;
		}

		TBuiltInResource GLSLTool::InitResources()
		{
			TBuiltInResource resources;

			resources.maxLights = 32;
			resources.maxClipPlanes = 6;
			resources.maxTextureUnits = 32;
			resources.maxTextureCoords = 32;
			resources.maxVertexAttribs = 64;
			resources.maxVertexUniformComponents = 4096;
			resources.maxVaryingFloats = 64;
			resources.maxVertexTextureImageUnits = 32;
			resources.maxCombinedTextureImageUnits = 80;
			resources.maxTextureImageUnits = 32;
			resources.maxFragmentUniformComponents = 4096;
			resources.maxDrawBuffers = 32;
			resources.maxVertexUniformVectors = 128;
			resources.maxVaryingVectors = 8;
			resources.maxFragmentUniformVectors = 16;
			resources.maxVertexOutputVectors = 16;
			resources.maxFragmentInputVectors = 15;
			resources.minProgramTexelOffset = -8;
			resources.maxProgramTexelOffset = 7;
			resources.maxClipDistances = 8;
			resources.maxComputeWorkGroupCountX = 65535;
			resources.maxComputeWorkGroupCountY = 65535;
			resources.maxComputeWorkGroupCountZ = 65535;
			resources.maxComputeWorkGroupSizeX = 1024;
			resources.maxComputeWorkGroupSizeY = 1024;
			resources.maxComputeWorkGroupSizeZ = 64;
			resources.maxComputeUniformComponents = 1024;
			resources.maxComputeTextureImageUnits = 16;
			resources.maxComputeImageUniforms = 8;
			resources.maxComputeAtomicCounters = 8;
			resources.maxComputeAtomicCounterBuffers = 1;
			resources.maxVaryingComponents = 60;
			resources.maxVertexOutputComponents = 64;
			resources.maxGeometryInputComponents = 64;
			resources.maxGeometryOutputComponents = 128;
			resources.maxFragmentInputComponents = 128;
			resources.maxImageUnits = 8;
			resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
			resources.maxCombinedShaderOutputResources = 8;
			resources.maxImageSamples = 0;
			resources.maxVertexImageUniforms = 0;
			resources.maxTessControlImageUniforms = 0;
			resources.maxTessEvaluationImageUniforms = 0;
			resources.maxGeometryImageUniforms = 0;
			resources.maxFragmentImageUniforms = 8;
			resources.maxCombinedImageUniforms = 8;
			resources.maxGeometryTextureImageUnits = 16;
			resources.maxGeometryOutputVertices = 256;
			resources.maxGeometryTotalOutputComponents = 1024;
			resources.maxGeometryUniformComponents = 1024;
			resources.maxGeometryVaryingComponents = 64;
			resources.maxTessControlInputComponents = 128;
			resources.maxTessControlOutputComponents = 128;
			resources.maxTessControlTextureImageUnits = 16;
			resources.maxTessControlUniformComponents = 1024;
			resources.maxTessControlTotalOutputComponents = 4096;
			resources.maxTessEvaluationInputComponents = 128;
			resources.maxTessEvaluationOutputComponents = 128;
			resources.maxTessEvaluationTextureImageUnits = 16;
			resources.maxTessEvaluationUniformComponents = 1024;
			resources.maxTessPatchComponents = 120;
			resources.maxPatchVertices = 32;
			resources.maxTessGenLevel = 64;
			resources.maxViewports = 16;
			resources.maxVertexAtomicCounters = 0;
			resources.maxTessControlAtomicCounters = 0;
			resources.maxTessEvaluationAtomicCounters = 0;
			resources.maxGeometryAtomicCounters = 0;
			resources.maxFragmentAtomicCounters = 8;
			resources.maxCombinedAtomicCounters = 8;
			resources.maxAtomicCounterBindings = 1;
			resources.maxVertexAtomicCounterBuffers = 0;
			resources.maxTessControlAtomicCounterBuffers = 0;
			resources.maxTessEvaluationAtomicCounterBuffers = 0;
			resources.maxGeometryAtomicCounterBuffers = 0;
			resources.maxFragmentAtomicCounterBuffers = 1;
			resources.maxCombinedAtomicCounterBuffers = 1;
			resources.maxAtomicCounterBufferSize = 16384;
			resources.maxTransformFeedbackBuffers = 4;
			resources.maxTransformFeedbackInterleavedComponents = 64;
			resources.maxCullDistances = 8;
			resources.maxCombinedClipAndCullDistances = 8;
			resources.maxSamples = 4;
			resources.limits.nonInductiveForLoops = 1;
			resources.limits.whileLoops = 1;
			resources.limits.doWhileLoops = 1;
			resources.limits.generalUniformIndexing = 1;
			resources.limits.generalAttributeMatrixVectorIndexing = 1;
			resources.limits.generalVaryingIndexing = 1;
			resources.limits.generalSamplerIndexing = 1;
			resources.limits.generalVariableIndexing = 1;
			resources.limits.generalConstantMatrixVectorIndexing = 1;

			return resources;
		}
	} // namespace render
} // namespace cube
