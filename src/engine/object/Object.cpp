#include "object/Object.hpp"

Object::Object(bool doesObjectTick)
:doesObjectTick(doesObjectTick)
{
}
bool Object::getDoesObjectTick()
{
    return doesObjectTick;
}