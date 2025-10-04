#include "get_data.hpp"

#include "rand.hpp"

#include "time.hpp"

#include <stdint.h>
#include <unistd.h>
#include <ctime>

std::mutex push_event_based_mutex;

ContinuousEntry get_continuous_entry(){
    double rand;
    randrange_double(5, 6, &rand);
    return ContinuousEntry({rand});
}

void generate_event_based_entries(bool* run, EntryCache** cache){
    while (*run){
        uint64_t rand_sleep;
        randrange_u64(1000000, 10000000, &rand_sleep);

        usleep(rand_sleep);

        std::unique_lock<std::mutex> lock(push_event_based_mutex, std::defer_lock);

        lock.lock();
        (*cache)->push_event_based(get_current_unix_millis(), 0);
        lock.unlock();

    }
}