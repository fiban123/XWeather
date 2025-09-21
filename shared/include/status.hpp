#pragma once

#include <string>
#include <vector>
#include <ctime>

enum StatusType{
    FAIL,
    SUCCESS
};

struct [[nodiscard]] Status{
    StatusType status_type;
    std::vector<std::pair<std::string, std::time_t> > traceback;

    Status(const StatusType status_type) : status_type(status_type){}
    
    Status(StatusType, const std::string msg);

    Status& add_trace(const std::string msg);

    bool fail();
};