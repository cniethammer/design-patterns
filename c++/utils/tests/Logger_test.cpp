#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "Logger.h"

#include <sstream>
#include <string>

using namespace Log;
using namespace Catch::Matchers;

TEST_CASE("Log levels working", "[functionality]") {
  std::ostringstream log_output;

  Logger logger(logLevel::None, &log_output);

  logger.fatal() << "fatal message";
  REQUIRE_THAT(log_output.str(), !Contains("fatal message"));

  logger.set_log_level(logLevel::Fatal);
  logger.fatal() << "fatal message";
  REQUIRE_THAT(log_output.str(), Contains("fatal message"));

  logger.error() << "error message";
  REQUIRE_THAT(log_output.str(), !Contains("error message"));

  logger.set_log_level(logLevel::Error);
  logger.error() << "error message";
  REQUIRE_THAT(log_output.str(), Contains("error message"));

  logger.warning() << "warning message";
  REQUIRE_THAT(log_output.str(), !Contains("warning message"));

  logger.set_log_level(logLevel::Warning);
  logger.warning() << "warning message";
  REQUIRE_THAT(log_output.str(), Contains("warning message"));

  logger.info() << "info message";
  REQUIRE_THAT(log_output.str(), !Contains("info message"));

  logger.set_log_level(logLevel::Info);
  logger.info() << "info message";
  REQUIRE_THAT(log_output.str(), Contains("info message"));

  logger.debug() << "debug message";
  REQUIRE_THAT(log_output.str(), !Contains("debug message"));

  logger.set_log_level(logLevel::Debug);
  logger.debug() << "debug message";
  REQUIRE_THAT(log_output.str(), Contains("debug message"));

  log_output.str("");
  logger.set_log_level(logLevel::None);
  logger.fatal() << "no output";
  logger.error() << "no output";
  logger.warning() << "no output";
  logger.info() << "no output";
  logger.debug() << "no output";
  REQUIRE_THAT(log_output.str(), !Contains("no output"));

  log_output.str("");
  logger.set_log_level(logLevel::All);
  logger.fatal() << "all output fatal";
  REQUIRE_THAT(log_output.str(), Contains("all output fatal"));
  logger.error() << "all output error";
  REQUIRE_THAT(log_output.str(), Contains("all output error"));
  logger.warning() << "all output warning";
  REQUIRE_THAT(log_output.str(), Contains("all output warning"));
  logger.info() << "all output info";
  REQUIRE_THAT(log_output.str(), Contains("all output info"));
  logger.debug() << "all output debug";
  REQUIRE_THAT(log_output.str(), Contains("all output debug"));
}
