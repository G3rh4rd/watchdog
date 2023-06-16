#include "watchdog_timer.h"

#include <chrono>

WatchdogTimer::WatchdogTimer(int64_t reset_time_ms, on_watchdog_fired_callback_t on_watchdog_fired_callback) {
    m_reset_duration_ms = reset_time_ms;
    m_is_wd_running = false;
    m_on_watchdog_fired_callback = on_watchdog_fired_callback;
}

WatchdogTimer::~WatchdogTimer() {
    m_is_wd_running = false;
}

void WatchdogTimer::start() {
    watchdogThread = std::thread(&WatchdogTimer::watchdogWorker, this);
    watchdogThread.detach();
}

void WatchdogTimer::stop() {
    m_is_wd_running = false;
}

void WatchdogTimer::reset() {
    m_mx.lock();
    m_last_reset_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    m_mx.unlock();
}

void WatchdogTimer::watchdogWorker() {
    m_is_wd_running = true;

    reset();

    while(m_is_wd_running) {
        int64_t current_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        m_mx.lock();
        int64_t last_reset_duration_ms = current_time_ms - m_last_reset_time_ms;
        m_mx.unlock();
        
        if(last_reset_duration_ms > m_reset_duration_ms && m_on_watchdog_fired_callback != NULL) {
            // watchdog hasn't been resetted within configured duration. so fire watchdog!
            m_on_watchdog_fired_callback();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(m_reset_duration_ms));
    }
}