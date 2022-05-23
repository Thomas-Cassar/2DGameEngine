#include "renderable/Renderable.hpp"

Renderable::Renderable(bool doesTick)
:Actor(doesTick)
{
    
}

void Renderable::setVisible(bool visible)
{
    this->visible=visible;
}

bool Renderable::getVisible()
{
    return visible;
}
