#include "ScanValue.hpp"
#include <vector>
#include <cassert>
#include <iostream>

// Unit tests of scanvalue class

void basicSequenceTest()
{
    std::cout << "Starting basic sequence test\n";

    using namespace sequence;

    ScanValue<int> scanValue{{{1, 0}, {2, 1}, {0, 3}, {3, 4}}, 999}; // 4 steps sequence with idle value

    assert(scanValue.executeSequence() == 999); // Check idle value

    scanValue.start(); // Start the sequence

    assert(scanValue.isSequenceRunning()); // Check sequence is running

    // Check sequence
    assert(scanValue.executeSequence() == 0);
    assert(scanValue.executeSequence() == 1);
    assert(scanValue.executeSequence() == 1);
    assert(scanValue.executeSequence() == 4);
    assert(scanValue.executeSequence() == 4);
    assert(scanValue.executeSequence() == 4);
    assert(scanValue.executeSequence() == 999);

    assert(!scanValue.isSequenceRunning()); // Check sequence is not running anymore

    std::cout << "Basic sequence test completed\n";
}

void moveHappensTest()
{
    std::cout << "Starting test that checks move happens\n";

    using namespace sequence;

    std::vector<ScanValue<int>::StepPar> seq {{{1, 0}, {2, 1}, {0, 3}, {3, 4}}};

    assert(seq.size() != 0);

    {
        ScanValue<int> scanValue{seq}; // Move should not happen
    }

    assert(seq.size() != 0);

    {
        ScanValue<int> scanValue{std::move(seq)}; // Now move should happen
    }

    assert(seq.size() == 0); // Check that sequence is empty since it must have been moved to scanValue

    std::cout << "Test check move happened completed\n";
}
