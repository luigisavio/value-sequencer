#ifndef SCAN_VALUE_HPP
#define SCAN_VALUE_HPP

#include "Sequence.hpp"
#include <vector>
#include <algorithm>

namespace valueSequencer
{
	// Generates a value that changes based on how many times a particular function is called
	template <typename T> // T represents the type of the value to be changed
	class ScanValue : public Sequence<T, long>
	{
	public:
		// Inherit constructor from base class
		using Sequence<T,long>::Sequence;

	protected:
		long m_numOfScansAlreadyDone{};
		// Switches to the step. Returns true if it was possible and false if the sequence is finished
		bool switchToNextStep();
        // Returns true if current step is finished
        bool isCurrentStepFinished();
        // Contains stuff to do to advance the step towards its length
        void stepAdvance();
	};

	template <typename T>
	bool ScanValue<T>::switchToNextStep()
	{
		// Switch to a step that is next to current one AND has at least one associated scan
		m_numOfScansAlreadyDone = 0;

		// Start searching from next element
		auto begin = m_sequenceDef.begin() + static_cast<std::size_t>(this->m_currentStepIndex + 1);
		// One after the end
		auto end = m_sequenceDef.begin() + static_cast<std::size_t>(this->m_numberOfSteps);		   

		auto it = std::find_if(begin, end, [](const ScanValue<T>::StepPar &step)
							   { return step.numberOfScans > 0; }); // Look for a step that has at least one scan

		if (it == end)
		{
			this->m_currentStepIndex = this->noStepIdx;
			return false;
		}

		this->m_currentStepIndex = static_cast<int>(std::distance(m_sequenceDef.begin(), it));
		return true;
	}

	template <typename T>
	bool ScanValue<T>::isCurrentStepFinished()
	{
		return m_numOfScansAlreadyDone >= m_sequenceDef[static_cast<std::size_t>(this->m_currentStepIndex)].length;
	}

	template <typename T>
	void ScanValue<T>::stepAdvance()
	{
		++m_numOfScansAlreadyDone;
	}

} // namespace sequence

#endif // SCAN_VALUE_HPP
