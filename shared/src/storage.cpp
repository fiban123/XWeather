#include "storage.hpp"

#include "shared_config.hpp"
#include "storage_format.hpp"
#include "log.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

void StorageFile::update_path(){
    path = std::format("{}/{:02}_{:02}_{}.xwd", parent_dir->path, unsigned(md.month()), unsigned(md.day()), time);
}

Status StorageFile::check_existence(){
    std::error_code ec;

    if (std::filesystem::exists(path, ec)){
        if (ec) return gen_root_err(std::format("filesystem::exists F ({})({})", path, err_code_info(ec)));

        if (std::filesystem::is_regular_file(path, ec)){
            if (ec) return gen_root_err(std::format("filesystem::is_regular_file F ({})({})", path, err_code_info(ec)));

            return Status(SUCCESS);
        }
        else{
            std::filesystem::remove_all(path, ec);
            if (ec) return gen_root_err(std::format("filesystem::remove_all F ({})({})", path, err_code_info(ec)));
        }
    }
    std::ofstream file(path); // create empty file
    if (!file) {
        return gen_root_err(std::format("std::ofstream F ({})({})", path, err_code_info(ec)));
    }

    return Status(SUCCESS);
}

void StorageDirectory::update_path(){
    path = std::string(PI_WEATHERDATA_PATH) + "/" + std::to_string(static_cast<int>(year));
}

Status StorageDirectory::check_existence(){
    std::error_code ec;
    if (std::filesystem::exists(path, ec)){
        if (ec) return gen_root_err(std::format("filesystem::exists F ({})({})", path, err_code_info(ec)));

        if (std::filesystem::is_directory(path, ec)){
            if (ec) return gen_root_err(std::format("filesystem::is_directory F ({})({})", path, err_code_info(ec)));

            return Status(SUCCESS);
        }
        else{
            std::filesystem::remove_all(path, ec);
            if (ec) return gen_root_err(std::format("filesystem::remove_all F ({})({})", path, err_code_info(ec)));
        }
    }
    std::filesystem::create_directory(path, ec);
    if (ec) return gen_root_err(std::format("filesystem::exists F ({})({})", path, err_code_info(ec)));


    return Status(SUCCESS);
}

Status get_storage_file(EntryCache* cache, StorageFile* outfile, StorageDirectory *outdir){
    // get ymd from cache timestamp
    namespace chr = std::chrono;

    LocalDays day = get_day(cache->first_entry_timestamp);

    chr::year_month_day ymd(day);

    *outdir = StorageDirectory();
    outdir->year = ymd.year();
    outdir->update_path();
    Status s = outdir->check_existence();
    if (s.fail()) return gen_err("StorageDirectory::check_existence F", s);

    *outfile = StorageFile();
    outfile->md = chr::month_day(ymd.month(), ymd.day());
    outfile->parent_dir = outdir;
    outfile->time = chr::duration_cast<chr::seconds>(day.time_since_epoch()).count();
    outfile->update_path();
    s = outfile ->check_existence();
    if (s.fail()) return gen_err("StorageFile::check_existence F", s);

    return Status(SUCCESS);
}

Status store_cache(EntryCache* cache, StorageFile* file){
    StorageFileData file_data;
    
    parse_storage_file(file, &file_data);

    file_data.n_entry_types = cache->n_entry_types;

    file_data.continuous_entries.insert(file_data.continuous_entries.end(), cache->continuous_entry_cache.begin(), cache->continuous_entry_cache.end());

    
    assert(file_data.event_based_entries.size() == cache->n_event_based_entries || file_data.event_based_entries.size() == 0);
    file_data.event_based_entries.resize(cache->n_event_based_entries);

    for (size_t i = 0; i < cache->n_event_based_entries; i++){

        file_data.event_based_entries[i].insert(
            file_data.event_based_entries[i].end(), 
            cache->event_based_entry_cache[i].begin(), 
            cache->event_based_entry_cache[i].end()
        );
    }

    LOG("" << file_data.continuous_entries.size() << " continuous entries, " << file_data.event_based_entries[0].size() << "event-based entries of type 0\n"); 

    //file_data.print_data();

    write_storage_file(file, &file_data);

    return Status(SUCCESS);
}