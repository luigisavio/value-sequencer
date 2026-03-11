#ifndef SCAN_VALUE_HPP
#define SCAN_VALUE_HPP

#include <vector>

namespace sequence
{
    // Abstract class to be used as parent to define a sequence
	template <typename T, typename U> // T it the value to be changed, U defines for how much each value must be assigned
	class Sequence
	{
	public:
		static constexpr int noStepIdx{-1};

		struct Step
		{
			T value{};
            U length{};
		};

		// Generic constructor
		// Passing the vector by value. If called on an lvalue a copy is made. Otherwise move constructor is called.
		// In both cases, the vector inside this function can be cleared by moving it to another vector for better 
		// performances.
		Sequence(std::vector<Step> sequenceDef, const T &idleValue = {})
        {
			setSequence(std::move(sequenceDef), idleValue);
		}

		// Default constructor
		Sequence() {}

		// Configure the sequence. If object was already doing something it is reset.
		// Pass vector by value: if it is an lvalue it will copied, otherwise moved.
		bool setSequence(std::vector<Step> sequenceDef, const T &idleValue = {});

		// Simply returns the value
		const T &getValue() const;

		// Updates time and returns the integral value after the update
		const T &executeSequence();

		// Completely resets the object, equivalent to creating a new one with constructor method
		Sequence &reset();

		// Triggers the start of the sequences
		Sequence &start()
		{
			m_startSequenceRequest = true;
			return *this;
		}

		// Checks if sequence is running
		bool isSequenceRunning() const { return (m_currentStepIndex != noStepIdx) || m_startSequenceRequest; }

        virtual ~Sequence() = default;

	private:
		std::vector<Step> m_sequenceDef{}; // Values of the number and times during the sequence
		T m_idleValue{};
		int m_currentStepIndex{noStepIdx}; // Current index of the step
		int m_numberOfSteps{};			   // Total number of steps to be executed, will be used in derived classes
		bool m_startSequenceRequest{false};

		// Switches to the step. Returns true if it was possible and false if the sequence is finished
		bool switchToNextStep() = 0;
        // Returns true if current step is finished
        bool isCurrentStepFinished() = 0;
        // Contains stuff to do to advance the step towards its length
        void stepAdvance() = 0;
	};

	// Template functions must be defined in header file since every TU needs to see the implementation to compile the required instance
	template <typename T, typename U>
	const T &Sequence<T, U>::getValue() const
	{
		return (m_currentStepIndex == noStepIdx) ? m_idleValue
												 : m_sequenceDef[static_cast<std::size_t>(m_currentStepIndex)].value;
	}

	template <typename T, typename U>
	const T &Sequence<T, U>::executeSequence()
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
                // Advance, do it only if sequence is running for efficiency
				stepAdvance();

				// Check required number of scans have already happened
				if (isCurrentStepFinished())
				{
					switchToNextStep();
				}
			}
		}
		return getValue();
	}

	template <typename T, typename U>
	bool Sequence<T, U>::setSequence(std::vector<Step> sequenceDef,const T &idleValue)
	{
		reset();
        // Avoid expensive copy by using move semantics on std vector (move assignment operator)
		m_sequenceDef = std::move(sequenceDef);
		m_idleValue = idleValue;
        // Get number of steps to be executed from size of steps vector
		m_numberOfSteps = static_cast<int>(m_sequenceDef.size());
		return m_numberOfSteps != 0;
	}

	template <typename T, typename U>
	Sequence<T, U> &Sequence<T, U>::reset()
	{
		*this = Sequence{};
		return *this;
	}

} // namespace sequence

#endif // SCAN_VALUE_HPP
