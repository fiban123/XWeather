#pragma once

#include "data.hpp"
#include "entry_cache.hpp"

#include <functional>

ContinuousEntry get_continuous_entry();

void generate_event_based_entries(bool* run, DataEntryCache** cache);