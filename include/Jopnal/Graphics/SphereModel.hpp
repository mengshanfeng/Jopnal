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

#ifndef JOP_SPHEREMODEL_HPP
#define JOP_SPHEREMODEL_HPP

//Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Graphics/Model.hpp>

//////////////////////////////////////////////


namespace jop
{
    class JOP_API SphereModel : public Model
    {
    public:

        SphereModel();

        /// \brief Creates a 3D sphere
        ///
        /// High values for sectors and rings will result in better looking results, but may
        /// affect performance heavily.
        ///
        /// \param radius The radius of the sphere
        /// \param rings How many rings will the sphere have
        /// \param sectors How many sectors the circle will get divided to
        ///
        SphereModel(const float radius, const unsigned int rings, const unsigned int sectors);


        bool load(const float radius, const unsigned int rings, const unsigned int sectors);

    };
}

#endif

/// \class SphereModel
/// \ingroup Graphics
///
/// Do explanation about the class