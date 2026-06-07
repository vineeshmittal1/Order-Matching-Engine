#pragma once

#include <vector>
#include <stack>
#include <memory>
#include <mutex>

template<typename T>
class MemoryPool {
private:
    std::vector<std::unique_ptr<T>> storage;
    std::stack<T*> freeList;

    mutable std::mutex mtx;

public:
    explicit MemoryPool(size_t capacity = 100000) {

        storage.reserve(capacity);

        for (size_t i = 0; i < capacity; i++) {

            storage.push_back(
                std::make_unique<T>()
            );

            freeList.push(storage.back().get());
        }
    }

    template<typename... Args>
    T* allocate(Args&&... args) {

        std::lock_guard<std::mutex> lock(mtx);

        if (freeList.empty()) {
            throw std::bad_alloc();
        }

        T* obj = freeList.top();
        freeList.pop();

        *obj = T(std::forward<Args>(args)...);

        return obj;
    }

    void deallocate(T* obj) {

        std::lock_guard<std::mutex> lock(mtx);

        freeList.push(obj);
    }

    size_t available() const {

        std::lock_guard<std::mutex> lock(mtx);

        return freeList.size();
    }
};