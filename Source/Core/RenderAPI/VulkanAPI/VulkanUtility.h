#pragma once

#include<vulkan\vulkan.h>

#ifdef _DEBUG
// TODO: platform의 logWriter하고 통합
#include<iostream>

#define PrintLog(msg) std::wcout << msg;
#define PrintlnLog(msg) std::wcout << msg << std::endl;
#define PrintLogWithSayer(sayer, msg) std::wcout << sayer << " : " << msg;
#define PrintlnLogWithSayer(sayer, msg) std::wcout << sayer << " : " << msg << std::endl;

const wchar_t* _CheckVkResult(const wchar_t* sayer, const wchar_t* msg, VkResult res);

#define CheckVkResult(sayer, msg, res) _CheckVkResult(sayer, msg,  res)
	
#else // _DEBUG

#define PrintLog(msg) 
#define PrintlnLog(msg) 
#define PrintLogWithSayer(sayer, msg) 
#define PrintlnLogWithSayer(sayer, msg) 

#define CheckVkResult(sayer, msg, res) 

#endif // _DEBUG

#include "BaseRenderAPI\BaseRenderUtility.h"

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
	}
}