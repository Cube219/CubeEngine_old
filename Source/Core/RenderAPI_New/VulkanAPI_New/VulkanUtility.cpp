#include "VulkanUtility.h"

#ifdef _DEBUG

void PrintVkFail(const char* fileName, int lineNum, cube::String msg, VkResult res)
{
	cube::String errMsg;

	switch (res) {
	case VK_ERROR_INITIALIZATION_FAILED:
		errMsg = CUBE_T("(VK_ERROR_INITIALIZATION_FAILED)");
		break;
	case VK_ERROR_DEVICE_LOST:
		errMsg = CUBE_T("(VK_ERROR_DEVICE_LOST)");
		break;
	case VK_ERROR_MEMORY_MAP_FAILED:
		errMsg = CUBE_T("(VK_ERROR_MEMORY_MAP_FAILED)");
		break;
	case VK_ERROR_LAYER_NOT_PRESENT:
		errMsg = CUBE_T("(VK_ERROR_LAYER_NOT_PRESENT)");
		break;
	case VK_ERROR_EXTENSION_NOT_PRESENT:
		errMsg = CUBE_T("(VK_ERROR_EXTENSION_NOT_PRESENT)");
		break;
	case VK_ERROR_FEATURE_NOT_PRESENT:
		errMsg = CUBE_T("(VK_ERROR_FEATURE_NOT_PRESENT)");
		break;
	case VK_ERROR_INCOMPATIBLE_DRIVER:
		errMsg = CUBE_T("(VK_ERROR_INCOMPATIBLE_DRIVER)");
		break;
	case VK_ERROR_TOO_MANY_OBJECTS:
		errMsg = CUBE_T("(VK_ERROR_TOO_MANY_OBJECTS)");
		break;
	case VK_ERROR_FORMAT_NOT_SUPPORTED:
		errMsg = CUBE_T("(VK_ERROR_FORMAT_NOT_SUPPORTED)");
		break;
	case VK_ERROR_FRAGMENTED_POOL:
		errMsg = CUBE_T("(VK_ERROR_FRAGMENTED_POOL)");
		break;
	case VK_ERROR_SURFACE_LOST_KHR:
		errMsg = CUBE_T("(VK_ERROR_SURFACE_LOST_KHR)");
		break;
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
		errMsg = CUBE_T("(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR)");
		break;
	case VK_ERROR_OUT_OF_DATE_KHR:
		errMsg = CUBE_T("(VK_ERROR_OUT_OF_DATE_KHR)");
		break;
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
		errMsg = CUBE_T("(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR)");
		break;
	case VK_ERROR_VALIDATION_FAILED_EXT:
		errMsg = CUBE_T("(VK_ERROR_VALIDATION_FAILED_EXT)");
		break;
	case VK_ERROR_INVALID_SHADER_NV:
		errMsg = CUBE_T("(VK_ERROR_INVALID_SHADER_NV)");
		break;
	case VK_ERROR_OUT_OF_POOL_MEMORY_KHR:
		errMsg = CUBE_T("(VK_ERROR_OUT_OF_POOL_MEMORY_KHR)");
		break;
	case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR:
		errMsg = CUBE_T("(VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR)");
		break;
	default:
		errMsg = CUBE_T("(UNKNOWN)");
		break;
	}

	cube::core::LogWriter::WriteLog(cube::LogType::Error, fileName, lineNum, CUBE_T("VkResult Failed! - {0} {1}"), msg, errMsg);
}

#endif // _DEBUG
