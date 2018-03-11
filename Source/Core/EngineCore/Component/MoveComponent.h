#pragma once

#include "../EngineCoreHeader.h"

#include "Component.h"
//#include "InputModule/InputModule.h"

namespace cube
{
	namespace core
	{
		class MoveComponent : public Component
		{
		public:
			MoveComponent();
			virtual ~MoveComponent();

			void OnInit() override;
			void OnUpdate() override;
			void OnDestroy() override;

		private:
			//SPtr<module::InputModule> mInputModule;
		};
	}
}