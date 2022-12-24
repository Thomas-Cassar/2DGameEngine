module;
#include <stdint.h>
export module Core:IdGenerator;

namespace Core
{
export using Id = size_t;

export class IdGenerator {
public:
    IdGenerator();

    Id getNewId();

private:
    Id nextId;
};
} // namespace Core