#pragma once

#include <iostream>
#include <atomic>
#include <thread>
#include <unistd.h>
#include <sys/syscall.h>

#ifdef __linux__
// Required for Linux CPU affinity
#include <sched.h>  // Required for Linux CPU affinity
#elif __APPLE__
// Required for macOS thread affinity
#include <mach/mach.h>
#include <mach/mach_init.h>
#include <mach/thread_policy.h>
#endif

namespace Common {
    /// Set affinity for current thread to be pinned to the provided core_id.
    inline auto setThreadCore(int core_id) noexcept {
#ifdef __linux__
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(core_id, &cpuset);
        return (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) == 0);

#elif __APPLE__
        thread_affinity_policy_data_t policy = { static_cast<integer_t>(core_id) };
        return (thread_policy_set(mach_thread_self(),
                                  THREAD_AFFINITY_POLICY,
                                  (thread_policy_t)&policy,
                                  1) == KERN_SUCCESS);
#else
        std::cerr << "Thread affinity not supported on this OS" << std::endl;
        return false;
#endif
    }

    /// Creates a thread instance, sets affinity on it, assigns it a name and
    /// passes the function to be run on that thread as well as the arguments to the function.
    template<typename T, typename... A>
    inline auto createAndStartThread(int core_id, const std::string &name, T &&func, A &&... args) noexcept {
        auto t = new std::thread([=]() {
            if (core_id >= 0 && !setThreadCore(core_id)) {
                std::cerr << "Failed to set core affinity for " << name << " thread: " << pthread_self() << " to core " << core_id << std::endl;
                exit(EXIT_FAILURE);
            }
            std::cerr << "Set core affinity for " << name << " thread: " << pthread_self() << " to core " << core_id << std::endl;

            std::forward<T>(func)(std::forward<A>(args)...);
        });

        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(1s);

        return t;
    }
}
