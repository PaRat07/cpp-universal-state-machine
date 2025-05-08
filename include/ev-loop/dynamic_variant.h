#pragma once

#include <utility>

template <typename... Ts>
class DynamicVariant {
 public:
    template<typename T>
    DynamicVariant(std::in_place_t<T>, auto&&... args) {

    }
 private:
    void *mem;
};
