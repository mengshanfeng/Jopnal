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

namespace detail
{
    template<typename T, bool isFloat = std::is_floating_point<T>::value>
    struct distribution  
    {
        typedef std::uniform_int_distribution<T>dist;
    };
    template<typename T>
    struct distribution<T, true>
    {
        typedef std::uniform_real_distribution<T>dist;
    };
}

//////////////////////////////////////////////

template<typename T>
T Randomizer::range(const T dStart,const T dEnd)
{
    return detail::distribution<T>::dist (dStart, dEnd)(m_randomGenerator);
}

//////////////////////////////////////////////

template<typename T>
T Randomizer::operator()(const T dStart, const T dEnd)
{
    return range(dStart,dEnd);
}

//////////////////////////////////////////////
template<typename T>
glm::vec2 Randomizer::insideCircle(T radius)
{
    const float angle = range(0.f, glm::two_pi<float>());
    const float rad = std::sqrt(range(0.f, 1.f)) * radius;

    return glm::vec2(rad * glm::cos(angle), rad * glm::sin(angle));
}

template<typename T>
glm::vec3 Randomizer::insideSphere(T radius)
{
    glm::vec3 s(range(-1.0f, 1.0f), range(-1.0f, 1.0f), range(-1.0f, 1.0f));

    return glm::normalize(s) * (std::sqrt(range(0.f, 1.f)) * radius);
}