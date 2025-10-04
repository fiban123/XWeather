#pragma once

#include "shared_config.hpp"

#include <ctime>
#include <stdint.h>
#include <unistd.h>
#include <chrono>


typedef std::chrono::system_clock::time_point Timestamp;
typedef std::chrono::system_clock::duration UnixTime;
typedef std::chrono::milliseconds Millis;
typedef std::chrono::seconds Seconds;
typedef std::chrono::years Years;
typedef std::chrono::time_point<std::chrono::local_t, std::chrono::days> LocalDays;

typedef std::chrono::year Year;
typedef std::chrono::sys_days Day;

typedef uint64_t UnixMillis;
typedef uint64_t UnixSecs;

inline const std::chrono::time_zone* time_zone = std::chrono::locate_zone(TIMEZONE);


struct Time{
    Timestamp timestamp;

    LocalDays d;

    void update();

    Timestamp wait_for_next_second();
    uint64_t get_seconds_left_in_day();
};

Timestamp get_current_time();
UnixMillis get_current_unix_millis();

Year get_current_year();

LocalDays get_day(Timestamp t);

uint64_t get_seconds_left_in_day();