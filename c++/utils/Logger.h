#ifndef LOGGER_H_
#define LOGGER_H_

#include <chrono>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <string>
#include <sstream>
#include <vector>

#ifdef ENABLE_MPI
#include <mpi.h>

#ifndef NDEBUG
/** When NDEBUG macro is undefined check the expression return value to be MPI_SUCCESS.
 *
 * Check expression to return MPI_SUCCESS. Can only be used after MPI_Init is called
 * because the Logger cannot be initialized before with the current implementation.
 */
#define MPI_CHECK(x) do {                   \
    int __ret;                              \
    if (MPI_SUCCESS != (__ret = (x)))       \
    Log::global_log->error() << "MPI returned with error code " << __ret << std::endl;  \
} while (0)

#else
#define MPI_CHECK(x) x
#endif

#endif  /* ENABLE_MPI */


/* we use a separate namespace because we have some global definitions for
 * the log level */
namespace Log {

class Logger;

/**
 * Gobal logger variable for use in the entire program.
 * Must be initialized with constructor e.g. new Log::Logger().
 * Namespace visibility:
 *   using Log::global_log;
 */
#ifndef LOGGER_SRC
extern Log::Logger *global_log;
#endif

/**
 * Available log levels
 * Levels have to be sorted in a way that more detailed levels have higher values.
 * For each level a name has to be specified in the init_log_levels() method
 * of the Logger() class. This name will be prepended later on to the log message.
 */
enum class logLevel {
	None,      /* supress output */
	Fatal,     /* unrecoverable error, program exit */
	Error,     /* error preventing to continue correctly in this part */
	Warning,   /* abnormal or unexpeced,  perhaps wrong */
	Info,      /* user info */
	Debug,     /* detailed info for debugging */
	All
};

/** @brief The Logger class provides a simple interface to handle log messages.
 *
 * Provides easy interface to handle log messages. Initialize either with
 * output level and stream or output level and filename or use default constructor
 * values (Error, &(std::cout)). With a given file basename and MPI Support each rank will
 * create and write to his own file.
 * For writing log messages use fatal(), error(), warning(), info() or debug() as
 * with normal streams, e.g.
 * > log.error() << "Wrong parameter." << endl;
 * For easy handling of output within MPI applications there are the following methods:
 * set_mpi_output_root(int root)
 * set_mpi_output_rall()
 * set_mpi_output_ranks(int num_ranks, int * ranks)
 * Please include std::endl statements at the end of output as they will flush
 * the stream buffers.
 * If ENABLE_MPI is enabled logger initialization has to take place after the
 * MPI_Init call.
 */
class Logger {
private:
	logLevel _log_level;
	logLevel _msg_log_level;
	bool _do_output;
	std::string _filename;
	std::ostream *_log_stream;
	std::map<logLevel, std::string> logLevelNames;
	std::chrono::steady_clock::time_point _starttime;

	int _rank;

	/// initilaize the list of log levels with the corresponding short names
	void init_log_levels() {
		logLevelNames.insert({logLevel::Fatal,   "FATAL ERROR"});
		logLevelNames.insert({logLevel::Error,   "ERROR"      });
		logLevelNames.insert({logLevel::Warning, "WARNING"    });
		logLevelNames.insert({logLevel::Info,    "INFO"       });
		logLevelNames.insert({logLevel::Debug,   "DEBUG"      });
	}

	// don't allow copy-construction
	Logger(const Logger&) : _log_level(logLevel::Error), _msg_log_level(logLevel::Error), _do_output(true),
			_filename(""), _log_stream(0), logLevelNames(), _starttime(), _rank(0)
	{ }

	// don't allow assignment
	Logger& operator=(const Logger&) { return *this; }

public:
	/** Initializes the log level, log stream and the list of log level names.
	 * If ENABLE_MPI is enabled by default all process perform logging output. */
	Logger(logLevel level = logLevel::Error, std::ostream *os = &(std::cout));

	Logger(logLevel level, std::string prefix);

	/// Destructor flushes stream
	~Logger();

	/// General output template for variables, strings, etc.
	template<typename T>
	Logger &operator<<(T const& t) {
		if (_msg_log_level <= _log_level && _do_output)
			*_log_stream << t;
		return *this;
	}

	/* Specialized versions for manipulators.  */
	// e.g. endl
	Logger &operator<<(std::ostream& (*f)(std::ostream&)) {
		if (_msg_log_level <= _log_level && _do_output)
			*_log_stream << f;
		return *this;
	}
	// e.g. hex.
	Logger &operator<<(std::ios_base& (*f)(std::ios_base&)) {
		if (_msg_log_level <= _log_level && _do_output)
			f(*_log_stream);
		return *this;
	}
	template<class Ch, class Tr>
	Logger &operator<<(std::basic_ios<Ch, Tr>& (*f)(std::basic_ios<Ch, Tr>&)) {
		if (_msg_log_level <= _log_level && _do_output)
			f(*_log_stream);
		return *this;
	}

	/// Add log info in front of messages
	Logger& msg_level(logLevel level) {
		using namespace std;
		_msg_log_level = level;
		if (_msg_log_level <= _log_level && _do_output) {
			*_log_stream << logLevelNames[level] << ":\t";
			// Include timestamp
			auto now_sys_time = std::chrono::system_clock::now();
			auto itt = std::chrono::system_clock::to_time_t(now_sys_time);
			stringstream timestampstream;
			timestampstream << setfill('0') << std::put_time(localtime(&itt), "%FT%T%Z");
			*_log_stream << timestampstream.str() << " ";
			// Include time since begin of logging
			auto now = std::chrono::steady_clock::now();
			*_log_stream << setw(12) << std::chrono::duration<double>(now - _starttime).count() << " ";
			// Include MPI rank
			*_log_stream << "[" << _rank << "]\t";
		}
		return *this;
	}

	/// shorthand versions for easy usage of the different log levels
	Logger& fatal() {
		return msg_level(logLevel::Fatal);
	}
	Logger& error() {
		return msg_level(logLevel::Error);
	}
	Logger& warning() {
		return msg_level(logLevel::Warning);
	}
	Logger& info() {
		return msg_level(logLevel::Info);
	}
	Logger& debug() {
		return msg_level(logLevel::Debug);
	}

	/// set log level
	logLevel set_log_level(logLevel l) {
		_log_level = l;
		return _log_level;
	}
	/// return log level
	logLevel get_log_level() {
		return _log_level;
	}

	/// initialize starting time
	void init_starting_time() {
		_starttime = std::chrono::steady_clock::now();
	}

	/* methods for easy handling of output processes */

	/// allow logging only for a single process
	void set_mpi_output_root(int root = 0);

	/// all processes shall perform logging
	void set_mpi_output_all();

	/// allow a set of processes for logging
	bool set_mpi_output_ranks(const std::vector<int> ranks);

}; /* end of class Logger */
} /* end of namespace */

#endif /*LOGGER_H_*/
