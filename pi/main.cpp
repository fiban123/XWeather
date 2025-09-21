

#include "data.hpp"
#include "get_data.hpp"
#include "entry_cache.hpp"
#include "pi_config.hpp"
#include "shared_config.hpp"
#include "rand.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <thread>

int main(){
    
    DataEntryCache cache;
    
    cache.first_entry_timestamp = wait_for_next_second();
    DataEntryCache* cacheptr = &cache;

    bool run = true;

    std::thread t(generate_event_based_entries, &run, &cacheptr);


    for (size_t i = 0; i < 10; i++){
        // add entries;
        ContinuousEntry centry = get_continuous_entry();
        
        cache.push_continuous(centry);

        wait_for_next_second();
    }

    run = false;
    t.join();

    cache.print_cached();
}