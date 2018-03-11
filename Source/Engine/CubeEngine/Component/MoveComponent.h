#pragma once

#include "../CubeEngineHeader.h"

#include "EngineCore/Component/Component.h"
#include "InputModule/InputModule.h"

namespace cube
{
	class MoveComponent : public core::Component
	{
	public:
		static const String& GetName() { return mName; }

	private:
		static String mName;

	public:
		MoveComponent();
		virtual ~MoveComponent();

		void OnInit() override;
		void OnUpdate() override;
		void OnDestroy() override;

	private:
		SPtr<module::InputModule> mInputModule;
	};
}
