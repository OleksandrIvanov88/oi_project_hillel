#ifndef SYRACOUSE_INFO_HPP
#define SYRACOUSE_INFO_HPP

#include <vector>
#include <atomic>
#include <cstdint>
#include <memory>
#include <limits>

namespace syracuse {

struct SyracuseInfo {
    uint64_t start_value;
    uint16_t sequence_length;
};

class SyracuseCalculator {
public:
    explicit SyracuseCalculator(size_t cache_size);
    ~SyracuseCalculator() = default;
    SyracuseCalculator(const SyracuseCalculator&) = delete;
    SyracuseCalculator& operator=(const SyracuseCalculator&) = delete;
    SyracuseCalculator(SyracuseCalculator&&) = default;
    SyracuseCalculator& operator=(SyracuseCalculator&&) = default;

    uint16_t CalculateSequenceLength(uint64_t n);
    SyracuseInfo FindLongestSequence(uint64_t range_limit);
    SyracuseInfo FindLongestSequenceParalel(uint64_t range_limit, uint16_t num_threads);

private:
    static constexpr uint64_t MAX_SAFE_ODD{(std::numeric_limits<uint64_t>::max() - 1) / 3};
    size_t cache_size_{};
    std::vector<std::atomic<uint16_t>> cache_{};

    SyracuseInfo FindLongestSequenceInRange(uint64_t start, uint64_t end);
};
} // namespace syracuse
#endif // SYRACOUSE_INFO_HPP