#pragma once
#include "worker.hpp"
#include <deque>
#include <map>
#include <vector>
#include <unordered_set>
#include <mutex>

class SpinLock final
{
public:
    SpinLock() noexcept = default;
    SpinLock(const SpinLock&) = delete;
    SpinLock(SpinLock&&) = delete;
    SpinLock& operator=(const SpinLock&) = delete;
    SpinLock& operator=(SpinLock&&) = delete;
    ~SpinLock() = default;

    void lock() noexcept
    {
        using namespace std::chrono_literals;
        while (!try_lock())
            std::this_thread::sleep_for(500us);
    }

    bool try_lock() noexcept
    {
        bool result = !m_lock.test_and_set(std::memory_order_acq_rel);
        for (size_t i = 0; !result && i < 1024; ++i) {
            result = !m_lock.test_and_set(std::memory_order_acq_rel);
        }

        return result;
    }

    void unlock() noexcept { m_lock.clear(std::memory_order_release); }

private:
    std::atomic_flag m_lock = ATOMIC_FLAG_INIT;
};


class SpinLockHolder final
{
public:
    explicit SpinLockHolder(SpinLock* lock) noexcept : m_lock(lock)
    {
        m_lock = lock;
        if (m_lock)
            m_lock->lock();
    }
    explicit SpinLockHolder(SpinLock* lock, const std::atomic_bool& abandonLock) noexcept : m_lock(lock)
    {
        using namespace std::chrono_literals;
        m_lock = lock;
        if (m_lock) {
            while (!m_lock->try_lock()) {
                if (abandonLock.load(std::memory_order_relaxed)) {
                    m_lock = nullptr;
                    return;
                }
                std::this_thread::sleep_for(500us);
            }
        }
    }
	
    SpinLockHolder(const SpinLockHolder&) = delete;
    SpinLockHolder(SpinLockHolder&&) = delete;
    SpinLockHolder& operator=(const SpinLockHolder&) = delete;
    SpinLockHolder& operator=(SpinLockHolder&&) = delete;
    ~SpinLockHolder() { unlock(); }

    bool isLocked() const noexcept { return m_lock; }

    void unlock() noexcept
    {
        if (m_lock)
            m_lock->unlock();
        m_lock = nullptr;
    }

private:
    SpinLock* m_lock = nullptr;
};





class task_dispatcher
{
public:
	explicit task_dispatcher(size_t number_of_workers);
	void insert_task(task_info&& task);
	void task_finished(const task_info& task, std::size_t worker_id, bool next);
	void schedule();
	~task_dispatcher();
	
private:
	std::vector<std::unique_ptr<worker>> workers_{};
	std::deque<task_info> tasks_queue_{};
	std::unordered_set<std::size_t> available_workers_{};
	SpinLock main_lock_{};
    //std::mutex main_lock_{};
    std::mutex wait_lock_{};
    std::condition_variable wait_{};


    bool queue_empty_{ false };
	std::atomic_bool stop_has_started_{ false };
};
