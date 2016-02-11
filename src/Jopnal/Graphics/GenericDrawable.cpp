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
    GenericDrawable::GenericDrawable(Object& object, const std::string& ID)
        : Drawable(object, ID)
    {}

    //////////////////////////////////////////////

    GenericDrawable* GenericDrawable::clone() const
    {
        return new GenericDrawable(*this);
    }

    //////////////////////////////////////////////

    void GenericDrawable::draw(const Camera& camera)
    {
        if (m_shader.expired() || m_model.expired())
            return;

        auto& s = *m_shader.lock();
        auto& m = *m_model.lock();

        m.getMesh().lock()->getVertexBuffer().bind();

        s.setUniform("u_PVMMatrix", camera.getProjectionMatrix() * camera.getViewMatrix() * getObject().getMatrix());
        s.setAttribute(0, gl::FLOAT, 3, sizeof(Vertex), false, (void*)Vertex::Position);

        m.getMaterial().sendToShader(s);
        s.setAttribute(1, gl::FLOAT, 2, sizeof(Vertex), false, (void*)Vertex::TexCoords);

        m.getMesh().lock()->getIndexBuffer().bind();
        glCheck(gl::DrawElements(gl::TRIANGLES, m.getElementAmount(), gl::UNSIGNED_INT, (void*)0));
    }
}