module;
#include <concepts>
export module Core;

export import :IdGenerator;

export template <class T, class U>
concept Derived = std::is_base_of<U, T>::value;