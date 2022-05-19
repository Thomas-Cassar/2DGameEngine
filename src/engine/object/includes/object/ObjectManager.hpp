#pragma once
#include "object/Object.hpp"
#include <vector>

class ObjectManager{
    private:
    std::vector<Object*> aliveObjects;
    std::vector<Object*> tickingObjects;
    public:
    ObjectManager();
    ~ObjectManager();
    void tickObjects();
    void registerObject(Object* obj);
    /**
     * @brief Deletes the memory used by the object and removes it from
     * the active objects.
     * 
     * @param obj The object to delete
     */
    void deleteObject(Object* obj);

};