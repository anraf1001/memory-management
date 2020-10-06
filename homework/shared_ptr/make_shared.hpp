#pragma once

#include <utility>

#include "shared_ptr.hpp"

namespace cs {

template <typename _T, typename... Args>
shared_ptr<_T> make_shared(Args&&... args) {
    cs::shared_ptr<_T> ptr;
    auto temporary = new blockObject<_T>(std::forward<Args>(args)...);
    ptr.ptr_ = &temporary->object;
    ptr.controlBlock_ = &temporary->controlBlock;
    return ptr;
}

}  // namespace cs
