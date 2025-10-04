#include "entry_cache.hpp"

#include <iostream>

EntryCache::EntryCache(){
    reset();
}

void EntryCache::push_continuous(ContinuousEntry entry){
    continuous_entry_cache.insert(continuous_entry_cache.end(), entry.begin(), entry.end());
    //continuous_entry_cache.push_back(entry);
    //std::cout << "continuous entry added: " << entry[0] << ", " << get_current_unix_millis() << std::endl;
}

void EntryCache::push_event_based(UnixMillis timestamp, size_t type){
    event_based_entry_cache[type].push_back(timestamp);
    //std::cout << "event based entry added: " << timestamp << ", " << get_current_unix_millis() << std::endl;
}

void EntryCache::reset(){
    continuous_entry_cache.clear();
    event_based_entry_cache.clear();
    event_based_entry_cache.resize(n_event_based_entries);
}

void EntryCache::print_cached(){
    UnixSecs first_entry_secs = first_entry_timestamp.time_since_epoch().count() / 1000.0;
    std::cout << "first entry timestamp" << first_entry_secs << std::endl;

    std::cout << "continuous entries: " << std::endl;
    for (size_t j = 0; j < continuous_entry_cache.size(); j++){
        std::cout << j << " " << continuous_entry_cache[j]<< ", " << first_entry_secs + j << std::endl;
    }

    std::cout << "event-based entries: " << std::endl;

    for (size_t j = 0; j < event_based_entry_cache[0].size(); j++){
        std::cout << j << " " << event_based_entry_cache[0][j] << std::endl;
    }
}

Status EntryCache::verify(){
    return Status(SUCCESS);
}
