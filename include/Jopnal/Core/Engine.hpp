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

#ifndef JOP_ENGINE_HPP
#define JOP_ENGINE_HPP

// Headers
#include <Jopnal/Header.hpp>
#include <Jopnal/Utility/Message.hpp>
#include <memory>
#include <vector>
#include <atomic>

//////////////////////////////////////////////


namespace jop
{
    class Subsystem;
    class Scene;

    class JOP_API Engine final
    {
    private:

        friend class StateLoader;

    public:

        /// Engine state
        ///
        /// No matter the state, sub systems will always be running normally, except
        /// in ZeroDelta state, the delta time passed to sub systems is still zero.
        ///
        enum class State
        {
            Running,    ///< Run normally
            ZeroDelta,  ///< Run normally, except delta time is always zero
            RenderOnly, ///< Only render, don't update
            Frozen      ///< No rendering or updating
        };

    public:

        /// \brief Constructor
        ///
        /// This will initialize the engine and all associated systems.
        /// Project name must not be empty, as it will be used to create
        /// the necessary config directories.
        ///
        /// The engine object must stay alive until exit is called and the
        /// main loop is returned from.
        ///
        /// \param projectName The project name
        /// \param argc Number of arguments passed from main()
        /// \param argv The argument array passed from main()
        ///
        Engine(const std::string& projectName, int argc, char* argv[]);

        /// \brief Destructor
        ///
        /// This will deinitialize the engine, so that the program can exit.
        /// Alternatively, a new jop::Engine object can be created.
        ///
        ~Engine();


        /// \brief Load the default subsystem configuration
        ///
        void loadDefaultConfiguration();

        /// \brief Run the main loop
        ///
        /// The main loop will run until exit() is called.
        ///
        /// \return An integer return value. To be used as the main() return value
        ///
        static int runMainLoop();

        /// \brief Advance a single frame
        ///
        /// When state is Running, this function has no effect.
        ///
        static void advanceFrame();


        /// \brief Create a scene
        ///
        /// This function will construct the scene and then set it as active.
        /// The previously active scene will be discarded.
        ///
        /// TODO: Rethink the scene system so that it becomes more flexible (threaded loading?)
        ///
        /// \param args The arguments to be used in the scene's construction
        ///
        /// \return A reference to the newly created scene
        ///
        template<typename T, typename ... Args>
        static T& createScene(Args&&... args);

        /// \brief Check if there's a current scene
        ///
        /// \return True if there's a current scene
        ///
        static bool hasCurrentScene();

        /// \brief Get the current scene
        ///
        /// This function asserts that the scene has been loaded first with createScene().
        ///
        /// \return Reference to the scene
        ///
        static Scene& getCurrentScene();


        /// \brief Create a subsystem
        ///
        /// \param args The arguments to be used in the subsystem's construction
        ///
        /// \return A reference to the newly created subsystem
        ///
        template<typename T, typename ... Args>
        static T& createSubsystem(Args&&... args);

        /// \brief Get a subsystem using type info
        ///
        /// \return Pointer to the subsystem. Nullptr if not found
        ///
        template<typename T>
        static T* getSubsystem();

        /// \brief Get a subsystem using subsystem ID
        ///
        /// \return Pointer to the subsystem. Nullptr if not found
        ///
        template<typename T>
        static T* getSubsystem(const std::string& ID);

        /// \brief Get a subsystem
        ///
        /// \param ID Identifier of the subsystem
        ///
        /// \return Pointer to the subsystem. Nullptr if not found
        ///
        static Subsystem* getSubsystem(const std::string& ID);

        /// \brief Remove a subsystem
        ///
        /// \param ID Identifier of the subsystem to be removed
        ///
        static bool removeSubsystem(const std::string& ID);


        /// \brief Check if the engine is running
        ///
        /// \return True if an engine object exists & m_running is true
        ///
        static bool exiting();

        /// \brief Exit the main loop
        ///
        /// This function will have no effect if no jop::Engine exits at the
        /// time of the call. The main loop returns only after the current
        /// frame has been processed.
        ///
        static void exit();


        /// \brief Sets paused to private m_paused member
        ///
        /// \param paused Boolean to set m_paused
        ///
        static void setState(const State state);

        /// \brief Sets paused state to all update methods not including subsystems also returns m_paused
        ///
        static State getState();


        /// \brief Set the global delta time scalar
        ///
        /// \param The scalar to set
        ///
        static void setDeltaScale(const float scale);

        /// \brief Get the global delta time scalar
        ///
        /// \return The global delta time scalar
        ///
        static float getDeltaScale();


        /// \brief Send a message to the whole engine
        ///
        /// \param message String holding message
        ///
        static Message::Result sendMessage(const std::string& message);

        /// \brief Send a message to the whole engine
        ///
        /// \param message String holding message
        /// \param returnWrap Pointer to hold extra data
        ///
        static Message::Result sendMessage(const std::string& message, Any& returnWrap);

        /// \brief Function to handle messages
        ///
        /// \param message The message
        ///
        static Message::Result sendMessage(const Message& message);


        /// \brief Get the shared scene
        ///
        /// The shared scene exists for the purpose of being able to have layers & objects
        /// that are shared between scenes. This makes it possible to have general-purpose
        /// functionality without having to take care of it in every scene separately.
        ///
        /// The existence of the shared scene will not be checked. Only
        /// call this function when you know there exists a valid jop::Engine
        /// object. If necessary, you can check by using jop::Engine::isRunning().
        ///
        /// \return Reference to the shared scene
        ///
        static Scene& getSharedScene();

        /// \brief Check if a shared scene exists
        ///
        /// \return True if the shared scene exists
        ///
        static bool hasSharedScene();

        /// \brief Set the shared scene
        ///
        /// This will replace the previous shared scene with a new one of the given type.
        ///
        /// \param args Arguments to use in the scene's construction
        ///
        /// \return Reference to the new scene
        ///
        template<typename T, typename ... Args>
        static T& setSharedScene(Args&&... args);


        /// \brief Get the total time since Engine construction
        ///
        /// \return Time in seconds
        ///
        static double getTotalTime();


    private:

        static Engine* m_engineObject;                          ///< The single Engine instance

        std::vector<std::unique_ptr<Subsystem>> m_subsystems;   ///< A vector containing the subsystems
        double m_totalTime;                                     ///< The total time
        std::unique_ptr<Scene> m_currentScene;                  ///< The current scene
        std::unique_ptr<Scene> m_sharedScene;                   ///< The shared scene
        std::atomic<bool> m_exit;                               ///< Should the engine exit?
        State m_state;                                          ///< Current state
        float m_deltaScale;                                     ///< The global delta scale
        bool m_advanceFrame;                                    ///< Advance a single frame when not paused?
    };

    /// \brief Get the project name
    ///
    /// \return A reference to the internal string containing the project name
    ///
    JOP_API const std::string& getProjectName();

    /// \brief Broadcast a message to the whole engine
    ///
    /// This is the same as calling jop::Engine::sendMessage
    ///
    /// \param message String holding message
    ///
    /// \return The message result
    ///
    JOP_API Message::Result broadcast(const std::string& message);

    /// \brief Broadcast a message to the whole engine
    ///
    /// This is the same as calling jop::Engine::sendMessage
    ///
    /// \param message String holding message
    /// \param returnWrap Pointer to hold extra data
    ///
    /// \return The message result
    ///
    JOP_API Message::Result broadcast(const std::string& message, Any& returnWrap);

    /// \brief Broadcast a message to the whole engine
    ///
    /// This is the same as calling jop::Engine::sendMessage
    ///
    /// \param message The message
    ///
    /// \return The message result
    ///
    JOP_API Message::Result broadcast(const Message& message);

    // Include the template implementation file
    #include <Jopnal/Core/Inl/Engine.inl>
}

/// \brief Initializing the engine with the default configuration
///
/// The engine should be constructed after entering main.
///
/// \param projectName The project name
/// \param argc Number of arguments passed from main()
/// \param argv The argument array passed from main()
///
#define JOP_ENGINE_INIT(projectName, argc, argv) jop::Engine jop_engine(projectName, argc, argv); jop_engine.loadDefaultConfiguration();

/// \brief Run the main loop
///
/// This macro must appear in the same scope as JOP_ENGINE_INIT
///
#define JOP_MAIN_LOOP jop::Engine::runMainLoop();

#endif

/// \class Engine
/// \ingroup core
///
/// 