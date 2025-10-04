#pragma once

#include "status.hpp"
#include "entry_cache.hpp"

#include <string>
#include <chrono>

struct StorageDirectory;

struct StorageFile{
    UnixSecs time;
    std::chrono::month_day md;

    StorageDirectory* parent_dir;
    size_t file_size;

    std::string path;

    void update_path();

    Status check_existence();
};

struct StorageDirectory{
    std::chrono::year year;

    std::string path;

    void update_path();

    Status check_existence();
};

Status get_storage_file(EntryCache* cache, StorageFile* outfile, StorageDirectory *outdir);

Status store_cache(EntryCache* cache, StorageFile* file);