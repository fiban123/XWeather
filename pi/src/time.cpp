#include "time.hpp"


Timestamp get_current_time(){
    std::chrono::system_clock::time_point systime = std::chrono::system_clock::now();

    return systime;
}

Year get_current_year(){
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    std::chrono::year_month_day ymd(std::chrono::floor<std::chrono::days>(now));

    return ymd.year();
}

LocalDays get_day(Timestamp t){
    namespace chr = std::chrono;

    auto zt = chr::zoned_time(time_zone, t);

    auto local = zt.get_local_time();


    return chr::floor<chr::days>(local);
}


UnixMillis get_current_unix_millis(){
    namespace chr = std::chrono;

    chr::system_clock::time_point now = chr::system_clock::now();

    UnixTime unix_time = now.time_since_epoch();

    return chr::duration_cast<chr::milliseconds>(unix_time).count();
}


uint64_t get_seconds_left_in_day()
{
    return 0;
}

void Time::update(){
    timestamp = get_current_time();

    d = get_day(timestamp);
}

Timestamp Time::wait_for_next_second(){
    namespace chr = std::chrono;

    update();

    chr::sys_seconds next_second = chr::time_point_cast<chr::seconds>(timestamp) + chr::seconds(1);

    int64_t us = chr::duration_cast<chr::microseconds>(next_second - timestamp).count();

    usleep(us);

    update();

    return timestamp;
}

uint64_t Time::get_seconds_left_in_day(){
    namespace chr = std::chrono;

    auto zt = chr::zoned_time(time_zone, timestamp);

    auto local = zt.get_local_time();

    auto local_day = chr::floor<chr::days>(local);
    auto next_day = local_day + chr::days(1);

    return chr::duration_cast<chr::seconds>(next_day - local).count();
}

