#include "zen/core/uuid.hpp"


namespace zen {

namespace core {


// init static members
std::random_device UUID::_trueRandom;
std::mt19937_64 UUID::_psuedoGenerator{UUID::_trueRandom()};
std::uniform_int_distribution<int64_t> UUID::_uniformDistributer;

// constructors
UUID::UUID(bool fromFile)
    : _uuid(fromFile
        ? std::abs(_uniformDistributer(_psuedoGenerator))
        : -std::abs(_uniformDistributer(_psuedoGenerator))) {}

UUID::UUID(int64_t uuid)
    : _uuid(uuid) {}

// conversion operator
UUID::operator int64_t() const {
    return _uuid;
}


} // namespace core

} // namespace zen
