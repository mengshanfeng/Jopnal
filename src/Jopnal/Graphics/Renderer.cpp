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
    namespace detail
    {
        class DrawableSorter
        {
            const glm::vec3& m_camPos;

        public:

            DrawableSorter(const Camera& cam)
                : m_camPos(cam.getObject()->getGlobalPosition())
            {}

            bool operator()(const Drawable* first, const Drawable* second) const
            {
                auto& pos1 = first->getObject()->getGlobalPosition();
                auto& pos2 = second->getObject()->getGlobalPosition();

                return glm::distance2(m_camPos, pos1) < glm::distance2(m_camPos, pos2);
            }
        };
    }

    //////////////////////////////////////////////

    Renderer::Renderer(const RenderTarget& mainTarget)
        : m_lights          (),
          m_cameras         (),
          m_drawables       (),
          m_envRecorders    (),
          m_mask            (1),
          m_mainTarget      (mainTarget)

        #ifdef JOP_DEBUG_MODE
        , m_physicsWorld   (nullptr)
        #endif
    {
        GlState::setDepthTest(true);
        GlState::setFaceCull(true);
        GlState::setSeamlessCubemap(true);
        GlState::setBlendFunc(true);
        GlState::setFramebufferSrgb(true);
    }

    Renderer::~Renderer()
    {}

    //////////////////////////////////////////////

    void Renderer::setMask(const uint32 mask)
    {
        m_mask = mask;
    }

    //////////////////////////////////////////////

    uint32 Renderer::getMask() const
    {
        return m_mask;
    }

    //////////////////////////////////////////////

    void Renderer::bind(const LightSource& light)
    {
        m_lights.insert(&light);
    }

    //////////////////////////////////////////////

    void Renderer::bind(const Camera& camera)
    {
        m_cameras.insert(&camera);
    }

    //////////////////////////////////////////////

    void Renderer::bind(const Drawable& drawable)
    {
        m_drawables.insert(&drawable);
    }

    //////////////////////////////////////////////

    void Renderer::bind(EnvironmentRecorder& envRecorder)
    {
        m_envRecorders.insert(&envRecorder);
    }

    //////////////////////////////////////////////

    void Renderer::unbind(const LightSource& light)
    {
        m_lights.erase(&light);
    }

    //////////////////////////////////////////////

    void Renderer::unbind(const Camera& camera)
    {
        m_cameras.erase(&camera);
    }

    //////////////////////////////////////////////

    void Renderer::unbind(const Drawable& drawable)
    {
        m_drawables.erase(&drawable);
    }

    //////////////////////////////////////////////

    void Renderer::unbind(EnvironmentRecorder& envRecorder)
    {
        m_envRecorders.erase(&envRecorder);
    }

    //////////////////////////////////////////////

    void Renderer::draw()
    {
        // Dummy LightContainer to pass into drawables when no lights were selected
        static const LightContainer dummyLightCont;

        // Render shadow maps
        for (auto light : m_lights)
        {
            if (light->isActive() && (m_mask & light->getRenderMask()) != 0)
                light->drawShadowMap(m_drawables);
        }

        // Render environment maps
        for (auto envmap : m_envRecorders)
        {
            if (envmap->isActive() && (m_mask & envmap->getRenderMask()) != 0)
                envmap->record();
        }

        m_mainTarget.bind();

        // Render objects
        for (uint32 i = 1, done = 0; i != 0 && m_mask > done; i <<= 1, done |= i)
        {
            if ((m_mask & i) == 0)
                continue;

            for (auto cam : m_cameras)
            {
                const uint32 camMask = cam->getRenderMask();
                if (!cam->isActive() || (camMask & i) == 0)
                    continue;

                cam->getRenderTexture().bind();
                cam->applyViewport(m_mainTarget);

                std::vector<const Drawable*> sorted;
                sorted.reserve(m_drawables.size());
                
                for (auto& drawable : m_drawables)
                {
                    const uint32 groupBit = 1 << drawable->getRenderGroup();
                    if (drawable->isActive() && drawable->getModel().isValid() && (i & groupBit) > 0 && (camMask & groupBit) > 0)
                        sorted.push_back(drawable);
                }

                std::sort(sorted.begin(), sorted.end(), detail::DrawableSorter(*cam));

                for (auto& drawable : sorted)
                {
                    if (drawable->receiveLights())
                    {
                        // Select lights
                        LightContainer lights;
                        chooseLights(*drawable, lights);
                        drawable->draw(*cam, lights);
                    }
                    else
                        drawable->draw(*cam, dummyLightCont);
                }

            #ifdef JOP_DEBUG_MODE
                if (m_physicsWorld)
                    m_physicsWorld->draw(*cam);
            #endif
            }
        }
    }

    //////////////////////////////////////////////

    void Renderer::chooseLights(const Drawable& drawable, LightContainer& lights) const
    {
        for (auto l : m_lights)
        {
            auto& container = lights[l->getType()];

            if (!l->isActive() || LightSource::getMaximumLights(l->getType()) <= container.size())
                continue;

            const uint32 lightMask = l->getRenderMask();
            if ((m_mask & lightMask) == 0 || (lightMask & (1 << drawable.getRenderGroup())) == 0)
                continue;

            if (drawable.lightTouches(*l))
                container.push_back(l);
        }
    }
}