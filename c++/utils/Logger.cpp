#define LOGGER_SRC

#include "Logger.h"

#include <algorithm>

namespace Log {

Logger *global_log;

Logger::Logger(logLevel level, std::ostream *os)
: _log_level(level), _msg_log_level(logLevel::Error), _do_output(true), _filename(""),
  _log_stream(os), logLevelNames(), _starttime(), _rank(0) {
	init_starting_time();
	this->init_log_levels();
#ifdef ENABLE_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &_rank);
#endif
}


Logger::Logger(logLevel level, std::string prefix) :  _log_level(level), _msg_log_level(logLevel::Error),
		_do_output(true), _filename(""), _log_stream(0), logLevelNames(), _starttime(), _rank(0) {
	init_starting_time();
	std::stringstream filenamestream;
	filenamestream << prefix;
#ifdef ENABLE_MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &_rank);
	filenamestream << "_R" << _rank;
#endif
	filenamestream << ".log";
	_filename = filenamestream.str();
	_log_stream = new std::ofstream(_filename.c_str());
}

Logger::~Logger() {
	*_log_stream << std::flush;
	if (_filename != "")
		(static_cast<std::ofstream*> (_log_stream))->close();
}


/// allow logging only for a single process
void Logger::set_mpi_output_root(int root) {
    set_mpi_output_ranks(std::vector<int>({root}));
}

/// all processes shall perform logging
void Logger::set_mpi_output_all() {
	_do_output = true;
}

/// allow a set of processes for logging
bool Logger::set_mpi_output_ranks(const std::vector<int> ranks) {
    if (std::find(ranks.begin(), ranks.end(), _rank) == ranks.end()) {
        _do_output = true;
    } else {
        _do_output = false;
    }
    return _do_output;
}

} /* end namespace Log */
