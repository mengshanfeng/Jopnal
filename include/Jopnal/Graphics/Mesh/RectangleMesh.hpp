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

#ifndef JOP_RECTANGLEMESH_HPP
#define JOP_RECTANGLEMESH_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Mesh/Mesh.hpp>
#include <glm/vec2.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API RectangleMesh : public Mesh
    {
    public:

        /// \copydoc Mesh::Mesh()
        ///
        RectangleMesh(const std::string& name);

        /// \copydoc Mesh::Mesh(const Mesh&, const std::string&)
        ///
        RectangleMesh(const RectangleMesh& other, const std::string& newName);


        /// \brief Load this rectangle
        ///
        /// \param size Size of the rectangle
        ///
        /// \return True if successful
        ///
        bool load(const float size);

        /// \brief Load this rectangle using differing dimensions
        ///
        /// \param size Size as vector
        ///
        /// \return True if successful
        ///
        bool load(const glm::vec2& size);

        /// \brief Load rectangle from specific part of texture
        ///
        /// \param size Size of the rectangle
        /// \param min Minimum texture coordinates
        /// \param max Maximum texture coordinates
        ///
        bool load(const float size, const glm::vec2& min, const glm::vec2& max);

        /// \copydoc load(const float, const glm::vec2&, const glm::vec2&)
        ///
        bool load(const glm::vec2& size, const glm::vec2& min, const glm::vec2& max);

        /// \brief Get the size
        ///
        /// \return The size
        ///
        const glm::vec2& getSize() const;

    private:

        glm::vec2 m_size;   ///< This rectangle's size
    };
}

/// \class jop::RectangleMesh
/// \ingroup graphics

#endif