#include "MoveComponent.h"

#include "../ModuleManager.h"
#include "Base/format.h"
#include "../LogWriter.h"

namespace cube
{
	namespace core
	{
		MoveComponent::MoveComponent()
		{
		}

		MoveComponent::~MoveComponent()
		{
		}

		void MoveComponent::OnInit()
		{
			//mInputModule = DPCast(module::InputModule)(ECore()->GetModuleManager()->GetModule(String("InputModule")));
		}

		void MoveComponent::OnUpdate()
		{
			//bool r = mInputModule->IsActionPressed(String("Jump"));

			//CUBE_LOG(LogType::Info, fmt::format(L"Jump: {0}", r));
		}

		void MoveComponent::OnDestroy()
		{
		}
	}
}