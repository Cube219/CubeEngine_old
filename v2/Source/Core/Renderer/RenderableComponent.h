#pragma once

#include "../CoreHeader.h"

namespace cube
{
    struct RenderableComponent
    {
        SPtr<Mesh> mesh;

        static void OnCreate(WorldObject& worldObject, RenderableComponent& component)
        {
        }

        static void OnDestroy(WorldObject& worldObject)
        {
        }

        void SetMesh(SPtr<Mesh> mesh)
        {
            mesh = mesh;
        }
    };

    namespace rt
    {
        struct RenderableComponent
        {
            SPtr<Mesh> rtMesh;
        };
    } // namespace rt

    // Sync시 rt::renderablecomponent의 matrix를 transform component에서 가져와서 갱신하는 system 추가
} // namespace cube
