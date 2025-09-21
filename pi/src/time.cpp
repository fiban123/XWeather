#include "time.hpp"

UnixMillis get_current_unix_millis(){
    std::chrono::system_clock::time_point systime = std::chrono::system_clock::now();

    std::chrono::system_clock::duration duration = systime.time_since_epoch();

    UnixMillis millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    return millis;
}

UnixMillis wait_for_next_second(){
    UnixMillis start = get_current_unix_millis();
    
    UnixSecs start_s = start / 1000;

    UnixSecs end_s = start_s + 1;
    UnixMillis end = end_s * 1000;

    Millis diff = end - start;

    usleep(diff * 1000);

    return end;
}