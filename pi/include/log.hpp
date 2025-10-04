#pragma once

#include "pi_config.hpp"

#include <iostream>
#include <fstream>


#ifdef LOG_TO_FILE
inline std::ofstream log_file(LOG_FILE_PATH);
inline std::ostream& log_stream = log_file;

#else
inline std::ostream& log_stream = std::cout;

#endif

#define LOG(x) log_stream << x << std::flush