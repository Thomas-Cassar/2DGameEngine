module;
#include <exception>
#include <limits>
#include <stdint.h>
#include <string>
export module Core:IdGenerator;

namespace Core
{
export using Id = size_t;

export class IdGenerator {
public:
    IdGenerator() : nextId(0) {}

    Id getNewId()
    {
        if (nextId == std::numeric_limits<Id>::max())
        {
            throw std::exception("Max ID reached!");
        }
        return nextId++;
    }

private:
    Id nextId;
};
} // namespace Core