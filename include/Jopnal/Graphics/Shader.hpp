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

#ifndef JOP_SHADER_HPP
#define JOP_SHADER_HPP

// Headers
#include <Jopnal/Jopnal.hpp>
#include <Jopnal/Core/Resource.hpp>

//////////////////////////////////////////////


namespace jop
{
    class Texture;

    class Shader : public Resource
    {
    public:

        /// \brief default constructor
        ///
        Shader();

        /// \brief default destructor
        ///
        ~Shader() override;


        /// \brief Load a shader
        ///
        /// The arguments don't have to be paths. They may also contain the shader code.
        ///
        /// \param vert Vertex shader path
        /// \param geom Geometry shader path
        /// \param frag Fragment shader path
        ///
        /// \return True if the shader was loaded, compiled and linked successfully
        ///
        bool load(const std::string& vert, const std::string& geom, const std::string& frag);

        /// \brief Destroy this shader
        ///
        void destroy();


        /// \brief Bind this shader for use
        ///
        /// \return True if successful
        ///
        bool bind() const;

        /// \brief Unbind the currently bound shader
        ///
        static void unbind();

          
        /// \brief method setting matrix 4x4
        ///
        /// \param name unique name
        /// \param matrix 4x4 matrix
        ///
        bool setUniform(const std::string& name, const glm::mat4& matrix);

        /// \brief method setting 3x3 matrix with unique name
        ///
        /// \param name unique name
        /// \param matrix 3x3 matrix
        ///
        bool setUniform(const std::string& name, const glm::mat3& matrix);

        /// \brief method setting vector with 3 dimensions and unique name
        ///
        /// \param name unique name
        /// \param vector 3 dimensional vector
        ///
        bool setUniform(const std::string& name, const glm::vec3& vector);
        
        /// \brief method setting vector with 4 dimensions and unique name
        ///
        /// \param name unique name
        /// \param vector 4 dimensional vector
        ///
        bool setUniform(const std::string& name, const glm::vec4& vector);

        /// \brief method setting texture with unique name and Uint
        ///
        /// \param name unique name
        /// \param texture The texture
        /// \param unit The texture unit
        ///
        bool setUniform(const std::string& name, const jop::Texture& texture, const unsigned int unit);

        /// \brief method setting attributes
        ///
        /// \param name unique name
        /// \param type GLenum type
        /// \param amount attributes size Glint size
        /// \param stride 
        /// \param pointer void pointer
        ///
        bool setAttribute(const std::string& name, unsigned int type, int amount, unsigned int stride, const bool normalize, const void* pointer);

        void setAttribute(const unsigned int loc, unsigned int type, int amount, unsigned int stride, const bool normalize, const void* pointer);

        static Shader& getDefault();

    private:

        /// \brief Get location of uniform by name
        ///
        int getUniformLocation(const std::string& name);

        /// \brief Get location of attribute by name
        ///
        int getAttributeLocation(const std::string& name);

        
        unsigned int m_shaderProgram; ///< The OpenGL shader handle
                
    };
}

#endif

/// \class Shader
/// \ingroup Graphics
///
/// Do explanation about the class