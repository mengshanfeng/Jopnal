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


namespace
{
    using namespace jop;

    const char* const ns_fileVersion = "0.1";
    const char* const ns_sceneField = "scene";
    const char* const ns_versionField = "version";
    const char* const ns_typeField = "type";
    const char* const ns_sharedSceneField = "sharedscene";
    const char* const ns_layerField = "layers";
    const char* const ns_objectField = "objects";
    const char* const ns_dataField = "data";

    enum
    {
        CompID,
        LayerID,
        SceneID,
        SubID,

        LoadID = 0,
        SaveID
    };
}

namespace jop
{
    StateLoader& StateLoader::getInstance()
    {
        static StateLoader instance;
        return instance;
    }

    //////////////////////////////////////////////

    bool StateLoader::saveState(const std::string& path, const bool scene, const bool sharedScene, const bool subsystems)
    {
        if (!Engine::m_engineObject)
            return false;

        json::Document doc;
        doc.SetObject();

        doc.AddMember(json::StringRef(ns_versionField), json::StringRef(ns_fileVersion), doc.GetAllocator());

        if (subsystems)
        {

        }

        if (sharedScene && !saveScene(*Engine::m_engineObject->m_sharedScene, doc.AddMember(json::StringRef(ns_sharedSceneField), json::kObjectType, doc.GetAllocator())[ns_sharedSceneField], doc.GetAllocator(), path))
            return false;

        if (scene && !saveScene(*Engine::m_engineObject->m_currentScene, doc.AddMember(json::StringRef(ns_sceneField), json::kObjectType, doc.GetAllocator())[ns_sceneField], doc.GetAllocator(), path))
            return false;

        json::StringBuffer buffer;
        json::PrettyWriter<json::StringBuffer> writer(buffer);
        doc.Accept(writer);

        if (!FileLoader::write(FileLoader::Directory::Resources, path + ".jop", buffer.GetString(), buffer.GetSize()))
            return false;

        return true;
    }

    //////////////////////////////////////////////

    bool StateLoader::loadState(const std::string& path, const bool scene, const bool sharedScene, const bool subsystems)
    {
        std::string buf;
        if (!FileLoader::read(path + ".jop", buf))
            return false;

        json::Document doc;
        doc.ParseInsitu<0>(&buf[0]);

        if (!json::checkParseError(doc))
            return false;

        // Handle version
        if (!doc.HasMember(ns_versionField))
            JOP_DEBUG_WARNING("State file doesn't have the version defined, assuming latest (" << ns_fileVersion << ")")
        else if (std::strcmp(doc[ns_versionField].GetString(), ns_fileVersion) != 0)
            JOP_DEBUG_WARNING("The state file version doesn't match the current library version. Attempting to load anyway...");

        // Load subsystems?
        if (subsystems)
        {
            // TODO Implement, remember: a complete success is required, otherwise original state must be returned
        }

        std::unique_ptr<Scene> sharedScenePtr;

        // Load the shared scene?
        if (sharedScene && (doc.HasMember(ns_sharedSceneField) && doc[ns_sharedSceneField].IsObject() && Engine::m_engineObject && !loadScene(sharedScenePtr, doc[ns_sharedSceneField], path)))
            return false;

        std::unique_ptr<Scene> scenePtr;

        // Load scene?
        if (scene && (doc.HasMember(ns_sceneField) && doc[ns_sceneField].IsObject() && Engine::m_engineObject && !loadScene(scenePtr, doc[ns_sceneField], path)))
            return false;

        // Finally assign the pointers
        if (subsystems)
        {

        }
        if (sharedScene)
        {
            if (sharedScenePtr)
                Engine::m_engineObject->m_sharedScene = std::move(sharedScenePtr);
            else
                return false;
        }
        if (scene)
        {
            if (scenePtr)
                Engine::m_engineObject->m_currentScene = std::move(scenePtr);
            else
                return false;
        }

        return true;
    }

    //////////////////////////////////////////////

    bool StateLoader::loadScene(std::unique_ptr<Scene>& scene, const json::Value& data, const std::string& path)
    {
        const auto& sceneCont = std::get<SceneID>(m_loaderSavers);

        // Does the scene object have a type?
        if (data.HasMember(ns_typeField) && data[ns_typeField].IsString())
        {
            auto itr = sceneCont.find(data[ns_typeField].GetString());

            // Has the load function been registered for this scene type?
            if (itr != sceneCont.end())
            {
                json::Value v(json::kObjectType);

                // Attempt to create the scene
                if (std::get<LoadID>(itr->second)(scene, data.HasMember(ns_dataField) && data[ns_dataField].IsObject() ? data[ns_dataField] : v))
                {
                    // Attempt to load layers
                    if (data.HasMember(ns_layerField) && data[ns_layerField].IsArray() && !data[ns_layerField].Empty())
                    {
                        if (!loadLayers(scene, data[ns_layerField], path))
                        {
                            JOP_DEBUG_ERROR("Couldn't load scene state, a layer reported loading failure: " << path);
                            return false;
                        }
                    }

                    // Attempt to load objects
                    if (data.HasMember(ns_objectField) && data[ns_objectField].IsArray() && !data[ns_objectField].Empty())
                    {
                        if (!loadObjects(scene, data[ns_objectField], path))
                        {
                            JOP_DEBUG_ERROR("Couldn't load scene state, an object reported loading failure: " << path);
                            return false;
                        }
                    }
                }
                else
                {
                    JOP_DEBUG_ERROR("Couldn't load scene state, the registered load function reported failure: " << path);
                    return false;
                }
            }
            else
            {
                JOP_DEBUG_ERROR("Couldn't load scene state, scene type (\"" << data[ns_typeField].GetString() << "\") not registered: " << path);
                return false;
            }
        }
        else
        {
            JOP_DEBUG_ERROR("Couldn't load scene state, missing type info: " << path);
            return false;
        }

        return true;
    }

    //////////////////////////////////////////////

    bool StateLoader::loadLayers(std::unique_ptr<Scene>& scene, const json::Value& data, const std::string& path)
    {
        const auto& layerCont = std::get<LayerID>(m_loaderSavers);

        // Load layers
        for (auto& i : data)
        {
            if (!i.IsObject())
                continue;

            if (i.HasMember(ns_typeField) && i[ns_typeField].IsString())
            {
                auto itr = layerCont.find(i[ns_typeField].GetString());

                if (itr != layerCont.end())
                {
                    std::unique_ptr<Layer> ptr;

                    if (i.HasMember(ns_dataField) && i[ns_dataField].IsObject() && std::get<LoadID>(itr->second)(ptr, i[ns_dataField]))
                        scene->m_layers.push_back(std::shared_ptr<Layer>(ptr.release()));
                    else
                    {
                        JOP_DEBUG_ERROR("Couldn't load layer state, data object missing or the registered load function reported failure: " << path);
                        return false;
                    }
                }
                else
                {
                    JOP_DEBUG_ERROR("Couldn't load layer state, layer type (\"" << i[ns_typeField].GetString() << "\") not registered: " << path);
                    return false;
                }
            }
            else
            {
                JOP_DEBUG_ERROR("Couldn't create layer, no type specified: " << path);
                return false;
            }
        }

        // Link layers, we can assume all the objects are valid since they went through the previous loop
        const char* const boundLayersField = "boundlayers";

        for (auto& i : data)
        {
            if (!i.IsObject())
                continue;

            // It can be assumed that the id exists and is of right type
            std::weak_ptr<Layer> weakCurrLayer = scene->getLayer(i[ns_dataField]["id"].GetString());

            if (weakCurrLayer.expired())
            {
                JOP_DEBUG_WARNING("Layer with id \"" << i["id"].GetString() << "\" expired while loading scene: " << path);
                continue;
            }

            if (i.HasMember(boundLayersField) && i[boundLayersField].IsArray())
            {
                Layer& currLayer = *weakCurrLayer.lock();

                for (auto& j : i[boundLayersField])
                {
                    std::weak_ptr<Layer> bound;
                    if (j.IsString() && !(bound = scene->getLayer(j.GetString())).expired())
                        currLayer.bindOtherLayer(*std::static_pointer_cast<Layer>(bound.lock()));
                    else
                        JOP_DEBUG_WARNING("Couldn't bind layer to \"" << currLayer.getID() << "\". Array element not a string or layer couldn't be found: " << path);
                }
            }
        }

        return true;
    }

    //////////////////////////////////////////////

    bool StateLoader::loadObjects(std::unique_ptr<Scene>& scene, const json::Value& data, const std::string& path)
    {
        for (auto& i : data)
        {
            if (!i.IsObject())
                continue;

            const char* id = (i.HasMember("id") && i["id"].IsString() ? i["id"].GetString() : "");

            scene->createObject(id);
            if (!loadObject(*scene->m_objects.back(), *scene, i, path))
            {
                JOP_DEBUG_ERROR("Failed to load object with id \"" << id << "\": " << path);
                return false;
            }
        }

        return true;
    }

    //////////////////////////////////////////////

    bool StateLoader::loadObject(Object& obj, const Scene& scene, const json::Value& data, const std::string& path)
    {
        const char* const activeField = "active";
        const char* const componentsField = "components";
        const char* const childrenField = "children";
        const char* const transformField = "transform";

        if (data.HasMember(activeField) && data[activeField].IsBool())
            obj.setActive(data[activeField].GetBool());

        if (data.HasMember(transformField) && data[transformField].IsArray() && data[transformField].Size() >= 10)
        {
            auto& val = data[transformField];

            for (auto& i : val)
            {
                if (!i.IsDouble())
                {
                    JOP_DEBUG_WARNING("Encountered unexpected transform value(s) while loading object with id \"" << obj.getID() << "\": " << path);
                    goto SkipTransform;
                }
            }

            obj.setPosition(static_cast<float>(val[0u].GetDouble()),
                            static_cast<float>(val[1u].GetDouble()),
                            static_cast<float>(val[2u].GetDouble()));

            obj.setScale(static_cast<float>(val[3u].GetDouble()),
                         static_cast<float>(val[4u].GetDouble()),
                         static_cast<float>(val[5u].GetDouble()));

            obj.setRotation(glm::quat(static_cast<float>(val[6u].GetDouble()),
                                      static_cast<float>(val[7u].GetDouble()),
                                      static_cast<float>(val[8u].GetDouble()),
                                      static_cast<float>(val[9u].GetDouble())));
        }

        SkipTransform:

        if (data.HasMember(componentsField) && data[componentsField].IsArray())
        {
            auto& compCont = std::get<CompID>(m_loaderSavers);

            for (auto& i : data[componentsField])
            {
                if (i.IsObject() && i.HasMember(ns_typeField) && i[ns_typeField].IsString())
                {
                    auto itr = compCont.find(i[ns_typeField].GetString());

                    if (itr != compCont.end())
                    {
                        if (!i.HasMember(ns_dataField) || !i[ns_dataField].IsObject() || !std::get<LoadID>(itr->second)(obj, scene, i[ns_dataField]))
                        {
                            JOP_DEBUG_ERROR("Couldn't load component state, registered load function reported failure: " << path);
                            return false;
                        }
                    }
                    else
                    {
                        JOP_DEBUG_ERROR("Couldn't load component state, component type (\"" << i[ns_typeField].GetString() << "\") not registered: " << path);
                        return false;
                    }
                }
                else
                {
                    JOP_DEBUG_ERROR("Failed to load component, no type specified: " << path);
                    return false;
                }
            }
        }

        if (data.HasMember(childrenField) && data[childrenField].IsArray())
        {
            for (auto& i : data[childrenField])
            {
                const char* id = (i.HasMember("id") && i["id"].IsString() ? i["id"].GetString() : "");

                obj.createChild(id);
                if (!loadObject(*obj.m_children.back(), scene, i, path))
                {
                    JOP_DEBUG_ERROR("Failed to load child object with id \"" << id << "\": " << path);
                    return false;
                }
            }
        }

        return true;
    }

    //////////////////////////////////////////////

    bool StateLoader::saveScene(const Scene& scene, json::Value& data, json::Value::AllocatorType& alloc, const std::string& path)
    {
        const auto& sceneCont = std::get<SceneID>(m_loaderSavers);
        const auto& nameMap = std::get<std::tuple_size<decltype(m_loaderSavers)>::value - 1>(m_loaderSavers);

        // Type name iterator
        auto itrRedir = nameMap.find(std::type_index(typeid(scene)));
        // Function iterator
        auto itr = sceneCont.end();

        if (itrRedir == nameMap.end() || (itr = sceneCont.find(itrRedir->second)) == sceneCont.end())
        {
            JOP_DEBUG_ERROR("Couldn't save scene, type (name) not registered: " << path);
            return false;
        }

        data.AddMember(json::StringRef(ns_typeField), json::StringRef(itrRedir->second.c_str()), alloc);

        if (std::get<SaveID>(itr->second)(scene, data.AddMember(json::StringRef(ns_dataField), json::kObjectType, alloc)[ns_dataField], alloc))
        {
            // Attempt to save layers
            if (!scene.m_layers.empty())
            {
                if (!saveLayers(scene, data.AddMember(json::StringRef(ns_layerField), json::kArrayType, alloc)[ns_layerField], alloc, path))
                {
                    JOP_DEBUG_ERROR("Couldn't save scene, a layer failed to save: " << path);
                    return false;
                }
            }

            // Attempt to save objects
            if (!scene.m_objects.empty())
            {
                if (!saveObjects(scene, data.AddMember(json::StringRef(ns_objectField), json::kArrayType, alloc)[ns_objectField], alloc, path))
                {
                    JOP_DEBUG_ERROR("Couldn't save scene, an object failed to save: " << path);
                    return false;
                }
            }
        }
        else
        {
            JOP_DEBUG_ERROR("Couldn't save scene, registered save function reported failure: " << path);
            return false;
        }

        return true;
    }

    //////////////////////////////////////////////

    bool StateLoader::saveLayers(const Scene& scene, json::Value& data, json::Value::AllocatorType& alloc, const std::string& path)
    {
        const auto& layerCont = std::get<LayerID>(m_loaderSavers);
        const auto& nameMap = std::get<std::tuple_size<decltype(m_loaderSavers)>::value - 1>(m_loaderSavers);

        for (auto& i : scene.m_layers)
        {
            // Type name iterator
            auto itrRedir = nameMap.find(std::type_index(typeid(*i)));
            // Function iterator
            auto itr = layerCont.end();

            if (itrRedir == nameMap.end() || (itr = layerCont.find(itrRedir->second)) == layerCont.end())
            {
                JOP_DEBUG_ERROR("Couldn't save layer, type (name) not registered: " << path);
                return false;
            }

            data.PushBack(json::kObjectType, alloc);
            auto& obj = data[data.Size() - 1];

            obj.AddMember(json::StringRef(ns_typeField), json::StringRef(itrRedir->second.c_str()), alloc);

            if (!std::get<SaveID>(itr->second)(*i, obj.AddMember(json::StringRef(ns_dataField), json::kObjectType, alloc)[ns_dataField], alloc))
            {
                JOP_DEBUG_ERROR("Couldn't save layer, registered save function reported failure: " << path);
                return false;
            }

            const char* const boundLayersField = "boundlayers";

            if (!i->m_boundLayers.empty())
            {
                auto& boundArray = data.AddMember(json::StringRef(boundLayersField), json::kArrayType, alloc)[boundLayersField];

                for (auto& j : i->m_boundLayers)
                {
                    if (!j.expired())
                        boundArray.PushBack(json::StringRef(j.lock()->getID().c_str()), alloc);
                }
            }
        }

        return true;
    }

    //////////////////////////////////////////////

    bool StateLoader::saveObjects(const Scene& scene, json::Value& data, json::Value::AllocatorType& alloc, const std::string& path)
    {
        for (auto& i : scene.m_objects)
        {
            data.PushBack(json::kObjectType, alloc);
            auto& obj = data[data.Size() - 1u];

            obj.AddMember(json::StringRef("id"), json::StringRef(i->getID().c_str()), alloc);

            if (!saveObject(*i, obj, alloc, path))
            {
                JOP_DEBUG_ERROR("Failed to save object with id \"" << i->getID() << "\": " << path);
                return false;
            }
        }

        return true;
    }

    //////////////////////////////////////////////

    bool StateLoader::saveObject(const Object& obj, json::Value& data, json::Value::AllocatorType& alloc, const std::string& path)
    {
        const char* const activeField = "active";
        const char* const componentsField = "components";
        const char* const childrenField = "children";
        const char* const transformField = "transform";

        const auto& compCont = std::get<CompID>(m_loaderSavers);
        const auto& nameMap = std::get<std::tuple_size<decltype(m_loaderSavers)>::value - 1>(m_loaderSavers);

        // Active
        data.AddMember(json::StringRef(activeField), obj.isActive(), alloc);

        // Transform
        data.AddMember(json::StringRef(transformField), json::kArrayType, alloc)[transformField]
            .PushBack(obj.getPosition().x, alloc)
            .PushBack(obj.getPosition().y, alloc)
            .PushBack(obj.getPosition().z, alloc)
            .PushBack(obj.getScale().x, alloc)
            .PushBack(obj.getScale().y, alloc)
            .PushBack(obj.getScale().z, alloc)
            .PushBack(obj.getRotation().w, alloc)
            .PushBack(obj.getRotation().x, alloc)
            .PushBack(obj.getRotation().y, alloc)
            .PushBack(obj.getRotation().z, alloc);
           
        // Components
        if (!obj.m_components.empty())
        {
            auto& comps = data.AddMember(json::StringRef(componentsField), json::kArrayType, alloc)[componentsField];

            for (auto& i : obj.m_components)
            {
                // Type name iterator
                auto itrRedir = nameMap.find(std::type_index(typeid(*i)));
                // Function iterator
                auto itr = compCont.end();

                if (itrRedir == nameMap.end() || (itr = compCont.find(itrRedir->second)) == compCont.end())
                {
                    JOP_DEBUG_ERROR("Couldn't save component with id \"" << i->getID() << "\", type (name) not registered: " << path);
                    return false;
                }

                comps.PushBack(json::kObjectType, alloc);
                auto& compObj = comps[comps.Size() - 1u];

                compObj.AddMember(json::StringRef(ns_typeField), json::StringRef(itrRedir->second.c_str()), alloc);

                if (!std::get<SaveID>(itr->second)(*i, compObj.AddMember(json::StringRef(ns_dataField), json::kObjectType, alloc)[ns_dataField], alloc))
                {
                    JOP_DEBUG_ERROR("Couldn't save component with id \"" << i->getID() << "\", registered save function reported failure: " << path);
                    return false;
                }
            }
        }

        if (!obj.m_children.empty())
        {
            auto& objs = data.AddMember(json::StringRef(childrenField), json::kArrayType, alloc)[childrenField];

            for (auto& i : obj.m_children)
            {
                objs.PushBack(json::kObjectType, alloc);
                auto& curr = objs[objs.Size() - 1];

                curr.AddMember(json::StringRef("id"), json::StringRef(i->getID().c_str()), alloc);

                if (!saveObject(*i, curr, alloc, path))
                {
                    JOP_DEBUG_ERROR("Failed to save object with id \"" << i->getID() << "\": " << path);
                    return false;
                }
            }
        }

        return true;
    }
}

//{
//    "version" : "0.1",
//    "subsystems" : [
//        subsystem datas here
//    ],
//    "sharedScene" : {
//        shared scene data here, same as "scene"
//    },
//    "scene" : {
//        "type" : "jop::Scene",
//        "data" : {
//            this object is passed to the registered scene load function
//        },
//        "layers" : [
//            Layer datas here, first is the default layer
//        ],
//        "objects" : [
//            {
//                "id" : "someObject",
//                transform data
//                active
//                children array
//                component array
//            }
//        ]
//    }
//}