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

#ifndef JOP_VERTEX_HPP
#define JOP_VERTEX_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API Vertex
    {
    public:

        /// \brief Default constructor
        ///
        Vertex() = default;

        /// \brief Construct the vertex from its position
        ///
        /// \param pos Vertex position
        ///
        Vertex(const glm::vec3& pos);

        /// \brief Construct the vertex from its position and texture coordinates
        ///
        /// \param pos Vertex position
        /// \param tc Vertex texture coordinates
        ///
        Vertex(const glm::vec3& pos, const glm::vec2& tc);

        /// \brief Construct the vertex from its position and normal vector 
        ///
        /// \param pos Vertex position
        /// \param nor Vertex normal vector
        ///
        Vertex(const glm::vec3& pos, const glm::vec3& nor);

        /// \brief Construct the vertex from its position, texture coordinates and normal vector
        ///
        /// \param pos Vertex position
        /// \param tc Vertex texture coordinates
        /// \param nor Vertex normal vector
        ///
        Vertex(const glm::vec3& pos, const glm::vec2& tc, const glm::vec3& nor);

        glm::vec3 position;     ///< 3D position of the vertex
        glm::vec2 texCoords;    ///< Coordinates of the texture's pixel to map to vertex
        glm::vec3 normal;       ///< Normal vector of the vertex 
    };
}

/// \class jop::Vertex
/// \ingroup graphics

#endif