#include "Utils/i2v.h"

#include "Utils/f2v.h"

namespace utils {

f2v i2v::tof2v() const { return f2v(static_cast<float>(x), static_cast<float>(y)); }

}