export module Core:IdGenerator;
import <exception>;
import <limits>;
import <stdint.h>;
import <string>;

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