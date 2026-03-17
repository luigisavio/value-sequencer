#ifndef I_TIME_DURATION_HPP
#define I_TIME_DURATION_HPP

#include <chrono>

namespace timer
{
    /**
     * @brief Time type stored in milliseconds.
     *
     * Use `unsigned long` for better performance on 32-bit CPUs.
     * Using unsigned doubles the maximum range (~50 days).
     * On 64-bit CPUs this will typically be 64-bit, allowing longer intervals efficiently.
     */
    using time_ms = std::chrono::duration<unsigned long, std::milli>;

    /**
     * @brief Interface for object that measure a predefined time duration
     *
     * Provides standard timer operations including preset time, elapsed
     * time, time duration start and check if tim is elapsed.
     */
    class ITimeDuration
    {
        public:
        /**
         * @brief Virtual destructor.
         *
         * Ensures derived classes implement proper cleanup.
         */
        virtual ~ITimeDuration() = default;

        /**
         * @brief Sets the duration of the measured time.
         *
         * @param timeDuration Time that must elapse for the timer to activate.
         */
        virtual void setDuration(time_ms duration) = 0;
        
        /**
         * @brief Gets the currently configured time duration.
         *
         * @return The time duration in milliseconds.
         */
        virtual time_ms getDuration() const = 0;

        /**
         * @brief Checks if the time duration has elapsed.
         *
         * @return True if the time duration has elapsed, false otherwise.
         */
        virtual bool isElapsed() const = 0;

        /**
         * @brief Gets the elapsed time since the time duration started.
         *
         * @return Elapsed time in milliseconds.
         */
        virtual time_ms getElapsedTime() const = 0;

        /**
         * @brief Starts the time duration.
         */
        virtual void start() = 0;

        /**
         * @brief Resets the time duration.
         *
         * It can then be used again by calling the start member function.
         */
        virtual void reset() const = 0;
    };
}

#endif // I_TIME_DURATION_HPP
