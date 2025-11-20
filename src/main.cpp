#include <iostream>
#include "profile.hpp"
#include "syracouse_info.hpp"

using namespace syracuse;

int main() {

    SyracuseCalculator calculator(1'000'000'000);

    uint64_t range_limit{0};
    std::cout << "Enter range limit: ";
    std::cin >> range_limit;
    std::cout << "Enter threads amount: ";
    uint16_t threads_amoutn{0};
    std::cin >> threads_amoutn;
    LOG_DURATION("Calculation duration");
    SyracuseInfo result = calculator.FindLongestSequenceParalel(range_limit, threads_amoutn);
    std::cout << "Longest Syracuse sequence up to " << range_limit << " starts at "
              << result.start_value << " with length " << result.sequence_length << std::endl;
    return 0;
}
