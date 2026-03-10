#ifndef I_TIMER_HPP
#define I_TIMER_HPP

/* Interface to timer, abstract class */

namespace timer
{
    // Always store time in milliseconds. Use long for better performance (32 bits guaranteed, long long has 64 and may 
    // be not optimal for 32 bits CPUs). Use unsigned to double the range to about 50 days. On 64 bits CPUs let this be
    // 64 bits. It may be more efficient and it can then count longer time intervals.
    using time_ms = unsigned long;

    class ITimer
    {
    public:
        // Virtual destructor to force child classes to implement a destructor
        virtual ~ITimer() = default;

        // Preset time is the time that is associated to the timer (PT)
        virtual void setPresetTime(time_ms presetTime) = 0; // Preset time is the time that must elapse
        virtual time_ms getPresetTime() const = 0;

        // Advance timer by one cycle
        virtual void tick() = 0;

        // Get the timer output (Q)
        virtual bool out() const = 0; // Returns true if timer output is active

        // Get how much time is currently elapsed (ET)
        virtual time_ms getElapsedTime() const = 0;

        // Manage the timer input (IN)
        virtual void setIn(bool in) = 0;
        virtual bool getIn() const = 0;
    };
}

#endif // I_TIMER_HPP
