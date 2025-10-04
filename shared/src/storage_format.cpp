#include "storage_format.hpp"
#include "log.hpp"

#include <fstream>
#include <iostream>
#include <cstring>

void read_bytes(std::vector<char> &buffer, size_t* i, size_t n, void* dest){
    // read n bytes into dest
    assert(*i + n <= buffer.size());
    memcpy(dest, buffer.data() + *i * sizeof(char), n * sizeof(char));
    *i += n;
}

void write_bytes(std::vector<char> &buffer, size_t *i, size_t n, void *src){
    // write n bytes into buffer
    assert(*i + n <= buffer.size());
    memcpy(buffer.data() + *i * sizeof(char), src, n * sizeof(char));
    *i += n;
}

Status parse_storage_file(StorageFile *sfile, StorageFileData *out)
{
    std::ifstream sf(sfile->path, std::ios::binary | std::ios::ate);


    if (!sfile) return gen_root_err(std::format("std::ifstream F ({})", sfile->path));

    // read file


    size_t fsize = sf.tellg();
    sf.seekg(0, std::ios::beg);

    std::vector<char> buffer(fsize);
    if (!sf.read(buffer.data(), fsize)) {
        return gen_root_err(std::format("std::ifstream::read F ({})", sfile->path));
    }
    sf.close();

    if (buffer.size() < 3 * sizeof(uint64_t)){
        *out = StorageFileData();
        return Status(SUCCESS);
    }

    UnixMillis s = get_current_unix_millis();

    // parse continuous entry metadata

    size_t i = 0;

    uint64_t n_entries = 0;

    read_bytes(buffer, &i, sizeof(uint64_t), &n_entries);
    read_bytes(buffer, &i, sizeof(uint64_t), &(out->n_entry_types));

    //parse continuous entry data
    out->continuous_entries.resize(n_entries * out->n_entry_types);

    read_bytes(buffer, &i, n_entries * out->n_entry_types * sizeof(uint64_t), out->continuous_entries.data()); // read continuous entry vector

    //parse event-based entry metadata
    uint64_t n_event_based_entry_types;
    read_bytes(buffer, &i, sizeof(uint64_t), &n_event_based_entry_types);

    //parse event-based entries
    out->event_based_entries.resize(n_event_based_entry_types);

    for (size_t j = 0; j < n_event_based_entry_types; j++){
        uint64_t n_entries;
        read_bytes(buffer, &i, sizeof(uint64_t), &n_entries); // read n entries

        out->event_based_entries[j].resize(n_entries);

        read_bytes(buffer, &i, n_entries * sizeof(EventBasedEntry), out->event_based_entries[j].data()); // read event based entry vector
    }

    UnixMillis e = get_current_unix_millis();

    LOG("read " << e - s << " ms\n");

    return Status(SUCCESS);
}

Status write_storage_file(StorageFile *sfile, StorageFileData *in){
    // open file

    std::ofstream sf(sfile->path, std::ios::binary);


    if (!sfile) return gen_root_err(std::format("std::ofstream F ({})", sfile->path));

    std::vector<char> buffer(in->get_size());

    // write continuous entry metadata

    UnixMillis s = get_current_unix_millis();

    size_t i = 0;

    uint64_t n_entries = in->continuous_entries.size() / in->n_entry_types;

    write_bytes(buffer, &i, sizeof(uint64_t), &n_entries);
    write_bytes(buffer, &i, sizeof(uint64_t), &(in->n_entry_types));

    // write continuous entry data
    write_bytes(buffer, &i, n_entries * in->n_entry_types * sizeof(uint64_t), in->continuous_entries.data()); // write continuous entry vector

    // write event-based entry metadata
    uint64_t n_event_based_entry_types = in->event_based_entries.size();
    write_bytes(buffer, &i, sizeof(uint64_t), &n_event_based_entry_types);

    //write event-based entries
    for (size_t j = 0; j < n_event_based_entry_types; j++){
        // foreach entry type

        uint64_t n_entries = in->event_based_entries[j].size();
        write_bytes(buffer, &i, sizeof(uint64_t), &n_entries); // write number of entries

        write_bytes(buffer, &i, n_entries * sizeof(EventBasedEntry), in->event_based_entries[j].data()); // write entry vector
    }

    UnixMillis e = get_current_unix_millis();

    LOG("write " << e - s << " ms\n");

    // write buffer to file
    sf.write(buffer.data(), buffer.size());


    sf.close();

    return Status(SUCCESS);
}

size_t StorageFileData::get_size(){
    size_t size = sizeof(uint64_t) * 3;

    // continuous entries

    size += continuous_entries.size() * sizeof(ContinuousEntryType);

    // event-based entries
    for (size_t i = 0; i < event_based_entries.size(); i++){
        std::vector<EventBasedEntry>& eb_entry = event_based_entries[i];
        size += eb_entry.size() * sizeof(EventBasedEntry);
        size += sizeof(uint64_t);
    }

    return size;
}

void StorageFileData::print_data(){

    for (size_t i = 0; i < continuous_entries.size() / n_entry_types; i++){
        std::cout << "entry " << i << ": ";
        for (size_t j = 0; j < n_entry_types; j++){
            std::cout << continuous_entries[i * n_entry_types + j] << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for (size_t i = 0; i < event_based_entries.size(); i++){
        std::cout << "entry type " << i << ":\n";
        for (size_t j = 0; j < event_based_entries[i].size(); j++){
            std::cout << j << ": " << event_based_entries[i][j] << std::endl;
        }
    }
}
