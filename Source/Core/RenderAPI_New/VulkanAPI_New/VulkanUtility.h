#pragma once

#include <vulkan/vulkan.h>

#include "Base/String.h"

#ifdef _DEBUG

namespace cube
{
	namespace render
	{
		void PrintVkFailImpl(VkResult res, const char* fileName, int lineNum, const String& msg);
		void PrintVkFailImpl(VkResult res, const char* fileName, int lineNum, String&& msg);

		template <typename ...Args>
		void PrintVkFail(VkResult res, const char* fileName, int lineNum, const String& msg, Args&&... args)
		{
			PrintVkFailImpl(res, fileName, lineNum, fmt::format(msg, std::forward<Args>(args)...));
		}

		template <typename ...Args>
		void PrintVkFail(VkResult res, const char* fileName, int lineNum, const Character* msg, Args&&... args)
		{
			PrintVkFailImpl(res, fileName, lineNum, fmt::format(msg, std::forward<Args>(args)...));
		}

		String GetVkResult(VkResult res);
	} // namespace render
} // namespace cube

#define CHECK_VK(res, msg, ...) if(res != VK_SUCCESS) cube::render::PrintVkFail(res, __FILE__, __LINE__, CUBE_T(msg), ##__VA_ARGS__)

#else // _DEBUG

#define CHECK_VK(res, msg, ...)

#endif // _DEBUG
