#include "Platform.h"
#include "BaseRenderAPI_New/RenderAPI.h"

using namespace cube;

int main(void)
{
	platform::Platform::Init();

	platform::Platform::InitWindow(CUBE_T("Test"), 800, 600);
	platform::Platform::ShowWindow();

	auto dLib = platform::Platform::LoadDLib(CUBE_T("VulkanAPI_New"));

	using CreateAPIFunction = render::RenderAPI*(*)();

	auto createAPIFunction = RCast(CreateAPIFunction)(dLib->GetFunction(CUBE_T("GetAPI")));
	SPtr<render::RenderAPI> renderAPI(createAPIFunction());

	render::RenderAPIAttribute renderAPIAttr;
	renderAPIAttr.enableDebugLayer = true;
	renderAPI->Init(renderAPIAttr);

	platform::Platform::Sleep(5000);

	return 0;
}