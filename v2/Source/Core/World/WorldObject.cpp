#include "WorldObject.h"

#include "../Core.h"
#include "../Renderer/RendererManager.h"

namespace cube
{
    HWorldObject WorldObject::Create()
    {
        return Core::GetWorld().RegisterWorldObject(std::make_unique<WorldObject>());
    }

    SPtr<rt::RenderObject> WorldObject::CreateRTObject()
    {
        SPtr<rt::WorldObject> rtWO = std::make_shared<rt::WorldObject>();

        return rtWO;
    }

    void WorldObject::Destroy()
    {
        auto h = GetHandler();
        Core::GetWorld().UnregisterWorldObject(h);
    }

    namespace rt
    {
        WorldObject::WorldObject()
        {
        }

        WorldObject::~WorldObject()
        {
        }

        void WorldObject::OnCreate()
        {
            RendererManagerRT::RegisterWorldObject(*this);
        }

        void WorldObject::OnDestroy()
        {
            RendererManagerRT::UnregisterWorldObject(*this);
        }
    } // namespace rt
} // namespace cube
