#pragma once

#include <ctime>
#include <stdint.h>
#include <unistd.h>
#include <chrono>

typedef int64_t UnixMillis;
typedef int64_t UnixSecs;
typedef int64_t Millis;

UnixMillis get_current_unix_millis();

UnixMillis wait_for_next_second();