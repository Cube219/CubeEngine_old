#pragma once

#include <vulkan/vulkan.h>

#include "EngineCore/LogWriter.h"

#ifdef _DEBUG

void PrintVkFail(const char* fileName, int lineNum, cube::String msg, VkResult res);
#define CheckVkResult(msg, res) if(res != VK_SUCCESS) PrintVkFail(__FILE__, __LINE__, CUBE_T(msg), res);

#else // _DEBUG

#define CheckVkResult(msg, res)

#endif // _DEBUG