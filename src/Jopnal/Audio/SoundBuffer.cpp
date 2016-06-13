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
    SoundBuffer::SoundBuffer(const std::string& name)
        : Resource      (name)
    {}

    SoundBuffer::SoundBuffer(const SoundBuffer& other, const std::string& newName)
        : Resource      (newName)
    {}

    SoundBuffer::~SoundBuffer()
    {}

    //////////////////////////////////////////////

    bool SoundBuffer::load(const std::string& path)
    {
        std::vector<uint8> buf;
        FileLoader::readBinaryfile(path, buf);
        
        return false;
    }

    //////////////////////////////////////////////

    bool SoundBuffer::load(const int id)
    {
        std::vector<uint8> buf;

        if (!FileLoader::readResource(id, buf))
            return false;

        return false;
    }

    //////////////////////////////////////////////

    SoundBuffer& SoundBuffer::getDefault()
    {
        static WeakReference<SoundBuffer> defBuf;
        
        if (defBuf.expired())
        {
            defBuf = static_ref_cast<SoundBuffer>(ResourceManager::getEmptyResource<SoundBuffer>("jop_default_sound").getReference());

            JOP_ASSERT_EVAL(defBuf->load(JOP_RES_DEFAULT_SOUND), "Failed to load default Sound!");

            defBuf->setPersistence(0);
        }

        return *defBuf;
    }
}