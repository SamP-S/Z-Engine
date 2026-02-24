#pragma once

#include <cstdint>
#include <random>
#include <cmath>


namespace zen {

namespace core {


// Universal Unique Identifier
// int64_t wrapper
// 0 : Invalid
// > 0 : from file
// < 0 : at runtime
struct UUID {
private:
    int64_t _uuid = 0;

    // Non-deterministic random number
    // Seed the pseudo number gen
    static std::random_device _trueRandom;

    // Psuedo random generator
    // Mersenne Twister algorithm
    // 2^19937 - 1 before repeating
	static std::mt19937_64 _psuedoGenerator;

    // even chance across whole UUID space
	static std::uniform_int_distribution<int64_t> _uniformDistributer;

public:

    // constructors
	UUID(bool fromFile = false);
	UUID(int64_t uuid);

    // conversion operator
    operator int64_t() const;

};


} // namespace core

} // namespace zen
