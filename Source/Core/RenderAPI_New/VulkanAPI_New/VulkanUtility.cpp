#include "VulkanUtility.h"

#ifdef _DEBUG

#include "EngineCore/Assertion.h"

namespace cube
{
	namespace render
	{
		void PrintVkFailImpl(VkResult res, const char* fileName, int lineNum, const String& msg)
		{
			ASSERTION_FAILED("{0} {1}", msg, GetVkResult(res));
		}

		void PrintVkFailImpl(VkResult res, const char* fileName, int lineNum, String&& msg)
		{
			PrintVkFailImpl(res, fileName, lineNum, msg);
		}

		String GetVkResult(VkResult res)
		{
			switch(res) {
			case VK_ERROR_INITIALIZATION_FAILED:
				return CUBE_T("(VK_ERROR_INITIALIZATION_FAILED)");
			case VK_ERROR_DEVICE_LOST:
				return CUBE_T("(VK_ERROR_DEVICE_LOST)");
			case VK_ERROR_MEMORY_MAP_FAILED:
				return CUBE_T("(VK_ERROR_MEMORY_MAP_FAILED)");
			case VK_ERROR_LAYER_NOT_PRESENT:
				return CUBE_T("(VK_ERROR_LAYER_NOT_PRESENT)");
			case VK_ERROR_EXTENSION_NOT_PRESENT:
				return CUBE_T("(VK_ERROR_EXTENSION_NOT_PRESENT)");
			case VK_ERROR_FEATURE_NOT_PRESENT:
				return CUBE_T("(VK_ERROR_FEATURE_NOT_PRESENT)");
			case VK_ERROR_INCOMPATIBLE_DRIVER:
				return CUBE_T("(VK_ERROR_INCOMPATIBLE_DRIVER)");
			case VK_ERROR_TOO_MANY_OBJECTS:
				return CUBE_T("(VK_ERROR_TOO_MANY_OBJECTS)");
			case VK_ERROR_FORMAT_NOT_SUPPORTED:
				return CUBE_T("(VK_ERROR_FORMAT_NOT_SUPPORTED)");
			case VK_ERROR_FRAGMENTED_POOL:
				return CUBE_T("(VK_ERROR_FRAGMENTED_POOL)");
			case VK_ERROR_SURFACE_LOST_KHR:
				return CUBE_T("(VK_ERROR_SURFACE_LOST_KHR)");
			case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
				return CUBE_T("(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR)");
			case VK_ERROR_OUT_OF_DATE_KHR:
				return CUBE_T("(VK_ERROR_OUT_OF_DATE_KHR)");
			case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
				return CUBE_T("(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR)");
			case VK_ERROR_VALIDATION_FAILED_EXT:
				return CUBE_T("(VK_ERROR_VALIDATION_FAILED_EXT)");
			case VK_ERROR_INVALID_SHADER_NV:
				return CUBE_T("(VK_ERROR_INVALID_SHADER_NV)");
			case VK_ERROR_OUT_OF_POOL_MEMORY_KHR:
				return CUBE_T("(VK_ERROR_OUT_OF_POOL_MEMORY_KHR)");
			case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR:
				return CUBE_T("(VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR)");
			default:
				return CUBE_T("(UNKNOWN)");
			}
		}
	} // namespace render
} // namespace cube

#endif // _DEBUG
