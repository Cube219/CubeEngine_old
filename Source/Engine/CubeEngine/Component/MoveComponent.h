#pragma once

#include "../CubeEngineHeader.h"

#include "EngineCore/Component/Component.h"
#include "InputModule/InputModule.h"

namespace cube
{
	class MoveComponent : public core::Component
	{
	public:
		static const String2& GetName() { return mName; }

	private:
		static String2 mName;

	public:
		MoveComponent();
		virtual ~MoveComponent();

		void OnInit() override;
		void OnUpdate(float dt) override;
		void OnDestroy() override;

	private:
		SPtr<module::InputModule> mInputModule;
	};
} // namespace cube
