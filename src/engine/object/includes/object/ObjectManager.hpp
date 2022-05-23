#pragma once
#include "object/Object.hpp"
#include "renderable/Renderable.hpp"
#include <vector>

class ObjectManager{
    private:
    std::vector<Object*> aliveObjects;
    std::vector<Object*> tickingObjects;
    std::vector<Renderable*> renderables;
    public:
    ObjectManager();
    ~ObjectManager();
    void tickObjects();
    void renderRenderables(glm::mat4 projectionViewMatrix);
    void registerObject(Object* obj);
    /**
     * @brief Deletes the memory used by the object and removes it from
     * the active objects.
     * 
     * @param obj The object to delete
     */
    void deleteObject(Object* obj);

};