﻿#include "ShaderVk.h"

#include "DeviceVk.h"
#include "../Tools/GLSLTool.h"
#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		ShaderVk::ShaderVk(DeviceVk& device, const ShaderAttribute& attr)
		{
			mType = attr.type;

			switch(attr.language) {
				case ShaderLanguage::GLSL:  LoadFromGLSL(attr); break;
				case ShaderLanguage::HLSL:  LoadFromHLSL(attr); break;
				case ShaderLanguage::SPIRV: LoadFromSPIRV(attr); break;
				default: ASSERTION_FAILED("Unknown shader language ({0}).", (Uint32)attr.type);
			}

			VkShaderModuleCreateInfo info;
			info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			info.pNext = nullptr;
			info.flags = 0;
			info.codeSize = mSPIRV.size() * sizeof(Uint32);
			info.pCode = mSPIRV.data();

			mShaderModule = device.GetLogicalDevice()->CreateVkShaderModuleWrapper(info, attr.debugName);

			// Create pipeline shader stage info
			mShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			mShaderStageInfo.pNext = nullptr;
			mShaderStageInfo.flags = 0;
			mShaderStageInfo.pSpecializationInfo = nullptr;

			switch(attr.type) {
				case ShaderType::Vertex:   mShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; break;
				case ShaderType::Pixel:    mShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT; break;
				case ShaderType::Geometry: mShaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT; break;
				case ShaderType::Hull:     mShaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT; break;
				case ShaderType::Domain:   mShaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT; break;
				case ShaderType::Compute:  mShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT; break;
				default: ASSERTION_FAILED("Unknown shader type ({0}).", (Uint32)attr.type);
			}

			mShaderStageInfo.pName = attr.entryPoint;
			mShaderStageInfo.module = mShaderModule.mObject;
		}

		ShaderVk::~ShaderVk()
		{
			mShaderModule.Release();
		}

		void ShaderVk::LoadFromGLSL(const ShaderAttribute& attr)
		{
			mSPIRV = GLSLTool::ToSPIRV(attr.type, attr.code);
		}

		void ShaderVk::LoadFromHLSL(const ShaderAttribute& attr)
		{
			ASSERTION_FAILED("Loading HLSL shader is not supported yet.");
		}

		void ShaderVk::LoadFromSPIRV(const ShaderAttribute& attr)
		{
			ASSERTION_FAILED("Loading SPIR-V shader is not supported yet.");
		}
	} // namespace render
} // namespace cube