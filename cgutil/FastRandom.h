#pragma once

#include "array"
#include "cstdint"
#include "ctime"

namespace cutil
{
    class FastRandom
    {
        std::array<uint64_t, 4> state;

        static uint64_t rotl(uint64_t x, int k);
        uint64_t lemire(uint64_t range);

    public:
        explicit FastRandom(uint64_t seed = clock());

        uint64_t u64();
        uint32_t u32();
        double f64();
        float f32();

        /**
         * [min, max]
         */
        uint64_t range(uint64_t min, uint64_t max);
        uint32_t range(uint32_t min, uint32_t max);
    };
}
