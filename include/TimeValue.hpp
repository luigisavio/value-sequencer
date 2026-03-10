#ifndef TIME_VALUE_HPP
#define TIME_VALUE_HPP

#include "ITimer.hpp"
#include <vector>
#include <algorithm>

namespace sequence
{
	// Generates a value that changes in time based on configured time lengths
	template <typename T> // T represents the type of the value to be changed.
	class TimeValue
	{
	public:
		static constexpr int noStepIdx{-1};
		// Configuration of a single time step
		struct StepPar
		{
			timer::time_ms stepTime{0};
			T value{};
		};

		// Generic constructor
		// Passing the vector by value. If called on an lvalue a copy is made. Otherwise move constructor is called.
		// In both cases, the vector inside this function can be cleared by moving it to another vector for better 
		// performances.
		TimeValue(std::vector<StepPar> sequenceDef, const T &idleValue = {}) noexcept
		{
			setSequence(std::move(sequenceDef), idleValue);
		}

		// Default constructor
		TimeValue() noexcept {}

		// Configure the sequence. If object was already doing something it is reset.
		// Pass vector by value: if it is an lvalue it will copied, otherwise moved.
		bool setSequence(std::vector<StepPar> sequenceDef, const T &idleValue = {}) noexcept;

		// Simply returns the value
		const T &getValue() const noexcept;

		// Updates time and returns the value after the update
		const T &executeSequence() noexcept;

		// Completely resets the object, equivalent to creating a new one with constructor method
		TimeValue &reset() noexcept;

		// Triggers the start of the sequences
		TimeValue &start() noexcept
		{
			m_startSequenceRequest = true;
			return *this;
		}

		// Checks if sequence is running
		bool isSequenceRunning() const noexcept { return (m_currentStepIndex != noStepIdx) || m_startSequenceRequest; }

	private:
		std::vector<StepPar> m_sequenceDef{}; // Values of the number and times during the sequence
		T m_idleValue{};
		int m_currentStepIndex{noStepIdx}; // Current index of the step
		int m_numberOfSteps{0};			   // Total number of steps to be executed
		bool m_startSequenceRequest{false};
        timer::ITimer stepTimer{}; // Timer used to count time during step

		// Switches to the step. Returns true if it was possible and false if the sequence is finished
		bool switchToNextStep() noexcept;
	};

	// Template functions must be defined in header file since every TU needs to see the implementation to compile the required instance
	template <typename T>
	const T &TimeValue<T>::getValue() const noexcept
	{
		return (m_currentStepIndex == noStepIdx) ? m_idleValue
												 : m_sequenceDef[static_cast<std::size_t>(m_currentStepIndex)].value;
	}

	template <typename T>
	bool TimeValue<T>::switchToNextStep() noexcept
	{
		// Switch to a step that is next to current one
		stepTimer.setIn(false);
        stepTimer.tick(); // Update timer to reset it

        m_currentStepIndex += 1;

        if (m_currentStepIndex == m_numberOfSteps)
        {
            // Exit, sequence is finished
            m_currentStepIndex = noStepIdx;
			return false;
        }else
        {
            stepTimer.setPresetTime(m_sequenceDef[m_currentStepIndex]);
            stepTimer.setIn(true);
		    return true;
        }
	}

	template <typename T>
	const T &TimeValue<T>::executeSequence() noexcept
	{
		// Check request to start the sequence
		if (m_startSequenceRequest)
		{
			m_startSequenceRequest = false;
			switchToNextStep();
		}
		else
		{
			// Check sequence is running
			if (m_currentStepIndex != noStepIdx)
			{
				stepTimer.tick();
				// Check required number of scans have already happened
				if (stepTimer.out())
				{
					switchToNextStep();
				}
			}
		}
		return getValue();
	}

	template <typename T>
	bool TimeValue<T>::setSequence(std::vector<StepPar> sequenceDef,const T &idleValue) noexcept
	{
		reset();
		m_sequenceDef = std::move(sequenceDef); // Avoid expensive copy by using move semantics on std vector (move assignment operator)
		m_idleValue = idleValue;
		m_numberOfSteps = static_cast<int>(m_sequenceDef.size()); // Get number of steps to be executed from size of steps vector
		return (m_numberOfSteps > 0);
	}

	template <typename T>
	TimeValue<T> &TimeValue<T>::reset() noexcept
	{
		*this = TimeValue{};
		return *this;
	}

} // namespace sequence

#endif // TIME_VALUE_HPP
