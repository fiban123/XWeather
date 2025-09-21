#pragma once

#include "data.hpp"
#include "pi_config.hpp"
#include "status.hpp"
#include "time.hpp"

#include <vector>

class DataEntryCache{
    public:
    
    UnixMillis first_entry_timestamp;

    size_t i;

    std::array<ContinuousEntry, DATA_CACHE_SIZE> continuous_entry_cache;
    std::array<std::vector<EventBasedEntry>, N_EVENT_BASED_ENTRIES> event_based_entry_cache;

    DataEntryCache();

    void push_continuous(ContinuousEntry entry);
    void push_event_based(UnixMillis timestamp, size_t type);

    void reset();

    void print_cached();

    Status verify();
};