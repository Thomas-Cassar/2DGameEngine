module;
#include <exception>
#include <limits>
module Core:IdGenerator;

namespace Core
{

IdGenerator::IdGenerator() : nextId(0) {}

Id IdGenerator::getNewId()
{
    if (nextId == std::numeric_limits<Id>::max())
    {
        throw std::exception("Max ID reached!");
    }
    return nextId++;
}
} // namespace Core
