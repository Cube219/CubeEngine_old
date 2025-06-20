#pragma once

#include "../CoreHeader.h"

#include "../Renderer/RenderObject.h"

#include "Utility/Vector.h"
#include "Utility/Matrix.h"
#include "Utility/MatrixUtility.h"
#include "Utility/Math.h"

#include <entt/entt.hpp>


namespace cube
{
    struct TransforSmComponent
    {
        Vector3 position;
        Vector3 rotation;
        Vector3 scale;

        Matrix GetModelMatrix() const
        {
            Matrix m = MatrixUtility::GetTranslation(position);
            m *= MatrixUtility::GetRotationXYZ(rotation * (Math::Pi / 180.0f));
            m *= MatrixUtility::GetScale(scale);

            return m;
        }
    };

    class CORE_EXPORT WorldObject : public RenderObject
    {
    public:
        static HWorldObject Create();

    public:
        WorldObject(entt::registry& registry) :
            mRegistry(registry)
        {}
        virtual ~WorldObject() = default;

        virtual SPtr<rt::RenderObject> CreateRTObject() override;
        // HWorldObject GetHandler() const { return mMyHandler; }

        void Destroy();

        template <typename Component, typename ...Args>
        void AddComponent(Args&& ...args)
        {
            auto& res = mRegistry.emplace<Component>(mEntity, std::forward<Args>(args)...);
            Component::OnCreate(*this, res);
        }

        template <typename Component>
        void RemoveComponent()
        {
            Component::OnDestroy(*this);
            mRegistry.remove<Component>(mEntity);
        }

    private:
        friend class World;

        entt::registry& mRegistry;
        entt::entity mEntity;
    };

    namespace rt
    {
        class WorldObject : public RenderObject
        {
        public:
            WorldObject();
            virtual ~WorldObject();

            void OnCreate() override;
            void OnDestroy() override;

        private:
            friend class RendererManagerRT;

            entt::entity mEntity;
        };
    } // namespace rt
} // namespace cube
