#pragma once

#include "data.hpp"
#include "pi_config.hpp"
#include "status.hpp"
#include "time.hpp"

#include <vector>

class EntryCache{
    public:
    
    Timestamp first_entry_timestamp;

    size_t n_event_based_entries = N_EVENT_BASED_ENTRY_TYPES;

    size_t n_entry_types = N_CONTINUOUS_ENTRY_TYPES;

    std::vector<ContinuousEntryType> continuous_entry_cache;
    std::vector<std::vector<EventBasedEntry> > event_based_entry_cache;

    EntryCache();

    void push_continuous(ContinuousEntry entry);
    void push_event_based(UnixMillis timestamp, size_t type);

    void reset();

    void print_cached();

    Status verify();
};