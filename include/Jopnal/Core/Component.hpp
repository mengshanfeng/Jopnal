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

#ifndef JOP_COMPONENT_HPP
#define JOP_COMPONENT_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/SafeReferenceable.hpp>
#include <Jopnal/Utility/Message.hpp>
#include <string>

//////////////////////////////////////////////


namespace jop
{
    class Object;

    class JOP_API Component : public SafeReferenceable<Component>
    {
    private:

        JOP_DISALLOW_MOVE(Component);

        void operator =(const Component&) = delete;

        friend class Object;

    protected:

        /// \brief Copy constructor
        ///
        Component(const Component& other);

    public:

        /// \brief Constructor
        ///
        /// \param object Reference to the object this component will be bound to
        /// \param ID Unique component identifier
        ///
        Component(Object& object, const std::string& ID);

        /// \brief Virtual destructor
        ///
        virtual ~Component() = 0;


        /// \brief Copy this component
        ///
        /// This function exists for internal use only. Do not call directly.
        ///
        virtual Component* clone() const = 0;


        /// \brief Function to handle messages
        ///
        /// \param message String holding the message
        ///
        Message::Result sendMessage(const std::string& message);

        /// \brief Function to handle messages
        ///
        /// \param message String holding the message
        /// \param returnWrap Pointer to hold extra data
        ///
        Message::Result sendMessage(const std::string& message, Any& returnWrap);

        /// \brief Function to handle messages
        ///
        /// \param message The message
        ///
        Message::Result sendMessage(const Message& message);

        /// \brief Update function for component
        ///
        /// \param deltaTime Double holding delta time
        ///
        virtual void update(const float deltaTime);

        /// \brief Fixed update function for component
        ///
        /// \param timeStep Double holding time step
        ///
        virtual void fixedUpdate(const float timeStep);

        /// \brief Function to get components unique identifier m_ID
        ///
        const std::string& getID() const;

        /// \brief Function to set components unique identifier m_ID
        ///
        void setID(const std::string& ID);

        /// \brief Get the object this component is bound to
        ///
        /// \return Reference to the object
        ///
        WeakReference<Object> getObject();

        /// \copydoc jop::Component::getObject()
        ///
        WeakReference<const Object> getObject() const;

        /// \brief Check if this component is active
        ///
        /// This is the same as calling getObject()->isActive().
        ///
        /// \return True if active
        ///
        bool isActive() const;
        
    private:

        /// \brief Virtual sendMessage
        ///
        virtual Message::Result sendMessageImpl(const Message& message);

        std::string m_ID;                   ///< Identifier
        WeakReference<Object> m_objectRef;  ///< Reference to the object this component is bound to
    };
}

/// \brief Convenience macro for defining a component's/scene's clone function.
///
#define JOP_GENERIC_CLONE(className) virtual className* clone() const override{return new className(*this);}

#endif

/// \class Component
/// \ingroup core