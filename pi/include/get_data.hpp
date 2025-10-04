#pragma once

#include "data.hpp"
#include "entry_cache.hpp"

#include <functional>
#include <mutex>

ContinuousEntry get_continuous_entry();

extern std::mutex push_event_based_mutex;

void generate_event_based_entries(bool* run, EntryCache** cache);