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
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

//////////////////////////////////////////////

// Headers
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Graphics/Mesh/BoxMesh.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    BoxMesh::BoxMesh(const std::string& name)
        : Mesh      (name),
          m_size    (0.f)
    {}

    BoxMesh::BoxMesh(const BoxMesh& other, const std::string& newName)
        : Mesh       (newName),
          m_size     (0.f)
    {
        load(other.getSize());
    }

    //////////////////////////////////////////////

    bool BoxMesh::load(const glm::vec3& size)
    {
        return load(size, glm::vec2(0.f), glm::vec2(1.f));
    }

    //////////////////////////////////////////////

    bool BoxMesh::load(const glm::vec3& size, const glm::vec2& min, const glm::vec2& max)
    {
        const auto pair = std::make_pair(min, max);
        return load(size, pair, pair, pair, pair, pair, pair);
    }

    //////////////////////////////////////////////

    bool BoxMesh::load(const glm::vec3& size,
                       const std::pair<glm::vec2, glm::vec2>& front,
                       const std::pair<glm::vec2, glm::vec2>& left,
                       const std::pair<glm::vec2, glm::vec2>& back,
                       const std::pair<glm::vec2, glm::vec2>& right,
                       const std::pair<glm::vec2, glm::vec2>& top,
                       const std::pair<glm::vec2, glm::vec2>& bottom)
    {
        const glm::vec3 half(size * 0.5f);
        const float norm = 1.f;

        const std::vector<Vertex> vertices
        ({
            // Front
            Vertex(glm::vec3(-half.x, -half.y,  half.z), glm::vec2(front.first.x,   front.second.y),  glm::vec3(0.f, 0.f, norm)), // 0, Left, Bottom, Front   
            Vertex(glm::vec3( half.x, -half.y,  half.z), glm::vec2(front.second.x,  front.second.y),  glm::vec3(0.f, 0.f, norm)), // 1, Right, Bottom, Front  
            Vertex(glm::vec3(-half.x,  half.y,  half.z), glm::vec2(front.first.x,   front.first.y),   glm::vec3(0.f, 0.f, norm)), // 2, Left, Top, Front      
            Vertex(glm::vec3( half.x,  half.y,  half.z), glm::vec2(front.second.x,  front.first.y),   glm::vec3(0.f, 0.f, norm)), // 3, Right, Top, Front     
                                                                             
            // Left                                                                                 
            Vertex(glm::vec3(-half.x, -half.y, -half.z), glm::vec2(left.first.x,    left.second.y),   glm::vec3(-norm, 0.f, 0.f)), // 4, Left, Bottom, Rear  
            Vertex(glm::vec3(-half.x, -half.y,  half.z), glm::vec2(left.second.x,   left.second.y),   glm::vec3(-norm, 0.f, 0.f)), // 5, Left, Bottom, Front 
            Vertex(glm::vec3(-half.x,  half.y, -half.z), glm::vec2(left.first.x,    left.first.y),    glm::vec3(-norm, 0.f, 0.f)), // 6, Left, Top, Rear     
            Vertex(glm::vec3(-half.x,  half.y,  half.z), glm::vec2(left.second.x,   left.first.y),    glm::vec3(-norm, 0.f, 0.f)), // 7, Left, Top, Front    
                                                                             
            // Rear                                                                                 
            Vertex(glm::vec3( half.x, -half.y, -half.z), glm::vec2(back.first.x,    back.second.y),   glm::vec3(0.f, 0.f, -norm)), // 8, Right, Bottom, Rear 
            Vertex(glm::vec3(-half.x, -half.y, -half.z), glm::vec2(back.second.x,   back.second.y),   glm::vec3(0.f, 0.f, -norm)), // 9, Left, Bottom, Rear  
            Vertex(glm::vec3( half.x,  half.y, -half.z), glm::vec2(back.first.x,    back.first.y),    glm::vec3(0.f, 0.f, -norm)), // 10, Right, Top, Rear   
            Vertex(glm::vec3(-half.x,  half.y, -half.z), glm::vec2(back.second.x,   back.first.y),    glm::vec3(0.f, 0.f, -norm)), // 11, Left, Top, Rear    
                                                                              
            // Right                                                                                 
            Vertex(glm::vec3( half.x, -half.y,  half.z), glm::vec2(right.first.x,   right.second.y),  glm::vec3(norm, 0.f, 0.f)), // 12, Right, Bottom, Front
            Vertex(glm::vec3( half.x, -half.y, -half.z), glm::vec2(right.second.x,  right.second.y),  glm::vec3(norm, 0.f, 0.f)), // 13, Right, Bottom, Rear
            Vertex(glm::vec3( half.x,  half.y,  half.z), glm::vec2(right.first.x,   right.first.y),   glm::vec3(norm, 0.f, 0.f)), // 14, Right, Top, Front  
            Vertex(glm::vec3( half.x,  half.y, -half.z), glm::vec2(right.second.x,  right.first.y),   glm::vec3(norm, 0.f, 0.f)), // 15, Right, Top, Rear   
                                                                              
            // Top                                                                                   
            Vertex(glm::vec3(-half.x,  half.y,  half.z), glm::vec2(top.first.x,     top.second.y),    glm::vec3(0.f, norm, 0.f)), // 16, Left, Top, Front   
            Vertex(glm::vec3( half.x,  half.y,  half.z), glm::vec2(top.second.x,    top.second.y),    glm::vec3(0.f, norm, 0.f)), // 17, Right, Top, Front
            Vertex(glm::vec3(-half.x,  half.y, -half.z), glm::vec2(top.first.x,     top.first.y),     glm::vec3(0.f, norm, 0.f)), // 18, Left, Top, Rear
            Vertex(glm::vec3( half.x,  half.y, -half.z), glm::vec2(top.second.x,    top.first.y),     glm::vec3(0.f, norm, 0.f)), // 19, Right, Top, Rear
                                                    
            // Bottom                                                                            
            Vertex(glm::vec3(-half.x, -half.y, -half.z), glm::vec2(bottom.first.x,  bottom.second.y), glm::vec3(0.f, -norm, 0.f)), // 20, Left, Bottom, Rear
            Vertex(glm::vec3( half.x, -half.y, -half.z), glm::vec2(bottom.second.x, bottom.second.y), glm::vec3(0.f, -norm, 0.f)), // 21, Right, Bottom, Rear
            Vertex(glm::vec3(-half.x, -half.y,  half.z), glm::vec2(bottom.first.x,  bottom.first.y),  glm::vec3(0.f, -norm, 0.f)), // 22, Left, Bottom, Front
            Vertex(glm::vec3( half.x, -half.y,  half.z), glm::vec2(bottom.second.x, bottom.first.y),  glm::vec3(0.f, -norm, 0.f))  // 23, Right, Bottom, Front
        });
        static const std::vector<unsigned int> indices
        ({
            // Front
            0, 1, 2,
            2, 1, 3,

            // Left
            4, 5, 6,
            6, 5, 7,

            // Rear
            8, 9, 10,
            10, 9, 11,

            // Right
            12, 13, 14,
            14, 13, 15,

            // Top
            16, 17, 18,
            18, 17, 19,

            // Bottom
            20, 21, 22,
            22, 21, 23
        });

        updateBounds(-half, half);

        return Mesh::load(vertices, indices);
    }

    bool BoxMesh::load(const glm::vec3& size,
                       const TextureAtlas& atlas,
                       const unsigned int front,
                       const unsigned int left,
                       const unsigned int back,
                       const unsigned int right,
                       const unsigned int top,
                       const unsigned int bottom)
    {
        return load(size,
                    atlas.getCoordinates(front),
                    atlas.getCoordinates(left),
                    atlas.getCoordinates(back), 
                    atlas.getCoordinates(right),
                    atlas.getCoordinates(top),
                    atlas.getCoordinates(bottom));
    }

    //////////////////////////////////////////////

    const glm::vec3& BoxMesh::getSize() const
    {
        return m_size;
    }
}