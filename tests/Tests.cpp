#include "ScanValueTests.hpp"
#include "TimeValueTests.hpp"

int main()
{
    scanValueTests::basicSequenceTest();
    scanValueTests::moveHappensTest();

    timeValueTests::basicSequenceTest();

    std::cout << "All ScanValue tests passed.\n";
    return 0;
}
