#include <memory>

#ifdef _WIN32
#include <Windows.h>

#include "CubeEngine/CubeEngine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	using namespace cube;

	CubeEngineStartOption startOption;
	startOption.title = L"Test title";
	startOption.windowWidth = 1024;
	startOption.windowHeight = 768;
	startOption.isWindowMode = true;

	CubeEngine::Start(startOption);

	CubeEngine::Run();

	CubeEngine::Destroy();

	return 0;
}
#endif // _WIN32
