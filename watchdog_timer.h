#ifndef WATCHDOG_TIMER_H
#define WATCHDOG_TIMER_H

#include <stdint.h>
#include <mutex>
#include <thread>

typedef void (*on_watchdog_fired_callback_t)();

class WatchdogTimer {
    public:
    WatchdogTimer(int64_t reset_time_ms, on_watchdog_fired_callback_t on_watchdog_fired_callback);
    ~WatchdogTimer();

    void start();
    void stop();
    void reset();

    private:
    bool m_is_wd_running;
    int64_t m_last_reset_time_ms;
    int64_t m_reset_duration_ms;
    std::mutex m_mx;
    std::thread watchdogThread;
    on_watchdog_fired_callback_t m_on_watchdog_fired_callback = NULL;

    void watchdogWorker();
};

#endif /* WATCHDOG_TIMER_H */