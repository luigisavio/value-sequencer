#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <vector>

namespace valueSequencer
{
    /**
	 * @brief Abstract class to create sequences of values.
	 *
	 * This class provides a base interface for generating sequences where
	 * a value is assigned for a specified duration.
	 *
	 * @tparam T Type of value assigned during the sequence.
	 * @tparam U Type used to represent how long each value is assigned.
	 */
	template <typename T, typename U>
	class Sequence
	{
	public:
		static constexpr int noStepIdx{-1};

		struct Step
		{
			T value{};
            U length{};
		};

		/**
		 * @brief Generic constructor.
		 *
		 * The sequence definition vector is passed by value. If the constructor
		 * is called with an lvalue, a copy is created; if called with an rvalue,
		 * the move constructor is used.
		 * In both cases, the vector inside this function can be moved into the
		 * internal storage, allowing the original container to be cleared and
		 * improving performance.
		 *
		 * @param sequenceDef Vector defining the sequence steps.
		 * @param idleValue Value assigned when the sequence is idle (default constructed if not specified).
		 */
		Sequence(std::vector<Step> sequenceDef, const T &idleValue = {})
        {
			setSequence(std::move(sequenceDef), idleValue);
		}

		/**
		 * @brief Default constructor.
		 */
		Sequence() {}

		/**
		 * @brief Configures the sequence.
		 *
		 * If the object was already executing a sequence, it is reset before
		 * applying the new configuration.
		 * The sequence definition vector is passed by value: if an lvalue is
		 * provided it will be copied, otherwise it will be moved.
		 *
		 * @param sequenceDef Vector defining the sequence steps.
		 * @param idleValue Value assigned when the sequence is idle.
		 * @return True if the sequence was configured successfully.
		 */
		bool setSequence(std::vector<Step> sequenceDef, const T& idleValue = {});

		/**
		 * @brief Returns the current value of the sequence.
		 *
		 * @return Reference to the current value.
		 */
		const T& getValue() const;

		/**
		 * @brief Updates the sequence progress and returns the resulting value.
		 *
		 * This function advances the internal state of the sequence according
		 * to the elapsed execution step and returns the updated value.
		 *
		 * @return Reference to the updated value.
		 */
		const T& executeSequence();

		/**
		 * @brief Resets the sequence state.
		 *
		 * Restores the object to the same state as after default construction.
		 *
		 * @return Reference to the current object.
		 */
		Sequence& reset();

		/**
		 * @brief Requests the start of the sequence.
		 *
		 * The sequence will start on the next execution cycle.
		 *
		 * @return Reference to the current object.
		 */
		Sequence& start()
		{
			m_startSequenceRequest = true;
			return *this;
		}

		/**
		 * @brief Checks whether the sequence is running or scheduled to start.
		 *
		 * The sequence is considered running if a step is currently active,
		 * or if a start request has been issued but not yet processed.
		 *
		 * @return True if the sequence is running or pending start, false otherwise.
		 */
		bool isSequenceRunning() const
		{
			return (m_currentStepIndex != noStepIdx) || m_startSequenceRequest;
		}

        virtual ~Sequence() = default;

	protected:
		// Values of the number and times during the sequence
		std::vector<Step> m_sequenceDef{};
		T m_idleValue{};
		// Current index of the step
		int m_currentStepIndex{noStepIdx};
		// Total number of steps to be executed, will be used in derived classes
		int m_numberOfSteps{};
		bool m_startSequenceRequest{false};

		// # Pure virtual member functions
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
		// Check sequence is running
		if (m_currentStepIndex != noStepIdx)
		{
			// Sequence already running, ignore requests to start the sequence
			m_startSequenceRequest = false;

			// Advance, do it only if sequence is running for efficiency
			stepAdvance();

			// Check required number of scans have already happened
			if (isCurrentStepFinished())
			{
				switchToNextStep();
			}
		}else if (m_startSequenceRequest) // Check request to start the sequence
		{
			m_startSequenceRequest = false;
			switchToNextStep();
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

} // namespace valueSequencer

#endif // SCAN_VALUE_HPP
