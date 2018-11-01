#include "Platform.h"
#include "BaseRenderAPI_New/RenderAPI.h"
#include "BaseRenderAPI_New/Interface/Device.h"

using namespace cube;

SPtr<platform::DLib> dLib;
SPtr<render::RenderAPI> renderAPI;

SPtr<render::Device> device;

void InitRenderAPI()
{
	dLib = platform::Platform::LoadDLib(CUBE_T("VulkanAPI_New"));

	using CreateAPIFunction = render::RenderAPI*(*)();

	auto createAPIFunction = RCast(CreateAPIFunction)(dLib->GetFunction(CUBE_T("GetAPI")));
	SPtr<render::RenderAPI> temp(createAPIFunction());
	renderAPI = temp;

	render::RenderAPIAttribute renderAPIAttr;
	renderAPIAttr.enableDebugLayer = true;
	renderAPI->Init(renderAPIAttr);
}

void InitDevice()
{
	render::DeviceAttribute deviceAttr;
	deviceAttr.GPUIndex = 0;
	deviceAttr.enableDebugLayer = true;
	deviceAttr.debugName = "MainDevice";

	device = renderAPI->GetDevice(deviceAttr);
}

int main(void)
{
	platform::Platform::Init();

	platform::Platform::InitWindow(CUBE_T("Test"), 800, 600);
	platform::Platform::ShowWindow();

	InitRenderAPI();
	InitDevice();
	
	return 0;
}