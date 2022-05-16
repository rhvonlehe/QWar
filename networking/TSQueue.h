#ifndef TSQUEUE_H
#define TSQUEUE_H

#include <mutex>
#include <deque>

// thread-safe queue
template<typename T>
class TSQueue
{
public:
    TSQueue() = default;
    TSQueue(const TSQueue<T>&) = delete;
    virtual ~TSQueue() { clear(); }

public:
    // Returns and maintains item at front of Queue
    const T& front()
    {
        std::scoped_lock lock(mtxQueue);
        return deqQueue.front();
    }

    // Returns and maintains item at back of Queue
    const T& back()
    {
        std::scoped_lock lock(mtxQueue);
        return deqQueue.back();
    }

    // Removes and returns item from front of Queue
    T pop_front()
    {
        std::scoped_lock lock(mtxQueue);
        auto t = std::move(deqQueue.front());
        deqQueue.pop_front();
        return t;
    }

    // Removes and returns item from back of Queue
    T pop_back()
    {
        std::scoped_lock lock(mtxQueue);
        auto t = std::move(deqQueue.back());
        deqQueue.pop_back();
        return t;
    }

    // Adds an item to back of Queue
    void push_back(const T& item)
    {
        std::scoped_lock lock(mtxQueue);
        deqQueue.emplace_back(std::move(item));

        std::unique_lock<std::mutex> ul(mtxBlocking);
        cvBlocking.notify_one();
    }

    // Adds an item to front of Queue
    void push_front(const T& item)
    {
        std::scoped_lock lock(mtxQueue);
        deqQueue.emplace_front(std::move(item));

        std::unique_lock<std::mutex> ul(mtxBlocking);
        cvBlocking.notify_one();
    }

    // Returns true if Queue has no items
    bool empty()
    {
        std::scoped_lock lock(mtxQueue);
        return deqQueue.empty();
    }

    // Returns number of items in Queue
    size_t count()
    {
        std::scoped_lock lock(mtxQueue);
        return deqQueue.size();
    }

    // Clears Queue
    void clear()
    {
        std::scoped_lock lock(mtxQueue);
        deqQueue.clear();
    }

    void wait()
    {
        while (empty())
        {
            std::unique_lock<std::mutex> ul(mtxBlocking);
            cvBlocking.wait(ul);
        }
    }

protected:
    std::mutex              mtxQueue;
    std::deque<T>           deqQueue;
    std::condition_variable cvBlocking;
    std::mutex              mtxBlocking;
};

#endif // TSQUEUE_H
