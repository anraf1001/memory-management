#pragma once

#include <atomic>
#include <functional>

template <typename T>
class SharedControlBlock {
public:
    SharedControlBlock(std::function<void(T*)> defDeleter = [](T* ptrToDelete) { delete ptrToDelete; })
        : defaultDeleter(defDeleter) {}

    void incrementSharedRefs() { ++sharedRefs_; }
    void decrementSharedRefs() { --sharedRefs_; }
    size_t getSharedRefs() const { return sharedRefs_.load(); }

    void incrementWeakRefs() { ++weakRefs_; }
    void decrementWeakRefs() { --weakRefs_; }
    size_t getWeakRefs() const { return weakRefs_.load(); }

    std::function<void(T*)> defaultDeleter;

private:
    std::atomic<size_t> sharedRefs_ = 1;
    std::atomic<size_t> weakRefs_ = 0;
};
