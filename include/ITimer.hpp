#ifndef I_TIMER_HPP
#define I_TIMER_HPP

namespace timer
{
    /**
     * @brief Time type stored in milliseconds.
     *
     * Use `unsigned long` for better performance on 32-bit CPUs.
     * Using unsigned doubles the maximum range (~50 days).
     * On 64-bit CPUs this will typically be 64-bit, allowing longer intervals efficiently.
     */
    using time_ms = unsigned long;

    /**
     * @brief Interface for a generic timer.
     *
     * Provides standard timer operations including preset time, elapsed
     * time, input/output handling, and cycle-based advancement.
     */
    class ITimer
    {
    public:
        /**
         * @brief Virtual destructor.
         *
         * Ensures derived classes implement proper cleanup.
         */
        virtual ~ITimer() = default;

        /**
         * @brief Sets the preset time (PT) of the timer.
         *
         * @param presetTime Time that must elapse for the timer to trigger.
         */
        virtual void setPresetTime(time_ms presetTime) = 0;

        /**
         * @brief Gets the currently configured preset time.
         *
         * @return The preset time in milliseconds.
         */
        virtual time_ms getPresetTime() const = 0;

        /**
         * @brief Advances the timer by one cycle.
         *
         * Should be called at each iteration of the main loop or tick.
         */
        virtual void tick() = 0;

        /**
         * @brief Returns the timer output (Q).
         *
         * @return True if the timer output is active, false otherwise.
         */
        virtual bool out() const = 0;

        /**
         * @brief Gets the elapsed time (ET) since the timer started.
         *
         * @return Elapsed time in milliseconds.
         */
        virtual time_ms getElapsedTime() const = 0;

        /**
         * @brief Sets the timer input (IN).
         *
         * @param in True to activate the timer input, false to deactivate.
         */
        virtual void setIn(bool in) = 0;

        /**
         * @brief Gets the current timer input state.
         *
         * @return True if the timer input is active, false otherwise.
         */
        virtual bool getIn() const = 0;
    };
}

#endif // I_TIMER_HPP
