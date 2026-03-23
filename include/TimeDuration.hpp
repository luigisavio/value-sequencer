#ifndef TON_TIMER_HPP
#define TON_TIMER_HPP

#include "ITimeDuration.hpp"
#include <chrono>

namespace timer
{
    /**
     * @brief Implementation for a time duration.
     *
     * Measures if a configured amount of time is elapsed from a start condition.
     */
    class TimeDuration : public ITimeDuration
    {
    private:
        // Type aliases to make accessing nested type easier
        using Clock = std::chrono::steady_clock;

        std::chrono::time_point<Clock> m_beg { Clock::now() };
        time_ms m_duration{};
        bool m_isRunning{};

    public:
        void setDuration(time_ms duration) { m_duration = static_cast<time_ms>(duration); }

        time_ms getDuration() const { return static_cast<time_ms>(m_duration); }

        time_ms getElapsedTime() const
        {
            return std::chrono::duration_cast<time_ms>(Clock::now() - m_beg);
        }

        bool isElapsed() const 
        { 
            return m_isRunning ? (Clock::now() - m_beg > m_duration) : false;
        }

        void start()
        {
            m_beg = Clock::now();
            m_isRunning = true; 
        }
        
        void reset()
        {
            m_isRunning = false; 
        }
    };

} // Namespace timer

#endif
