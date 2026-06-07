#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> q;
    mutable std::mutex mtx;
    std::condition_variable cv;

public:
    ThreadSafeQueue() = default;

    void push(T item) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(std::move(item));
        }

        cv.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [this]() {
            return !q.empty();
        });

        T item = std::move(q.front());
        q.pop();

        return item;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return q.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return q.size();
    }
};