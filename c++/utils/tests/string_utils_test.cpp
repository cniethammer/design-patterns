#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "string_utils.h"

using namespace string_utils;

/* tests for join */
TEST_CASE("join(vec({1,2,3}), ', ') returns '1, 2, 3'", "[join]") {
  std::string str = join(std::vector<int>({1, 2, 3}), ", ");
  REQUIRE(str == "1, 2, 3");
}

TEST_CASE("join(str({'path1','path2'}), ':') returns 'path1:path2'", "[join]") {
  std::string str = join(std::vector<std::string>({"path1", "path2"}), ":");
  REQUIRE(str == "path1:path2");
}

/* tests for string_to_type */
TEST_CASE("string_to_type<int>('100') returns 100", "[string_to_type]") {
  REQUIRE(string_to_type<int>("100") == 100);
}

TEST_CASE("string_to_type<double>('3.14') returns 3.14", "[string_to_type]") {
  REQUIRE(string_to_type<double>("3.14") == 3.14);
}

TEST_CASE("string_to_type<string>(' Hello ') returns 'Hello'",
          "[string_to_type]") {
  REQUIRE(string_to_type<std::string>(" Hello ") == std::string("Hello"));
}

