#include "VulkanShader.h"

#include "EngineCore/Assertion.h"
#include "VulkanDevice.h"

namespace cube
{
	namespace render
	{
		VulkanShader::VulkanShader(const SPtr<VulkanDevice>& device, ShaderInitializer& initializer) : 
			mDevice_ref(device)
		{
			VkShaderStageFlags stageFlagBits = GetVkShaderStageFlags(initializer.type);

			switch(initializer.language) {
				case ShaderLanguage::GLSL:
					LoadFromGLSL(initializer, stageFlagBits);
					break;

				case ShaderLanguage::SPIR_V:
					LoadFromSPIR_V(initializer);
					break;

				default:
					ASSERTION_FAILED("Unsupported shader language ({0})", (int)initializer.language);
					return;
			}

			// Create a module
			VkShaderModuleCreateInfo moduleInfo = {};
			moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleInfo.pNext = nullptr;
			moduleInfo.flags = 0;
			moduleInfo.codeSize = mSpvShader.size() * sizeof(unsigned int);
			moduleInfo.pCode = mSpvShader.data();

			VkResult res;
			res = vkCreateShaderModule(device->GetHandle(), &moduleInfo, nullptr, &mShaderModule);
			CheckVkResult("Cannot create a shader module", res);

			mEntryName = initializer.entryPoint;

			// Create stage info
			mShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			mShaderStageInfo.pNext = nullptr;
			mShaderStageInfo.flags = 0;
			mShaderStageInfo.pSpecializationInfo = nullptr;
			mShaderStageInfo.stage = (VkShaderStageFlagBits)stageFlagBits;
			mShaderStageInfo.pName = mEntryName.c_str();
			mShaderStageInfo.module = mShaderModule;
		}

		VulkanShader::~VulkanShader()
		{
			vkDestroyShaderModule(mDevice_ref->GetHandle(), mShaderModule, nullptr);

			mSpvShader.clear();
		}

		void VulkanShader::LoadFromSPIR_V(ShaderInitializer& initializer)
		{
			// TODO
		}

		void VulkanShader::LoadFromGLSL(ShaderInitializer& initializer, VkShaderStageFlags stageFlagBits)
		{
			TBuiltInResource resources = InitResource();

			EShLanguage type;

			switch(stageFlagBits) {
				case VK_SHADER_STAGE_VERTEX_BIT:
					type = EShLangVertex;
					break;
				case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
					type = EShLangTessControl;
					break;
				case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
					type = EShLangTessEvaluation;
					break;
				case VK_SHADER_STAGE_GEOMETRY_BIT:
					type = EShLangGeometry;
					break;
				case VK_SHADER_STAGE_FRAGMENT_BIT:
					type = EShLangFragment;
					break;
				case VK_SHADER_STAGE_COMPUTE_BIT:
					type = EShLangCompute;
					break;

				default:
					type = EShLangVertex;
					break;
			}
			glslang::InitializeProcess();

			glslang::TProgram program;
			glslang::TShader shader(type);
			shader.setStrings(&initializer.code, 1);
			shader.setEntryPoint(initializer.entryPoint);

			EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);
			bool res = shader.parse(&resources, 100, false, messages);
			CHECK(res, "Failed to parse the glsl to spir-v. ({0})", shader.getInfoLog());

			program.addShader(&shader);

			res = program.link(messages);
			CHECK(res, "Failed to link the glsl to spir-v. ({0})", shader.getInfoLog());

			glslang::GlslangToSpv(*program.getIntermediate(type), mSpvShader);

			glslang::FinalizeProcess();
		}

		TBuiltInResource VulkanShader::InitResource()
		{
			TBuiltInResource res;

			res.maxLights = 32;
			res.maxClipPlanes = 6;
			res.maxTextureUnits = 32;
			res.maxTextureCoords = 32;
			res.maxVertexAttribs = 64;
			res.maxVertexUniformComponents = 4096;
			res.maxVaryingFloats = 64;
			res.maxVertexTextureImageUnits = 32;
			res.maxCombinedTextureImageUnits = 80;
			res.maxTextureImageUnits = 32;
			res.maxFragmentUniformComponents = 4096;
			res.maxDrawBuffers = 32;
			res.maxVertexUniformVectors = 128;
			res.maxVaryingVectors = 8;
			res.maxFragmentUniformVectors = 16;
			res.maxVertexOutputVectors = 16;
			res.maxFragmentInputVectors = 15;
			res.minProgramTexelOffset = -8;
			res.maxProgramTexelOffset = 7;
			res.maxClipDistances = 8;
			res.maxComputeWorkGroupCountX = 65535;
			res.maxComputeWorkGroupCountY = 65535;
			res.maxComputeWorkGroupCountZ = 65535;
			res.maxComputeWorkGroupSizeX = 1024;
			res.maxComputeWorkGroupSizeY = 1024;
			res.maxComputeWorkGroupSizeZ = 64;
			res.maxComputeUniformComponents = 1024;
			res.maxComputeTextureImageUnits = 16;
			res.maxComputeImageUniforms = 8;
			res.maxComputeAtomicCounters = 8;
			res.maxComputeAtomicCounterBuffers = 1;
			res.maxVaryingComponents = 60;
			res.maxVertexOutputComponents = 64;
			res.maxGeometryInputComponents = 64;
			res.maxGeometryOutputComponents = 128;
			res.maxFragmentInputComponents = 128;
			res.maxImageUnits = 8;
			res.maxCombinedImageUnitsAndFragmentOutputs = 8;
			res.maxCombinedShaderOutputResources = 8;
			res.maxImageSamples = 0;
			res.maxVertexImageUniforms = 0;
			res.maxTessControlImageUniforms = 0;
			res.maxTessEvaluationImageUniforms = 0;
			res.maxGeometryImageUniforms = 0;
			res.maxFragmentImageUniforms = 8;
			res.maxCombinedImageUniforms = 8;
			res.maxGeometryTextureImageUnits = 16;
			res.maxGeometryOutputVertices = 256;
			res.maxGeometryTotalOutputComponents = 1024;
			res.maxGeometryUniformComponents = 1024;
			res.maxGeometryVaryingComponents = 64;
			res.maxTessControlInputComponents = 128;
			res.maxTessControlOutputComponents = 128;
			res.maxTessControlTextureImageUnits = 16;
			res.maxTessControlUniformComponents = 1024;
			res.maxTessControlTotalOutputComponents = 4096;
			res.maxTessEvaluationInputComponents = 128;
			res.maxTessEvaluationOutputComponents = 128;
			res.maxTessEvaluationTextureImageUnits = 16;
			res.maxTessEvaluationUniformComponents = 1024;
			res.maxTessPatchComponents = 120;
			res.maxPatchVertices = 32;
			res.maxTessGenLevel = 64;
			res.maxViewports = 16;
			res.maxVertexAtomicCounters = 0;
			res.maxTessControlAtomicCounters = 0;
			res.maxTessEvaluationAtomicCounters = 0;
			res.maxGeometryAtomicCounters = 0;
			res.maxFragmentAtomicCounters = 8;
			res.maxCombinedAtomicCounters = 8;
			res.maxAtomicCounterBindings = 1;
			res.maxVertexAtomicCounterBuffers = 0;
			res.maxTessControlAtomicCounterBuffers = 0;
			res.maxTessEvaluationAtomicCounterBuffers = 0;
			res.maxGeometryAtomicCounterBuffers = 0;
			res.maxFragmentAtomicCounterBuffers = 1;
			res.maxCombinedAtomicCounterBuffers = 1;
			res.maxAtomicCounterBufferSize = 16384;
			res.maxTransformFeedbackBuffers = 4;
			res.maxTransformFeedbackInterleavedComponents = 64;
			res.maxCullDistances = 8;
			res.maxCombinedClipAndCullDistances = 8;
			res.maxSamples = 4;
			res.limits.nonInductiveForLoops = 1;
			res.limits.whileLoops = 1;
			res.limits.doWhileLoops = 1;
			res.limits.generalUniformIndexing = 1;
			res.limits.generalAttributeMatrixVectorIndexing = 1;
			res.limits.generalVaryingIndexing = 1;
			res.limits.generalSamplerIndexing = 1;
			res.limits.generalVariableIndexing = 1;
			res.limits.generalConstantMatrixVectorIndexing = 1;

			return res;
		}
	} // namespace render
} // namespace cube
