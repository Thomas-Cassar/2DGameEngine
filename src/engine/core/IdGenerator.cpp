#include "core/IdGenerator.hpp"
#include "core/core.hpp"
#include <iostream>
#include <limits>

IdGenerator::IdGenerator() : nextId(0) {}

Id IdGenerator::getNewId()
{
    if (nextId == std::numeric_limits<Id>::max())
    {
        std::cerr << "Error max Id reached" << std::endl;
        engineAssert(false);
    }
    return nextId++;
}