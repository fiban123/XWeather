#include "rand.hpp"

#include <fstream>
#include <limits>
#include <stdexcept>

Status get_random64(uint64_t* out){
    if (!out) return Status(FAIL, "get_random64: out pointer is null");

    uint64_t val;
    std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
    if (!urandom) return Status(FAIL, "cannot open /dev/urandom");

    urandom.read(reinterpret_cast<char*>(&val), sizeof(val));
    if (!urandom) return Status(FAIL, "failed to read random data");

    *out = val;
    return Status(SUCCESS);
}

Status randrange_u64(uint64_t min, uint64_t max, uint64_t* out) {
    if (!out) return Status(FAIL, "randrange_u64: out pointer is null");
    if (max < min) return Status(FAIL, "randrange64: invalid range");

    uint64_t range = max - min + 1;
    uint64_t value;

    // Use rejection sampling to avoid modulo bias
    uint64_t limit = UINT64_MAX - (UINT64_MAX % range);
    do {
        Status status = get_random64(&value);
        if (status.fail()){
            return status.add_trace("randrange64: get_random64 failed");
        }
    } while (value >= limit);

    *out = min + (value % range);

    return Status(SUCCESS);
}

Status randrange_double(double min, double max, double* out) {
    if (!out) return Status(FAIL, "randrange_double: out pointer is null");
    if (max < min) return Status(FAIL, "randrange_double: invalid range");

    // map [0, UINT64_MAX] -> [0.0, 1.0)
    uint64_t rand64;

    Status status = get_random64(&rand64);
    if (status.fail()){
        return status.add_trace("randrange_double: get_random64 failed");
    }

    double unit = (double)rand64 / (double)UINT64_MAX;
    *out =  min + (unit * (max - min));
    return Status(SUCCESS);
}