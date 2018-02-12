#include "ExampleModule.h"

namespace cube
{
	namespace module
	{
		BaseModule* CreateModule()
		{
			return new ExampleModule;
		}

		ExampleModule::ExampleModule()
		{
			mInfo.name = "ExampleModule";
			mInfo.isRunAfterComponent = false;
		}

		ExampleModule::~ExampleModule()
		{
		}

		void ExampleModule::Init()
		{
		}

		void ExampleModule::Update(float dt)
		{
		}

		void ExampleModule::Destroy()
		{
		}
	}
}