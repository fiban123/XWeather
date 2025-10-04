#include "status.hpp"

#include <format>

Status::Status(const StatusType status_type, const std::string msg): status_type(status_type){
    traceback.push_back(std::make_pair(msg, time(NULL)));
}

Status& Status::add_trace(const std::string msg){
    traceback.push_back(std::make_pair(msg, time(NULL)));

    return *this;
}

bool Status::fail(){
    return status_type == FAIL;
}

std::string code_position(const std::source_location& loc) {
    return std::format("{}:{} in {};", loc.file_name(), loc.line(), loc.function_name());
}

Status gen_root_err(std::string msg, const std::source_location &loc){
    return Status(FAIL, std::format("{} {}", code_position(loc), msg));
}

Status gen_err(std::string msg, Status s, const std::source_location &loc){
    return s.add_trace(std::format("{} {}", code_position(loc), msg));
}

std::string err_code_info(std::error_code ec){
    return "(ec: " + ec.message() + ", " + std::to_string(ec.value()) + ")";
}
