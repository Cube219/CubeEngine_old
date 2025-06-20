#pragma once

#include "../CubeEngineHeader.h"

#include "EngineCore/Component/Component.h"
#include "InputModule/InputModule.h"

namespace cube
{
	class CUBE_ENGINE_EXPORT MoveComponent : public Component
	{
	public:
		static const String& GetName() { return mName; }

	private:
		static String mName;

	public:
		MoveComponent();
		virtual ~MoveComponent();

		void OnInit() override;
		void OnUpdate(float dt) override;
		void OnDestroy() override;
		void OnTransformChanged() override;

	private:
		SPtr<module::InputModule> mInputModule;
	};

	using HMoveComponent = Handler<MoveComponent>;
} // namespace cube
