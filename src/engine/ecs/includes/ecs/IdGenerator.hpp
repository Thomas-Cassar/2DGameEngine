#pragma once
#include <stdint.h>

using Id = uint64_t;

class IdGenerator {
public:
    IdGenerator();
    Id getNewId();

private:
    Id nextId;
};