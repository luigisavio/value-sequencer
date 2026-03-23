#ifndef TIME_VALUE_HPP
#define TIME_VALUE_HPP

#include "ITimeDuration.hpp"
#include "Sequence.hpp"

namespace valueSequencer
{
	/**
	 * @brief Generates a value that changes over time based on configured durations.
	 *
	 * This class is a template that derives from Sequence, where the value
	 * changes according to the time lengths specified in each step.
	 *
	 * @tparam T Type of the value to be modified over time.
	 */
	template <typename T>
	class TimeValue : public Sequence<T, timer::time_ms>
	{
	public:
		/**
		 * @brief Inherit constructors from the base Sequence class.
		 *
		 * No additional initialization is required.
		 */
		using Sequence<T, timer::time_ms>::Sequence;

	protected:
		/// Timer used to track step durations.
		timer::ITimeDuration m_timeDuration{};

		/**
		 * @brief Advances the sequence to the next step.
		 *
		 * @return True if the step was successfully advanced, false otherwise.
		 */
		bool switchToNextStep();

		/**
		 * @brief Checks if the current step has finished based on elapsed time.
		 *
		 * @return True if the current step is complete, false otherwise.
		 */
		bool isCurrentStepFinished();

		/**
		 * @brief Performs a single advance of the sequence.
		 *
		 * Updates internal state based on elapsed time and sequence configuration.
		 */
		void stepAdvance();
	};

	template <typename T>
	bool TimeValue<T>::switchToNextStep()
	{
		// Reset timer
		this->m_timeDuration.reset();

        this->m_currentStepIndex += 1;

        if (this->m_currentStepIndex == this->m_numberOfSteps)
        {
            // Exit, sequence is finished
            this->m_currentStepIndex = this->noStepIdx;
			return false;
        }else
        {
			// Preset and start the timer
            this->m_timeDuration.setDuration(this->m_sequenceDef[this->m_currentStepIndex]);
            this->m_timeDuration.start();
		    return true;
        }
	}

	template <typename T>
	bool TimeValue<T>::isCurrentStepFinished()
	{
		// Current step is finished if timer has finished counting
		return this->m_timeDuration.isElapsed();
	}

	template <typename T>
	void TimeValue<T>::stepAdvance()
	{
		; // Do nothing, time duration does not need to be updated
	}

} // namespace valueSequencer

#endif // TIME_VALUE_HPP
