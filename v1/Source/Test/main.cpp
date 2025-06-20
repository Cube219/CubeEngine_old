#include "Platform.h"
#include "RenderAPI/RenderAPI.h"
#include "RenderAPI/Interface/Device.h"
#include "RenderAPI/Interface/SwapChain.h"

using namespace cube;

SPtr<platform::DLib> dLib;
SPtr<render::RenderAPI> renderAPI;

SPtr<render::Device> device;
SPtr<render::SwapChain> swapChain;

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

void CreateSwapChain()
{
	render::SwapChainAttribute swapChainAttr;
	swapChainAttr.width = 1024;
	swapChainAttr.height = 768;
	swapChainAttr.colorBufferFormat = render::TextureFormat::RGBA_8_sRGB;
	swapChainAttr.depthBufferFormat = render::TextureFormat::D32_Float;
	swapChainAttr.bufferCount = 2;
	swapChainAttr.debugName = "SwapChain";

	swapChain = device->CreateSwapChain(swapChainAttr);
}

int main(void)
{
	platform::Platform::Init();

	platform::Platform::InitWindow(CUBE_T("Test"), 800, 600);
	platform::Platform::ShowWindow();

	InitRenderAPI();
	InitDevice();
	CreateSwapChain();
	
	return 0;
}