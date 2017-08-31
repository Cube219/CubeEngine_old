#include <memory>

#include "BasePlatform\BasePlatform.h"
#include "EngineCore\EngineCore.h"

using namespace cube;

void LoadEngine(std::shared_ptr<platform::BasePlatform>& platform)
{
	auto core = std::make_unique<core::EngineCore>(platform);

	core->Prepare();
	core->Run();
}

#ifdef _WIN32
#include <Windows.h>

#include "WinPlatform\WinPlatform.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	SPtr<platform::BasePlatform> p = std::make_shared<platform::WinPlatform>(hInstance);

	p->InitWindow(L"Test Title", 1024, 768);
	p->ShowWindow();
	
	LoadEngine(p);

	Sleep(1000);

	return 0;
}
#endif // _WIN32