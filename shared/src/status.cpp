#include "status.hpp"

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
