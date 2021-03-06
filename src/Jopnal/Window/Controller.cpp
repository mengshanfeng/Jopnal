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

    #include <Jopnal/Window/Controller.hpp>

    #include <Jopnal/Core/Android/ActivityState.hpp>
    #include <Jopnal/Core/SettingManager.hpp>
    #include <Jopnal/Core/Engine.hpp>
    #include <Jopnal/Window/InputEnumsImpl.hpp>

    #ifdef JOP_OS_DESKTOP
        #include <GLFW/glfw3.h>
    #endif

#endif

//////////////////////////////////////////////


namespace jop
{
    int Controller::controllersPresent()
    {
    #if defined(JOP_OS_DESKTOP)

        static int count = 0;

        if (!count)
            for (int i = 0; i < GLFW_JOYSTICK_LAST && glfwJoystickPresent(i) == GLFW_TRUE; ++i, count += 1);

        return count;

    #elif defined(JOP_OS_ANDROID)
        return detail::ActivityState::get()->controllerPresent;

    #endif
    }

    //////////////////////////////////////////////

    bool Controller::isControllerPresent(const int index)
    {
    #if defined(JOP_OS_DESKTOP)
        return glfwJoystickPresent(index) == GL_TRUE;

    #elif defined(JOP_OS_ANDROID)

        return index == 0 && detail::ActivityState::get()->controllerPresent;

    #endif
    }

    //////////////////////////////////////////////

    bool Controller::isButtonDown(const int index, const int button)
    {
    #if defined(JOP_OS_DESKTOP)

        int count = 0;
        const unsigned char* buttons = glfwGetJoystickButtons(index, &count);

        return (button < count) ? buttons[button] == GLFW_PRESS : false;

    #elif defined(JOP_OS_ANDROID)
        return detail::ActivityState::get()->activeControllerButtons[button];

    #endif
    }

    //////////////////////////////////////////////

    float Controller::getAxisOffset(const int index, const int axis)
    {
        if (isControllerPresent(index))
        {
        #if defined(JOP_OS_DESKTOP)

            int count = 0;
            const float* axes = glfwGetJoystickAxes(index, &count);

            switch (axis)
            {
                case XBox::Axis::LeftStickX:
                    return axes[0];

                case XBox::Axis::LeftStickY:
                    return axes[1];

                case XBox::Axis::RightStickX:
                    return axes[2];

                case XBox::Axis::RightStickY:
                    return axes[3];

                case XBox::Axis::LTrigger:
                {
                    if (count >= 5)
                        return ((axes[4] + 1.f) / 2.f);

                    break;
                }
                case XBox::Axis::RTrigger:
                {
                    if (count >= 6)
                        return ((axes[5] + 1.f) / 2.f);
                }
            }

        #elif defined(JOP_OS_ANDROID)

            auto state = detail::ActivityState::get();

            switch (axis)
            {
                case XBox::Axis::LeftStickX:
                    return state->activeAxes[0];

                case XBox::Axis::LeftStickY:
                    return state->activeAxes[1];

                case XBox::Axis::RightStickX:
                    return state->activeAxes[2];

                case XBox::Axis::RightStickY:
                    return state->activeAxes[3];

                case XBox::Axis::LTrigger:
                    return state->activeAxes[4];

                case XBox::Axis::RTrigger:
                    return state->activeAxes[5];
            }

        #endif
        }

        return 0.f;
    }
}