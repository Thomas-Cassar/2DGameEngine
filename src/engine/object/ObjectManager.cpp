#include "object/ObjectManager.hpp"

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{
    for(auto it=aliveObjects.begin();it!=aliveObjects.end();it++)
    {
        delete (*it);
    }
}

void ObjectManager::tickObjects()
{
    for(auto it=tickingObjects.begin();it!=tickingObjects.end();it++)
    {
        (*it)->objectTick();
    }
}

void ObjectManager::renderRenderables(glm::mat4 projectionViewMatrix)
{
    for(auto it=renderables.begin();it!=renderables.end();it++)
    {
        if((*it)->getVisible())
            (*it)->render(projectionViewMatrix);
    }
}

void ObjectManager::registerObject(Object* obj)
{
    aliveObjects.push_back(obj);
    if(obj->getDoesObjectTick())
        tickingObjects.push_back(obj);
    Renderable* renderableObject = dynamic_cast<Renderable*>(obj);
    if(renderableObject!=nullptr)
    {
        renderables.push_back(renderableObject);
    }
}
void ObjectManager::deleteObject(Object* obj)
{
    for(auto it=aliveObjects.begin();it!=aliveObjects.end();it++)
    {
        if((*it)==obj)
        {
            aliveObjects.erase(it);
            break;
        }
    }
    for(auto it=tickingObjects.begin();it!=tickingObjects.end();it++)
    {
        if((*it)==obj)
        {
            tickingObjects.erase(it);
            break;
        }
    }
    for(auto it=renderables.begin();it!=renderables.end();it++)
    {
        if((*it)==obj)
        {
            renderables.erase(it);
            break;
        }
    }
    delete obj;
}