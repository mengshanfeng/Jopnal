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
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Physics2D/Shape/ConvexHullShape2D.hpp>

    #include <Jopnal/Core/ResourceManager.hpp>
    #include <Box2D/Collision/Shapes/b2PolygonShape.h>

#endif

//////////////////////////////////////////////

namespace jop
{
    ConvexHullShape2D::ConvexHullShape2D(const std::string& name)
        : CollisionShape2D(name)
    {}

    //////////////////////////////////////////////

    bool ConvexHullShape2D::load(const std::vector<glm::vec2>& points)
    {
        return load(points, std::vector<unsigned int>());
    }

    //////////////////////////////////////////////

    bool ConvexHullShape2D::load(const std::vector<glm::vec2>& points, const std::vector<unsigned int>& indices)
    {
        if (points.size() <= 2 || points.size() >= 9)
            return false;

        auto temp = std::make_unique<b2PolygonShape>();
        indices.size() < 3 ? temp->Set(reinterpret_cast<const b2Vec2*>(points.data()), points.size()) : temp->Set(reinterpret_cast<const b2Vec2*>(points.data()), indices.size());
        m_shape = std::move(temp);
        return true;
    }

    //////////////////////////////////////////////

    ConvexHullShape2D& ConvexHullShape2D::getDefault()
    {
        static WeakReference<ConvexHullShape2D> defShape;

        if (defShape.expired())
        {
            defShape = static_ref_cast<ConvexHullShape2D>(ResourceManager::getEmptyResource<ConvexHullShape2D>("jop_default_staticmeshshape2D").getReference());

            static const std::vector<glm::vec2> points =
            {
                glm::vec2(-0.5f, -0.5f),
                glm::vec2(0.5f, -0.5f),
                glm::vec2(0.5f, 0.5f),
                glm::vec2(-0.5f, 0.5f)
            };

            defShape->load(points);
            defShape->setPersistence(0);
        }
        return *defShape;
    }
}