#pragma once
#include <stdint.h>

using Id = size_t;

class IdGenerator {
public:
    IdGenerator();
    Id getNewId();

private:
    Id nextId;
};