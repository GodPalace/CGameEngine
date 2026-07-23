#include "FastRandom.h"

uint64_t cutil::FastRandom::rotl(uint64_t x, int k)
{
    return x << k | x >> (64 - k);
}

uint64_t cutil::FastRandom::lemire(uint64_t range)
{
    uint64_t x = u64();
    __uint128_t product = static_cast<__uint128_t>(x) * range;
    auto low = static_cast<uint64_t>(product);

    if (low < range)
    {
        uint64_t t = -range % range;
        while (low < t)
        {
            x = u64();
            product = static_cast<__uint128_t>(x) * range;
            low = static_cast<uint64_t>(product);
        }
    }

    return product >> 64;
}

cutil::FastRandom::FastRandom(uint64_t seed) : state()
{
    // splitmix64 init
    for (auto& s : state)
    {
        seed += 0x9e3779b97f4a7c15;
        uint64_t z = seed;
        z = (z ^ z >> 30) * 0xbf58476d1ce4e5b9;
        z = (z ^ z >> 27) * 0x94d049bb133111eb;
        s = z ^ z >> 31;
    }
}

uint64_t cutil::FastRandom::u64()
{
    uint64_t result = rotl(state[1] * 5, 7) * 9;
    uint64_t t = state[1] << 17;
    state[2] ^= state[0];
    state[3] ^= state[1];
    state[1] ^= state[2];
    state[0] ^= state[3];
    state[2] ^= t;
    state[3] = rotl(state[3], 45);
    return result;
}

uint32_t cutil::FastRandom::u32()
{
    return static_cast<uint32_t>(u64());
}

double cutil::FastRandom::f64()
{
    return static_cast<double>(u64() >> 11) * (1.0 / (1ULL << 53));
}

float cutil::FastRandom::f32()
{
    return static_cast<float>(static_cast<double>(u64() >> 40) * (1.0f / (1ULL << 24)));
}

uint64_t cutil::FastRandom::range(uint64_t min, uint64_t max)
{
    uint64_t range = max - min + 1;
    return lemire(range) + min;
}

uint32_t cutil::FastRandom::range(uint32_t min, uint32_t max)
{
    return static_cast<uint32_t>(range(static_cast<uint64_t>(min), static_cast<uint64_t>(max)));
}
