#pragma once

#include "CubeEngineHeader.h"

#include "EngineCore/GameThread.h"
#include "EngineCore/Renderer/RenderingThread.h"
#include "EngineCore/Thread/MutexLock.h"

namespace cube
{
	struct CUBE_ENGINE_EXPORT CubeEngineStartOption
	{
		bool isWindowMode = false;
		uint32_t windowWidth;
		uint32_t windowHeight;
		String title;
	};

	class CUBE_ENGINE_EXPORT CubeEngine
	{
	public:
		CubeEngine() = delete;
		~CubeEngine() = delete;

		static void Initialize(const CubeEngineStartOption& startOption);
		static void ShutDown();

		static void Run();

		static void Close();

		static void SetCustomClosingFunction(std::function<void()> func);

	private:
		static void RegisterImporters();

		static void InitComponents();

		static void DefaultClosingFunction();

		static EventFunction<void()> closingEventFunc;
	};
} // namespace cube
