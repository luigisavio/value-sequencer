#include "ScanValue.hpp"
#include <cassert>
#include <iostream>

// Unit tests of ScanValue class
void basicSequenceTest()
{
    std::cout << "Starting basic sequence test\n";

    using namespace valueSequencer;

    // Define a sequence: each Step = {value, duration_in_scans}
    ScanValue<int> scanValue{{{1, 0}, {2, 1}, {0, 3}, {3, 4}}, 999}; // 4-values sequence with idle value

    // Initially, sequence has not started, so executeSequence returns idle value
    assert(scanValue.executeSequence() == 999);

    // Start the sequence
    scanValue.start();
    assert(scanValue.isSequenceRunning());

    // Execute sequence step by step
    // For ScanValue, value is repeated according to step duration (number of scans)
    assert(scanValue.executeSequence() == 2);
    assert(scanValue.executeSequence() == 0);
    assert(scanValue.executeSequence() == 0);
    assert(scanValue.executeSequence() == 0);
    assert(scanValue.executeSequence() == 3);
    assert(scanValue.executeSequence() == 3);
    assert(scanValue.executeSequence() == 3);
    assert(scanValue.executeSequence() == 3);

    // After finishing all steps, sequence returns idle value
    assert(scanValue.executeSequence() == 999);
    assert(!scanValue.isSequenceRunning());

    std::cout << "Basic sequence test completed\n";
}

void moveHappensTest()
{
    std::cout << "Starting test that checks move happens\n";

    using namespace valueSequencer;

    std::vector<ScanValue<int>::Step> seq {{{1, 0}, {2, 1}, {0, 3}, {3, 4}}};
    assert(!seq.empty());

    {
        // Copy constructor (passing lvalue)
        ScanValue<int> scanValue{seq};
    }

    // Original vector should remain unchanged
    assert(!seq.empty());

    {
        // Move constructor (passing rvalue)
        ScanValue<int> scanValue{std::move(seq)};
    }

    // Original vector should now be empty
    assert(seq.empty());

    std::cout << "Test check move happened completed\n";
}
