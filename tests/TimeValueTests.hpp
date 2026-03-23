#include "TimeValue.hpp"
#include "TimeDuration.hpp"
#include <vector>
#include <cassert>
#include <iostream>
#include <thread>

// Unit tests of timevalue class

namespace timeValueTests
{
    void basicSequenceTest()
    {
        std::cout << "Starting basic sequence test\n";

        using namespace valueSequencer;

        timer::TimeDuration timeDuration{};

        // Define a sequence: each Step = {value, duration_in_ms}
        TimeValue<int> timeValue{timeDuration, 
            {{1, static_cast<timer::time_ms>(0)}, 
            {2, static_cast<timer::time_ms>(1000)}, 
            {0, static_cast<timer::time_ms>(3000)}, 
            {3, static_cast<timer::time_ms>(4000)}}, 
            999}; // 4-values sequence with idle value

        // Initially, sequence has not started, so executeSequence returns idle value
        assert(timeValue.executeSequence() == 999);

        // Start the sequence
        timeValue.start();
        assert(timeValue.isSequenceRunning());

        // Execute sequence step by step
        // For TimeValue, value is repeated according to step duration
        assert(timeValue.executeSequence() == 2);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        assert(timeValue.executeSequence() == 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(4000));
        assert(timeValue.executeSequence() == 3);

        // After finishing all steps, sequence returns idle value
        assert(timeValue.executeSequence() == 999);
        assert(!timeValue.isSequenceRunning());

        std::cout << "Basic sequence test completed\n";
    }
}
