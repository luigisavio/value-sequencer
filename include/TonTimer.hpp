#ifndef TON_TIMER_HPP
#define TON_TIMER_HPP

#include "ITimeDuration.hpp"
#include <chrono>

namespace timer
{
    /**
     * @brief Implementation for a lightweight TON timer.
     *
     * Lightweight, updates are done with tick() function. All other functions
     * are lightweight and only return the state of the timer. But tick() 
     * function must be called to update internal variables.
     * TON means that the timer output activates after configured preset time
     * is elapsed with timer input continuously active.
     */
    class LightTonTimer : public ITimeDuration
    {
    private:
        // Type aliases to make accessing nested type easier
        using Clock = std::chrono::steady_clock;
        using Millisecond = std::chrono::milliseconds;

        std::chrono::time_point<Clock> m_beg { Clock::now() };
        Millisecond m_presetTime {};

    public:
        void setPresetTime(time_ms presetTime) { m_presetTime = static_cast<Millisecond>(presetTime); }

        time_ms getPresetTime() { return static_cast<time_ms>(m_presetTime); }

        time_ms getElapsedTime() const
        {
            return std::chrono::duration_cast<Millisecond>(Clock::now() - m_beg).count();
        }

    };

} // Namespace timer

#endif
