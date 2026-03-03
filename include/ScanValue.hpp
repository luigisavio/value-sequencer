#ifndef SCAN_VALUE_HPP
#define SCAN_VALUE_HPP

#include <vector>
#include <algorithm>

namespace Sequence
{
	// Generates a value that changes based on how many times a particular function is called.
	template <typename T> // T represents the type of the value to be changed.
	class ScanValue
	{
	public:
		static constexpr int noStepIdx{-1};
		// Configuration of a single time step
		struct StepPar
		{
			long numberOfScans{0}; // For how many scans the value will be assigned
			T value{};
		};

		// Generic constructor
		ScanValue(std::vector<StepPar> sequenceDef, const T &idleValue = {}) noexcept
		{
			setSequence(sequenceDef, idleValue);
		}

		// Default constructor
		ScanValue() noexcept {}

		// Configure the sequence. If object was already doing something it is reset
		bool setSequence(const std::vector<StepPar> &sequenceDef, const T &idleValue = {}) noexcept; // Safer
		bool setSequence(std::vector<StepPar> &&sequenceDef, const T &idleValue = {}) noexcept;		 // More performant, avoids a copy of the array, use std::move to pass the array

		// Simply returns the value
		const T &getValue() const noexcept;

		// Updates time and returns the integral value after the update
		const T &executeSequence() noexcept;

		// Completely resets the object, equivalent to creating a new one with constructor method
		ScanValue &reset() noexcept;

		// Triggers the start of the sequences
		ScanValue &start() noexcept
		{
			m_startSequenceRequest = true;
			return *this;
		}

		// Checks if sequence is running
		bool isSequenceRunning() const noexcept { return m_currentStepIndex != noStepIdx; }

	private:
		std::vector<StepPar> m_sequenceDef{}; // Values of the number and times during the sequence
		T m_idleValue{};
		int m_currentStepIndex{noStepIdx}; // Current index of the step
		long m_numOfScansAlreadyDone{0};	// Number of scans that have already been executed
		int m_numberOfSteps{0};				// Total number of steps to be executed
		bool m_startSequenceRequest{false};

		// Switches to the step. Returns true if it was possible and false if the sequence is finished
		bool switchToNextStep() noexcept;
	};

	// Template functions must be defined in header file since every TU needs to see the implementation to compile the required instance
	template <typename T>
	const T &ScanValue<T>::getValue() const noexcept
	{
		return (m_currentStepIndex == noStepIdx) ? m_idleValue
												  : m_sequenceDef[static_cast<std::size_t>(m_currentStepIndex)].value;
	}

	
	template <typename T>
	bool ScanValue<T>::switchToNextStep() noexcept
	{
		// Switch to a step that is next to current one AND has at least one associated scan
		m_numOfScansAlreadyDone = 0;

		auto begin = m_sequenceDef.begin() + static_cast<std::size_t>(m_currentStepIndex + 1); // Start searching from next element
		auto end   = m_sequenceDef.begin() + static_cast<std::size_t>(m_numberOfSteps); // One after the end

		auto it = std::find_if(begin, end, [] (const ScanValue<T>::StepPar &step) {return step.numberOfScans > 0;}); // Look for a step that has at least one scan

		if (it == end)
		{
			m_currentStepIndex = noStepIdx;
			return false;
		}

		m_currentStepIndex = static_cast<int>(std::distance(m_sequenceDef.begin(), it));
		return true;
	}


	template <typename T>
	const T &ScanValue<T>::executeSequence() noexcept
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
				++m_numOfScansAlreadyDone;
				// Check required number of scans have already happened
				if (m_numOfScansAlreadyDone >= m_sequenceDef[static_cast<std::size_t>(m_currentStepIndex)].numberOfScans)
					switchToNextStep();
			}
		}

		return getValue();
	}

	template <typename T>
	bool ScanValue<T>::setSequence(const std::vector<StepPar> &sequenceDef, const T &idleValue) noexcept
	{
		reset();
		m_sequenceDef = std::vector<StepPar>(sequenceDef); // Potentially expensive copy
		m_idleValue = idleValue;
		m_numberOfSteps = m_sequenceDef.size(); // Get number of steps to be executed from size of steps vector
		return (m_numberOfSteps > 0);
	}

	template <typename T>
	bool ScanValue<T>::setSequence(std::vector<StepPar> &&sequenceDef, const T &idleValue) noexcept
	{
		reset();
		m_sequenceDef = std::move(sequenceDef); // Avoid expensive copy
		m_idleValue = idleValue;
		m_numberOfSteps = m_sequenceDef.size(); // Get number of steps to be executed from size of steps vector
		return (m_numberOfSteps > 0);
	}

	template <typename T>
	ScanValue<T> &ScanValue<T>::reset() noexcept
	{
		*this = ScanValue{};
		return *this;
	}

} // namespace Sequence

#endif // SCAN_VALUE_HPP
