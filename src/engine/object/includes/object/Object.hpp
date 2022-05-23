#pragma once
class Object{
    private:
    bool doesObjectTick=false;
    public:
    Object(bool doesObjectTick);
    virtual ~Object()= default;
    virtual void objectTick()=0;
    bool getDoesObjectTick();
};