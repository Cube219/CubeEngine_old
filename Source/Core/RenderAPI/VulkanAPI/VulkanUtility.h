#pragma once

#include<vulkan\vulkan.h>

#ifdef _DEBUG

#include "EngineCore/LogWriter.h"
void PrintVkFail(const char* fileName, int lineNum, cube::String msg, VkResult res);
#define CheckVkResult(msg, res) if(res != VK_SUCCESS) PrintVkFail(__FILE__, __LINE__, CUBE_T(msg), res);
	
#else // _DEBUG

#define PrintLog(msg) 
#define PrintlnLog(msg) 
#define PrintLogWithSayer(sayer, msg) 
#define PrintlnLogWithSayer(sayer, msg) 

#define CheckVkResult(sayer, msg, res) 

#endif // _DEBUG

#include "BaseRenderAPI/BaseRenderUtility.h"

namespace cube
{
	namespace core
	{
		VkFormat GetVkFormat(DataFormat format);
		VkPrimitiveTopology GetVkPrimitiveTopology(VertexTopology topology);
		VkViewport GetVkViewport(Viewport viewport);
		VkRect2D GetVkRect2D(Rect2D rect);
		VkPolygonMode GetVkPolygonMode(PolygonMode polygonMode);
		VkFrontFace GetVkFrontFace(PolygonFrontFace frontFace);
		VkCullModeFlags GetVkCullModeFlags(CullMode cullMode);
		VkCompareOp GetVkCompareOp(CompareOperator compareOperator);
		VkStencilOpState GetVkStencilOpState(StencilOperatorState operatorState);
		VkStencilOp GetVkStencilOp(StencilOperator op);
		VkBlendFactor GetVkBlendFactor(BlendFactor blendFactor);
		VkBlendOp GetVkBlendOp(BlendOperator blendOperator);
		VkLogicOp GetVkLogicOp(LogicOperator logicOperator);
		VkClearColorValue GetVkClearColorValue(Color color);
		VkClearDepthStencilValue GetVkClearDepthStencilValue(DepthStencilValue value);
		VkAccessFlags GetVkAccessFlags(AccessBits access);
		VkShaderStageFlags GetVkShaderStageFlags(ShaderTypeBits shaderType);
	}
}