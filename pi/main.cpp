#include "data.hpp"
#include "get_data.hpp"
#include "entry_cache.hpp"
#include "pi_config.hpp"
#include "shared_config.hpp"
#include "rand.hpp"
#include "storage.hpp"
#include "storage_format.hpp"
#include "log.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <thread>

void write_cache(EntryCache cache){ // take cache by value
    UnixMillis start = get_current_unix_millis();

    StorageFile sfile;
    StorageDirectory sdir;
    get_storage_file(&cache, &sfile, &sdir);
    store_cache(&cache, &sfile);

    UnixMillis end = get_current_unix_millis();

    LOG("parse + write took" << end - start << std::endl);
}


int main(){
    EntryCache cache;

    EntryCache* cacheptr = &cache;
    bool run = true;

    Time time;

    std::thread t(generate_event_based_entries, &run, &cacheptr);

    LOG("start\n");

    while (1){
        LOG("\n");
        std::unique_lock<std::mutex> lock(push_event_based_mutex, std::defer_lock);
        lock.lock();

        cache.reset();

        lock.unlock();

        time.update();
        cache.first_entry_timestamp = time.timestamp;

        uint64_t seconds_left_in_day = time.get_seconds_left_in_day() + 1;
        size_t cache_n_entries = std::min((uint64_t)DATA_CACHE_SIZE, seconds_left_in_day);


        if (cache_n_entries != DATA_CACHE_SIZE){
            LOG("short cache" << time.timestamp.time_since_epoch().count() << "\n");
        }

        assert(cache_n_entries != 0);


        for (size_t i = 0; i < cache_n_entries; i++){
            // add entries;
            ContinuousEntry centry = get_continuous_entry();
            
            cache.push_continuous(centry);

            time.wait_for_next_second();
        }

        LOG("writing cache...");
        //wait_for_next_second();


        std::thread t2(write_cache, cache);
        t2.detach();
    }
} 