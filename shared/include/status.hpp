#pragma once

#include <string>
#include <vector>
#include <ctime>
#include <system_error>
#include <source_location>

#define HERE std::source_location::current()

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

std::string err_code_info(std::error_code ec);

std::string code_position(const std::source_location& loc = std::source_location::current());

Status gen_err(std::string msg, Status s, const std::source_location& loc = std::source_location::current());
Status gen_root_err(std::string msg, const std::source_location& loc = std::source_location::current());