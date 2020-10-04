#pragma once

#include <iostream>

#include "sharedControlBlock.hpp"
#include "shared_ptr.hpp"

namespace cs {

template <typename T>
class weak_ptr {
public:
    constexpr weak_ptr() noexcept = default;
    weak_ptr(const weak_ptr& r) noexcept;
    weak_ptr(const cs::shared_ptr<T>& r) noexcept;
    weak_ptr(weak_ptr&& r) noexcept;

    weak_ptr& operator=(const weak_ptr& r) noexcept;
    weak_ptr& operator=(const shared_ptr<T>& r) noexcept;
    weak_ptr& operator=(weak_ptr&& r) noexcept;
    ~weak_ptr();
    size_t use_count() const noexcept;
    bool expired() const noexcept;
    cs::shared_ptr<T> lock() const noexcept;
    void reset() noexcept;

private:
    T* ptr_ = nullptr;
    SharedControlBlock<T>* controlBlock_ = nullptr;
};

template <typename T>
weak_ptr<T>::weak_ptr(const weak_ptr<T>& r) noexcept
    : ptr_{r.ptr_}, controlBlock_{r.controlBlock_} {
    if (controlBlock_) {
        controlBlock_->incrementWeakRefs();
    }
}

template <typename T>
weak_ptr<T>::weak_ptr(const cs::shared_ptr<T>& r) noexcept
    : ptr_{r.ptr_}, controlBlock_{r.controlBlock_} {
    if (controlBlock_) {
        controlBlock_->incrementWeakRefs();
    }
}

template <typename T>
weak_ptr<T>::weak_ptr(weak_ptr<T>&& r) noexcept
    : ptr_{std::move(r.ptr_)}, controlBlock_{std::move(r.controlBlock_)} {
    r.ptr_ = nullptr;
    r.controlBlock_ = nullptr;
}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(const weak_ptr<T>& r) noexcept {
    if (this != &r) {
        ptr_ = r.ptr_;
        controlBlock_ = r.controlBlock_;
        if (controlBlock_) {
            controlBlock_->incrementWeakRefs();
        }
    }
    return *this;
}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(const shared_ptr<T>& r) noexcept {
    ptr_ = r.ptr_;
    controlBlock_ = r.controlBlock_;
    if (controlBlock_) {
        controlBlock_->incrementWeakRefs();
    }
    return *this;
}

template <typename T>
weak_ptr<T>& weak_ptr<T>::operator=(weak_ptr<T>&& r) noexcept {
    if (this != &r) {
        ptr_ = r.ptr_;
        controlBlock_ = r.controlBlock_;
        r.ptr_ = nullptr;
        r.controlBlock_ = nullptr;
    }
    return *this;
}

template <typename T>
weak_ptr<T>::~weak_ptr() {
    if (controlBlock_) {
        if (controlBlock_->getSharedRefs() == 1 && controlBlock_->getWeakRefs() == 1) {
            controlBlock_->defaultDeleter(ptr_);
            delete controlBlock_;
        } else {
            controlBlock_->decrementWeakRefs();
        }
    }
}

template <typename T>
size_t weak_ptr<T>::use_count() const noexcept {
    if (controlBlock_) {
        return controlBlock_->getSharedRefs();
    }
    return 0;
}

template <typename T>
bool weak_ptr<T>::expired() const noexcept {
    return use_count() == 0;
}

template <typename T>
cs::shared_ptr<T> weak_ptr<T>::lock() const noexcept {
    //return expired() ? cs::shared_ptr<T>() : cs::shared_ptr<T>(*this);
    //need copy constructor which takes weak_ptr
    return expired() ? cs::shared_ptr<T>() : cs::shared_ptr<T>(ptr_);
}

template <typename T>
void weak_ptr<T>::reset() noexcept {
    if (controlBlock_) {
        if (controlBlock_->getWeakRefs() == 1 && controlBlock_->getSharedRefs() == 1) {
            controlBlock_->defaultDeleter(ptr_);
            delete controlBlock_;
        } else {
            controlBlock_->decrementWeakRefs();
        }
    }
    ptr_ = nullptr;
    controlBlock_ = nullptr;
}

}  // namespace cs
