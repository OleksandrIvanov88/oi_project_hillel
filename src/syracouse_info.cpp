#include "syracouse_info.hpp"
#include <stdexcept>
#include <thread>
#include <future>
#include <algorithm>
#include <limits>
#include <iostream>

namespace syracuse {

SyracuseCalculator::SyracuseCalculator(size_t cache_size) : cache_(cache_size) {}

uint16_t SyracuseCalculator::CalculateSequenceLength(uint64_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    if (n < cache_.size() && cache_[n].load() != 0) {
        return cache_[n].load();
    }

    uint64_t original_n = n;
    uint16_t length{1};
    
    while (n != 1) {
        if (n < cache_.size() && cache_[n].load() != 0) {
            length += cache_[n].load() - 1;
            break;
        }
        bool is_odd{n % 2 != 0};
        if(is_odd && n > MAX_SAFE_ODD) {
            throw std::overflow_error("Overflow would occur");
        }
        n = is_odd ? (3 * n + 1) : (n / 2);
        ++length;
    }

    if (original_n < cache_.size()) {
        cache_[original_n].store(length);
    }

    return length;
}


SyracuseInfo SyracuseCalculator::FindLongestSequence(uint64_t range_limit) {
    return FindLongestSequenceInRange(1, range_limit);
}

SyracuseInfo SyracuseCalculator::FindLongestSequenceParalel(uint64_t range_limit, uint16_t num_threads) {
    if (range_limit == 0) return SyracuseInfo{0, 0};
    
    if (num_threads == 0) {
        num_threads = std::thread::hardware_concurrency();
        if (num_threads == 0) num_threads = 8;
        std::cout << "WARNING! Using " << num_threads << " threads for calculation." << std::endl;
    }

    uint64_t chunk_size{range_limit / num_threads};
    if (chunk_size == 0) return FindLongestSequence(range_limit);

    std::vector<std::future<SyracuseInfo>> futures;

    for (uint16_t i{0}; i < num_threads; ++i) {
        uint64_t start{i * chunk_size + 1};
        uint64_t end{(i == num_threads - 1) ? range_limit : (i + 1) * chunk_size};
        
        if (start <= end) {
            futures.emplace_back(
                std::async(std::launch::async,
                    &SyracuseCalculator::FindLongestSequenceInRange, this, start, end));
        }
    }

    SyracuseInfo global_longest{1, 1};
    for (auto& future : futures) {
        SyracuseInfo result = future.get();
        if (result.sequence_length > global_longest.sequence_length) {
            global_longest = result;
        }
    }

    return global_longest;
}

SyracuseInfo SyracuseCalculator::FindLongestSequenceInRange(uint64_t start, uint64_t end) {
    SyracuseInfo longest{1, 1};
    
    for (uint64_t n = start; n <= end; ++n) {
        uint16_t length = CalculateSequenceLength(n);
        if (length > longest.sequence_length) {
            longest = SyracuseInfo{n, length};
        }
    }
    
    return longest;
}
} // namespace syracuse
