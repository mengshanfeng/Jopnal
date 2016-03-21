// Jopnal Engine C++ Library
// Copyright (c) 2016 Team Jopnal
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

// Headers
#include <Jopnal/Precompiled.hpp>

//////////////////////////////////////////////


namespace detail
{
    class MotionState final : public btMotionState
    {
    private:

        jop::WeakReference<jop::Object> m_obj;

    public:

        MotionState(jop::Object& obj)
            : m_obj(obj)
        {}

        void getWorldTransform(btTransform& worldTrans) const override
        {
            auto& p = m_obj->getPosition();
            auto& r = m_obj->getRotation();

            worldTrans.setOrigin(btVector3(p.x, p.y, p.z));
            worldTrans.setRotation(btQuaternion(r.x, r.y, r.z, r.w));
        }

        void setWorldTransform(const btTransform& worldTrans) override
        {
            auto& p = worldTrans.getOrigin();
            auto r = worldTrans.getRotation();

            m_obj->setPosition(p.x(), p.y(), p.z());
            m_obj->setRotation(glm::quat(r.w(), r.x(), r.y(), r.z()));
        }
    };
}

namespace jop
{
    Collider::Collider(Object& object, World& world, const std::string& ID)
        : Component     (object, ID),
          m_motionState (std::make_unique<::detail::MotionState>(object)),
          m_body        (),
          m_worldRef    (world)
    {}

    Collider::Collider(const Collider& other, Object& newObj)
        : Component     (other, newObj),
          m_motionState (std::make_unique<::detail::MotionState>(newObj)),
          m_body        (),
          m_worldRef    (other.m_worldRef)
    {}

    Collider::~Collider()
    {}

    //////////////////////////////////////////////

    bool Collider::checkOverlap(const Collider& other) const
    {
        struct Callback : btBroadphaseAabbCallback
        {
            const void* m_against;
            bool hit;

            Callback(const void* against)
                : m_against (against),
                  hit       (false)
            {}

            bool process(const btBroadphaseProxy* proxy) override
            {
                return (hit = (m_against == proxy->m_clientObject)) != true;
            }

        } cb(m_body->getBroadphaseHandle()->m_clientObject);

        auto& bp = *m_worldRef.m_worldData->world->getBroadphase();

        btVector3 min, max;
        bp.getAabb(other.m_body->getBroadphaseHandle(), min, max);
        bp.aabbTest(min, max, cb);

        return cb.hit;
    }

    //////////////////////////////////////////////

    bool Collider::checkContact(const Collider& other) const
    {
        struct Callback : btCollisionWorld::ContactResultCallback
        {
            bool hit;

            Callback()
                : hit(false)
            {}

            btScalar addSingleResult(btManifoldPoint&, const btCollisionObjectWrapper*, int, int, const btCollisionObjectWrapper*, int, int) override
            {
                hit = true;
                return 0.f;
            }

        } cb;

        m_worldRef.m_worldData->world->contactPairTest(m_body.get(), other.m_body.get(), cb);

        return cb.hit;
    }

    //////////////////////////////////////////////

    bool Collider::checkRay(const glm::vec3& start, const glm::vec3& ray) const
    {
        struct Callback : btCollisionWorld::RayResultCallback
        {
            const void* m_against;
            bool hit;

            Callback(const void* against)
                : m_against (against),
                  hit       (false)
            {}

            btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool) override
            {
                hit = rayResult.m_collisionObject->getUserPointer() == m_against;
                return 0.f;
            }

        } cb(this);

        btTransform from;
        from.setOrigin(btVector3(start.x, start.y, start.z));
        btTransform to;
        const glm::vec3 fromTo(start + ray);
        to.setOrigin(btVector3(fromTo.x, fromTo.y, fromTo.z));

        m_worldRef.m_worldData->world->rayTestSingle(from, to, m_body.get(), m_body->getCollisionShape(), m_body->getWorldTransform(), cb);

        return cb.hit;
    }
}