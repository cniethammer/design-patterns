#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "Logger.h"

#include <sstream>
#include <string>

using namespace Log;

TEST_CASE( "Log levels working", "[functionality]" ) {
    std::ostringstream log_output;

    Logger logger(logLevel::None, &log_output);

    logger.fatal()   << "fatal message";
    REQUIRE( log_output.str().find("fatal message") == std::string::npos );

    logger.set_log_level(logLevel::Fatal);
    logger.fatal()   << "fatal message";
    REQUIRE( log_output.str().find("fatal message") != std::string::npos );

    logger.error()   << "error message";
    REQUIRE( log_output.str().find("error message") == std::string::npos );

    logger.set_log_level(logLevel::Error);
    logger.error()   << "error message";
    REQUIRE( log_output.str().find("error message") != std::string::npos );

    logger.warning() << "warning message";
    REQUIRE( log_output.str().find("warning message") == std::string::npos );

    logger.set_log_level(logLevel::Warning);
    logger.warning() << "warning message";
    REQUIRE( log_output.str().find("warning message") != std::string::npos );

    logger.info()    << "info message";
    REQUIRE( log_output.str().find("info message") == std::string::npos );

    logger.set_log_level(logLevel::Info);
    logger.info()    << "info message";
    REQUIRE( log_output.str().find("info message") != std::string::npos );

    logger.debug()   << "debug message";
    REQUIRE( log_output.str().find("debug message") == std::string::npos );

    logger.set_log_level(logLevel::Debug);
    logger.debug()   << "debug message";
    REQUIRE( log_output.str().find("debug message") != std::string::npos );

    log_output.str("");
    logger.set_log_level(logLevel::None);
    logger.fatal()   << "no output";
    logger.error()   << "no output";
    logger.warning() << "no output";
    logger.info()    << "no output";
    logger.debug()   << "no output";
    REQUIRE( log_output.str().find("no output") == std::string::npos );

    log_output.str("");
    logger.set_log_level(logLevel::All);
    logger.fatal()   << "all output fatal";
    REQUIRE( log_output.str().find("all output fatal") != std::string::npos );
    logger.error()   << "all output error";
    REQUIRE( log_output.str().find("all output error") != std::string::npos );
    logger.warning() << "all output warning";
    REQUIRE( log_output.str().find("all output warning") != std::string::npos );
    logger.info()    << "all output info";
    REQUIRE( log_output.str().find("all output info") != std::string::npos );
    logger.debug()   << "all output debug";
    REQUIRE( log_output.str().find("all output debug") != std::string::npos );
}
