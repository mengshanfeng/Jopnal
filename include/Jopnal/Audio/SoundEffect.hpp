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

#ifndef JOP_SOUNDEFFECT_HPP
#define JOP_SOUNDEFFECT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Audio/SoundSource.hpp>

//////////////////////////////////////////////


namespace jop
{
    class SoundBuffer;

    class JOP_API SoundEffect : public SoundSource
    {
    private:

        JOP_GENERIC_COMPONENT_CLONE(SoundEffect);

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object this component will be bound to
        ///
        SoundEffect(Object& object);

        /// \brief Destructor
        ///
        ~SoundEffect();


        /// \brief Update
        ///
        /// Automatically updates position.
        ///
        /// \param deltaTime The delta time
        ///
        void update(const float deltaTime) override;

        
        /// \brief Set the sound buffer
        ///
        /// \warning The bound buffer has to stay alive as long as it's being used
        ///
        /// \param buffer Reference to the buffer
        ///
        /// \return Reference to self
        ///
        SoundEffect& setBuffer(const SoundBuffer& buffer);

        /// \brief Play sound
        ///
        /// \return Reference to self
        ///
        /// \comm playEffect
        ///
        SoundEffect& play();

        /// \brief Play sound from start
        ///
        /// \return Reference to self
        ///
        /// \see stop()
        /// \see pause()
        ///
        SoundEffect& playReset();

        /// \brief Stop playing sound
        ///
        /// After this call, the playing offset will be at the beginning.
        ///
        /// \return Reference to self
        ///
        /// \see play()
        /// \see pause()
        ///
        /// \comm stopEffect
        ///
        SoundEffect& stop();

        /// \brief Pause sound
        ///
        /// The playing offset will be left at the current position.
        ///
        /// \return Reference to self
        ///
        /// \see play()
        /// \see stop()
        ///
        /// \comm pauseEffect
        ///
        SoundEffect& pause();

        /// \brief Set from which point the sound starts playing
        ///
        /// The value will be clamped to fit the buffer's length.
        ///
        /// \param time The time point as seconds
        ///
        /// \return Reference to self
        ///
        /// \comm setEffectOffset
        ///
        SoundEffect& setOffset(const float time);

        /// \brief Returns point where sound is playing
        ///
        /// \return The playing offset in seconds
        ///
        float getOffset() const;

        /// \brief Enable/disable looping
        ///
        /// If looping is enabled, the sound will loop indefinitely until stopped.
        ///
        /// \param loop True to set this sound to loop
        ///
        /// \return Reference to self
        ///
        /// \comm setEffectLoop
        ///
        SoundEffect& setLoop(const bool loop);

        /// \brief Check if this sound is looping
        ///
        /// \return True if looping
        ///
        bool isLooping() const;

    private:

        WeakReference<const SoundBuffer> m_buffer;  ///< SoundBuffer linked to owned source
        bool m_resetSound;                          ///< Check for not breaking ongoing sound
    };
}

/// \class jop::SoundEffect
/// \ingroup audio
///
/// Audio component that plays sound

#endif