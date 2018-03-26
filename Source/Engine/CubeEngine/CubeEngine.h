#pragma once

#include "CubeEngineHeader.h"

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

		static void Start(const CubeEngineStartOption& startOption);

		static void Run();

		static void Destroy();

	private:
		static void InitComponents();

		static void InitPlatform();
	};
} // namespace cube
