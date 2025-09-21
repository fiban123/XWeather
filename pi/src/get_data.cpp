#include "get_data.hpp"

#include "rand.hpp"

#include "time.hpp"

#include <stdint.h>
#include <unistd.h>
#include <ctime>

ContinuousEntry get_continuous_entry(){
    double rand;
    randrange_double(21.0, 23.0, &rand);
    return ContinuousEntry({rand});
}

void generate_event_based_entries(bool* run, DataEntryCache** cache){
    while (*run){
        uint64_t rand_sleep;
        randrange_u64(1000000, 10000000, &rand_sleep);

        usleep(rand_sleep);

        (*cache)->push_event_based(get_current_unix_millis(), 0);
    }
}