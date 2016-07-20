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

#ifndef JOP_JOINT2D_HPP
#define JOP_JOINT2D_HPP

// Headers
#include <Jopnal/Header.hpp>

//////////////////////////////////////////////

class b2Joint;

namespace jop
{
    class RigidBody2D;

    class JOP_API Joint2D
    {
    public:
        Joint2D(World2D& worldRef, RigidBody2D& bodyA, RigidBody2D& bodyB);
        virtual ~Joint2D() = 0;

        unsigned int getID() const;
        Joint2D& setID(const unsigned int id);

        World2D* m_worldRef;

        RigidBody2D* m_bodyA;
        RigidBody2D* m_bodyB;
        //std::weak_ptr<RigidBody2D> m_bodyA;
        //std::weak_ptr<RigidBody2D> m_bodyB;

    protected:
        static b2Body* getBody(RigidBody2D& body); //(std::weak_ptr<RigidBody2D>& body);

        b2Joint* m_joint;
        unsigned int m_ID;
    };
}
#endif