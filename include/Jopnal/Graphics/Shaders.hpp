// Jopnal Engine C++ Library
// Copyright(c) 2016 Team Jopnal
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

//////////////////////////////////////////////

#ifndef JOP_SHADERS_HPP
#define JOP_SHADERS_HPP

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

        ///\brief default constructor
        ///
		Shader();

        ///\brief default destructor
        ///
		~Shader();

        ///\brief Load method for loading shaders
        ///
        ///\param shaders takes a const string with all shader code inside
        ///
        bool load(const std::string& shaders) override;

        ///\brief method setting matrix 4x4 with unique name
        ///
        ///\param name unique name
        ///\param matrix 4x4 matrix
        ///
        void setUniform(const std::string& name, const glm::mat4& matrix);

        ///\brief method setting 3x3 matrix with unique name
        ///
        ///\param name unique name
        ///\param matrix 3x3 matrix
        ///
        void setUniform(const std::string& name, const glm::mat3& matrix);
        
        ///\brief method setting vector with 4 dimensions and unique name
        ///
        ///\param name unique name
        ///\param vector 4 dimensional vector
        ///
        void setUniform(const std::string& name, const glm::vec4& vector);

        ///\brief method setting texture with unique name and Uint
        ///
        ///\param name unique name
        ///\param texture Uint 
        ///
        void setUniform(const std::string& name, const jop::Texture& );


	private:

        int getUniformLocation(const std::string& name); ///< get location of uniform by name
        
        unsigned int m_shader; ///< An internal Uint
                
	};
}

#endif

/// \class Shader
/// \ingroup Graphics
///
/// Do explanation about the class