#include "entry_cache.hpp"

#include <iostream>

DataEntryCache::DataEntryCache(){
    reset();
}

void DataEntryCache::push_continuous(ContinuousEntry entry){
    continuous_entry_cache[i] = entry;
    i++;
    std::cout << "continuous entry added: " << entry[0] << ", " << get_current_unix_millis() << std::endl;
}

void DataEntryCache::push_event_based(UnixMillis timestamp, size_t type){
    event_based_entry_cache[type].push_back(timestamp);
    std::cout << "event based entry added: " << timestamp << ", " << get_current_unix_millis() << std::endl;
}

void DataEntryCache::reset(){
    i = 0;
    continuous_entry_cache.fill(ContinuousEntry({0}));
}

void DataEntryCache::print_cached(){
    UnixSecs first_entry_secs = first_entry_timestamp / 1000;
    std::cout << "first entry timestamp" << first_entry_secs << std::endl;

    std::cout << "continuous entries: " << std::endl;
    for (size_t j = 0; j < i; j++){
        std::cout << j << " " << continuous_entry_cache[j][0 ]<< ", " << first_entry_secs + j << std::endl;
    }

    std::cout << "event-based entries: " << std::endl;

    for (size_t j = 0; j < event_based_entry_cache[0].size(); j++){
        std::cout << j << " " << event_based_entry_cache[0][j] << std::endl;
    }
}

Status DataEntryCache::verify(){
    return Status(SUCCESS);
}
