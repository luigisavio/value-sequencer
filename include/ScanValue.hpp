#ifndef SCAN_VALUE_HPP
#define SCAN_VALUE_HPP

#include "Sequence.hpp"
#include <algorithm>

namespace valueSequencer
{
	/**
	 * @brief Generates a value that changes based on the number of times a function is called.
	 *
	 * This class derives from Sequence and advances its value according
	 * to how many “scan” steps (function calls) have been executed.
	 *
	 * @tparam T Type of the value to be modified during the sequence.
	 */
	template <typename T>
	class ScanValue : public Sequence<T, long>
	{
	public:
		/**
		 * @brief Inherit constructors from the base Sequence class.
		 *
		 * No additional initialization is required.
		 */
		using Sequence<T, long>::Sequence;

	protected:
		/// Number of scans already performed in the current step.
		long m_numOfScansAlreadyDone{};

		/**
		 * @brief Advances the sequence to the next step.
		 *
		 * @return True if the step was successfully advanced, false otherwise.
		 */
		bool switchToNextStep();

		/**
		 * @brief Checks if the current step has finished based on scan count.
		 *
		 * @return True if the current step is complete, false otherwise.
		 */
		bool isCurrentStepFinished();

		/**
		 * @brief Performs a single advance of the sequence based on scan count.
		 *
		 * Updates internal state and increments `m_numOfScansAlreadyDone`.
		 */
		void stepAdvance();
	};

	template <typename T>
	bool ScanValue<T>::switchToNextStep()
	{
		m_numOfScansAlreadyDone = 0;
		// Search for a step that is next and has at leas one associated scan
		// Start searching from next element
		auto begin = m_sequenceDef.begin() + static_cast<std::size_t>(this->m_currentStepIndex + 1);
		// One after the end
		auto end = m_sequenceDef.begin() + static_cast<std::size_t>(this->m_numberOfSteps);		   

		// Look for a step that has at least one scan
		auto it = std::find_if(begin, end, [](const ScanValue<T>::StepPar &step)
							   { return step.numberOfScans > 0; });

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
