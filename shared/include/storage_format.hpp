#pragma once

#include "entry_cache.hpp"
#include "shared_config.hpp"
#include "storage_format.hpp"
#include "storage.hpp"

#include <vector>
#include <array>
#include <cassert>

static_assert(sizeof(double) == 8);
static_assert(sizeof(uint64_t) == 8);
static_assert(sizeof(size_t) == 8);

struct StorageFileData{
    size_t n_entry_types;

    std::vector<ContinuousEntryType> continuous_entries; // flattened 2d array
    std::vector<std::vector<EventBasedEntry> > event_based_entries;

    size_t get_size();

    void print_data();
};

void read_bytes(std::vector<char>& buffer, size_t* i, size_t n, void* dest);
void write_bytes(std::vector<char>& buffer, size_t* i, size_t n, void* src);

Status parse_storage_file(StorageFile* sfile, StorageFileData* out);
Status write_storage_file(StorageFile* sfile, StorageFileData* in);
