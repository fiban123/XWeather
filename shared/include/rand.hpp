#pragma once

#include "status.hpp"

#include <cstdint>

Status get_random64(uint64_t* out);

Status randrange_u64(uint64_t min, uint64_t max, uint64_t* out);

Status randrange_double(double min, double max, double* out);