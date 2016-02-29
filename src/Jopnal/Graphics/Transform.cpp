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


namespace jop
{
    Transform::Transform()
        : m_transform               (IdentityMatrix),
          m_invTransform            (IdentityMatrix),
          m_rotation                (1.f, 0.f, 0.f, 0.f),
          m_scale                   (1.f, 1.f, 1.f),
          m_position                (0.f, 0.f, 0.f),
          m_transformNeedUpdate     (false),
          m_invTransformNeedsUpdate (false)
    {}

    //////////////////////////////////////////////

    const glm::mat4& Transform::getMatrix() const
    {
        if (m_transformNeedUpdate)
        {
            m_transform = IdentityMatrix;

            m_transform = glm::translate(m_transform, m_position);
            m_transform *= m_rotation.operator glm::tmat4x4<float, glm::highp>();
            m_transform = glm::scale(m_transform, m_scale);
            
            m_transformNeedUpdate = false;
        }

        return m_transform;
    }

    //////////////////////////////////////////////

    const glm::mat4& Transform::getInverseMatrix() const
    {
        if (m_invTransformNeedsUpdate)
        {
            m_invTransform = glm::inverse(getMatrix());
            m_invTransformNeedsUpdate = false;
        }

        return m_invTransform;
    }

    //////////////////////////////////////////////

    Transform& Transform::setRotation(const float x, const float y, const float z)
    {
        return setRotation(glm::vec3(x, y, z));
    }

    //////////////////////////////////////////////

    Transform& Transform::setRotation(const glm::vec3& rotation)
    {
        return setRotation(glm::quat(rotation));
    }

    //////////////////////////////////////////////

    Transform& Transform::setRotation(const glm::quat& rotation)
    {
        m_rotation = glm::conjugate(rotation);
        m_transformNeedUpdate = true;
        m_invTransformNeedsUpdate = true;

        return *this;
    }

    //////////////////////////////////////////////

    Transform& Transform::setRotation(const float angle, const glm::vec3& axis)
    {
        return setRotation(glm::quat(angle, axis));
    }

    //////////////////////////////////////////////

    glm::quat Transform::getRotation() const
    {
        return glm::conjugate(m_rotation);
    }

    //////////////////////////////////////////////

    glm::quat Transform::getGlobalRotation() const
    {
        return glm::conjugate(glm::quat_cast(getMatrix()));
    }

    //////////////////////////////////////////////

    glm::vec3 Transform::getGlobalFront() const
    {
        auto& m = getMatrix();
        return glm::normalize(-glm::vec3(m[2][0], m[2][1], m[2][2]));
    }

    //////////////////////////////////////////////

    glm::vec3 Transform::getGlobalRight() const
    {
        auto& m = getMatrix();
        return glm::normalize(glm::vec3(m[0][0], m[0][1], m[0][2]));
    }

    //////////////////////////////////////////////

    glm::vec3 Transform::getGlobalUp() const
    {
        auto& m = getMatrix();
        return glm::normalize(glm::vec3(m[1][0], m[1][1], m[1][2]));
    }

    //////////////////////////////////////////////

    glm::vec3 Transform::getLocalFront() const
    {
        return m_rotation * Front;
    }

    //////////////////////////////////////////////

    glm::vec3 Transform::getLocalRight() const
    {
        return m_rotation * Right;
    }

    //////////////////////////////////////////////

    glm::vec3 Transform::getLocalUp() const
    {
        return m_rotation * Up;
    }

    //////////////////////////////////////////////

    Transform& Transform::setScale(const float x, const float y, const float z)
    {
        m_scale = glm::vec3(x, y, z);
        m_transformNeedUpdate = true;
        m_invTransformNeedsUpdate = true;

        return *this;
    }

    //////////////////////////////////////////////

    Transform& Transform::setScale(const glm::vec3& scale)
    {
        return setScale(scale.x, scale.y, scale.z);
    }

    //////////////////////////////////////////////

    Transform& Transform::setScale(const float delta)
    {
        return setScale(delta, delta, delta);
    }

    //////////////////////////////////////////////

    const glm::vec3& Transform::getScale() const
    {
        return m_scale;
    }

    //////////////////////////////////////////////

    glm::vec3 Transform::getGlobalScale() const
    {
        auto& m = getMatrix();

        // X axis
        auto x = glm::length(glm::vec3(m[0][0], m[0][1], m[0][2]));

        // Y axis
        auto y = glm::length(glm::vec3(m[1][0], m[1][1], m[1][2]));

        // Z axis
        auto z = glm::length(glm::vec3(m[2][0], m[2][1], m[2][2]));

        return glm::vec3(x, y, z);
    }

    //////////////////////////////////////////////

    Transform& Transform::setPosition(const float x, const float y, const float z)
    {
        m_position = glm::vec3(x, y, z);
        m_transformNeedUpdate = true;
        m_invTransformNeedsUpdate = true;

        return *this;
    }

    //////////////////////////////////////////////

    Transform& Transform::setPosition(const glm::vec3& origin)
    {
        return setPosition(origin.x, origin.y, origin.z);
    }

    //////////////////////////////////////////////

    const glm::vec3& Transform::getPosition() const
    {
        return m_position;
    }

    //////////////////////////////////////////////

    glm::vec3 Transform::getGlobalPosition() const
    {
        auto& mat = getMatrix();
        return glm::vec3(mat[3][0], mat[3][1], mat[3][2]);
    }

    //////////////////////////////////////////////

    Transform& Transform::lookAt(const glm::vec3& point)
    {
        static const glm::vec3 upVec(0.f, 1.f, 0.f);
        return lookAt(point, upVec);
    }

    //////////////////////////////////////////////

    Transform& Transform::lookAt(const float x, const float y, const float z)
    {
        return lookAt(glm::vec3(x, y, z));
    }

    //////////////////////////////////////////////

    Transform& Transform::lookAt(const glm::vec3& point, const glm::vec3& up)
    {
        m_invTransform = glm::lookAt(getGlobalPosition(), point, up);
        m_transform = glm::inverse(m_invTransform);
        m_transformNeedUpdate = false;
        m_invTransformNeedsUpdate = false;

        return *this;
    }

    //////////////////////////////////////////////

    Transform& Transform::move(const float x, const float y, const float z)
    {
        return move(glm::vec3(x, y, z));
    }

    //////////////////////////////////////////////

    Transform& Transform::move(const glm::vec3& offset)
    {
        return setPosition(m_position + offset);
    }

    //////////////////////////////////////////////

    Transform& Transform::rotate(const float x, const float y, const float z)
    {
        return rotate(glm::vec3(x, y, z));
    }

    //////////////////////////////////////////////

    Transform& Transform::rotate(const glm::quat& rotation)
    {
        return setRotation(glm::conjugate(m_rotation * glm::conjugate(rotation)));
    }

    //////////////////////////////////////////////
    
    Transform& Transform::rotate(const glm::vec3& rotation)
    {
        return rotate(glm::quat(rotation));
    }

    //////////////////////////////////////////////

    Transform& Transform::rotate(const float angle, const glm::vec3& axis)
    {
        return rotate(glm::angleAxis(angle, axis));
    }

    //////////////////////////////////////////////

    Transform& Transform::scale(const float x, const float y, const float z)
    {
        return scale(glm::vec3(x, y, z));
    }

    //////////////////////////////////////////////

    Transform& Transform::scale(const glm::vec3& scale)
    {
        return setScale(m_scale * scale);
    }

    //////////////////////////////////////////////

    Transform& Transform::scale(const float delta)
    {
        return scale(delta, delta, delta);
    }

    //////////////////////////////////////////////

    const glm::mat4 Transform::IdentityMatrix
    (
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f
    );

    //////////////////////////////////////////////

    const glm::vec3 Transform::Front(0.f, 0.f, -1.f);

    const glm::vec3 Transform::Right(1.f, 0.f, 0.f);

    const glm::vec3 Transform::Up(0.f, 1.f, 0.f);
}