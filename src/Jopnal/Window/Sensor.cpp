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
#include JOP_PRECOMPILED_HEADER_FILE

#ifndef JOP_PRECOMPILED_HEADER

    #include <Jopnal/Window/Sensor.hpp>

    #include <Jopnal/Window/SensorManager.hpp>
    #include <Jopnal/Core/DebugHandler.hpp>

#endif

//////////////////////////////////////////////


namespace jop
{
    bool Sensor::use(const Type sensorType)
    {
        if (SensorManager::getInstance().isAvailable(sensorType))
        {
            enable(sensorType);
            return true;
        }

        JOP_DEBUG_ERROR("Sensor with type ID (" << static_cast<size_t>(sensorType) << ") is not available for use!");

        return false;
    }

    //////////////////////////////////////////////

    void Sensor::enable(const Type sensorType)
    {
        SensorManager::getInstance().enable(sensorType);
    }

    //////////////////////////////////////////////

    void Sensor::disable(const Type sensorType)
    {
        SensorManager::getInstance().disable(sensorType);
    }

    //////////////////////////////////////////////

    glm::vec3 Sensor::getData(const Type sensorType)
    {
        return SensorManager::getInstance().getData(sensorType);
    }

    //////////////////////////////////////////////

    bool Sensor::getStatus(const Type sensorType)
    {
        return SensorManager::getInstance().getStatus(sensorType);
    }
}