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

#ifndef JOP_SOUNDSOURCE_HPP
#define JOP_SOUNDSOURCE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/Message.hpp>
#include <memory>
#include <string>

//////////////////////////////////////////////

namespace sf
{
    class SoundSource;
}

namespace jop
{
    class Object;

    class JOP_API SoundSource : public Component
    {
    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object this component will be bound to
        /// \param ID Unique component identifier
        ///
        SoundSource(Object& object, const std::string& ID);


        /// \brief Copy constructor
        ///
        SoundSource(const SoundSource& other);

        /// \brief Check for copy constructor's need
        ///
        JOP_GENERIC_CLONE(SoundSource);

        /// \brief Virtual destructor
        ///
        ~SoundSource();

        /// \brief Automatically updates position
        ///
        void update(const float deltaTime)override;

        /// \brief Set sound's volume
        ///
        /// \param Float 0-100.0f default is 100.0f
        ///
        SoundSource& setVolume(const float vol);

        /// \brief Returns float volume
        ///
        float getVolume();

        /// \brief Set pitch
        ///
        /// \param Float pitch value default is 1 min is 0.0 crashes after 8.9
        ///
        SoundSource& setPitch(const float value);

        /// \brief Returns pitch
        ///
        float getPitch();

        /// \brief Toggle listener on/off
        ///
        /// \param Boolean true plays sound at listener's position and false enables spatialization in 3d space
        ///
        SoundSource& setListener(const bool toggle);

        /// \brief Returns relativity to listener
        ///
        bool getListener();

        /// \brief Change stream's fadein distance
        ///
        /// \param Attenuation 0-100.0f and minDistance 1<x
        ///
        SoundSource& setAttenuation(const float at);
        
        /// \brief Change sound's distance when it is heard in max volume
        ///
        /// \param Attenuation 0-100.0f and minDistance 1<x
        ///
        SoundSource& setMinDistance(const float min);
       
        /// \brief Returns fadein value
        ///
        float getAttenuation();

        /// \brief Returns distance of max volume
        ///
        float getMinDistance();

    protected:
        /// \brief  Automatic check if sound is allowed to play
        ///
        /// \param Deltatime for calculation
        ///
        void allowSound(const float deltaTime);

        std::unique_ptr<sf::SoundSource> m_sound;      ///< Unique audio sample
        float m_speedCounter;                          ///< Counter for speed of sound
        bool m_playWithSpeed;                          ///< Calculate when sound is allowed to play
        bool m_playOnce;                               ///< Breaks link from update to calculate sound()
        bool m_resetSound;                             ///< When using speed of sound saves value from play(bool)
    };
}

#endif

/// \class SoundSource
/// \ingroup Audio
///
/// Base class for audio component